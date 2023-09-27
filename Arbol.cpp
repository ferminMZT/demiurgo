/*
  Arbol.cpp
  Implementacion de las fmc de la clase Arbol
*/

#include "Arbol.h"
#include "Constante.h"
#include "Utiles.h"
#include "Variable.h"

// Constructor complejo casi COMPLETO
Arbol::Arbol(int prof, Conjunto<ptrNodo> &cVar, Conjunto<Flt> &tCte, Conjunto<ptrNodo> &cOpe)
{
 if(prof == 1)			// Caso basico: alcanzada profundidad 1
 {				// El nodo es una hoja terminal (y nada por debajo) var o cte
  n = (COC()) ? cVar.getElementoAleatorio() : new Constante(tCte);
  nIntron = 1;			// Inicialmente, todo intron vale siempre 1
  i = d = c = 0;
 }
 else				// Sino, escoge un NO terminal (una operacion), y sigue...
 {
  n = cOpe.getElementoAleatorio();
  nIntron = 1;			// Inicialmente, todo intron vale siempre 1
  // Al menos siempre tiene la rama izquierda
  i = new Arbol(prof - 1, cVar, tCte, cOpe);
  if(n->esUnario())
  {
   d = c = 0;
   return;
  }
  d = new Arbol(prof - 1, cVar, tCte, cOpe);
  if(n->esTernario())
  {
   c = new Arbol(prof - 1, cVar, tCte, cOpe);
   return;
  }
  c = 0;
 }
}

// Constructor simple de arbol a partir de un nodo y 2 subarboles
Arbol::Arbol(Nodo *nod, Arbol *izq, Arbol *der, Arbol *cen)
{
 n = nod;
 nIntron = 1;		// Valor por defecto de un intron
 i = izq;
 d = der;
 c = cen;
}

// Constructor ALEATORIO sin complicarse la cabeza:
// ENORME PROBLEMA: Aunque sea casi imposible, la maldad del GNA hace muy probables tanto arboles de un solo
// nodo como arboles extremadamente grandes. Esto lo soluciono con un coeficiente aleatorio
Arbol::Arbol(Conjunto<ptrNodo> &cVar, Conjunto<Flt> &tCte, Conjunto<ptrNodo> &cOpe, int prof)
{
	// La probabilidad de escoger un terminal es 0 al principio de la creacion
	// y va creciendo hasta 100 al acabar en la profundidad maxima.
	// Se incrementa en funcion de la maxima profundidad a alcanzar...
	static Flt incProb = 100.f / prof;	// Incremento de la probabilidad
	static Flt probTerm = 0.f;			// Probabilidad de escoger terminal
	
	// Escoge un terminal (cte o var) y se acaba ahi esa rama sucesoria: es mas probable segun se baja...
	if(RND(100) < probTerm)
	{
		n = (COC()) ? cVar.getElementoAleatorio() : new Constante(tCte);
		nIntron = 1;
		i = d = c = 0;
	}
	else				
	// Sino, escoge un NO terminal (una operacion), y sigue...
	{				
		// Segun el tipo de operacion habra que crear mas o menos subarboles...
		n = cOpe.getElementoAleatorio();

		nIntron = 1;

		// En el proximo paso, la probabilidad de coger un terminal, se incrementa
		probTerm += incProb;

		if(n->esUnario())			// Si es unario no hay mas que hacer
		{
			// El subarbol izquierdo siempre existe y lleva un nodo mas
			i = new Arbol(cVar, tCte, cOpe, prof - 1);

			d = c = 0;
			// Al moverse hacia la raiz, hay que recuperar la anterior probabilidad
			probTerm -= incProb;
			return;
		}
		if(n->esTernario())			// Si es ternario, se agrega el subarbol central (condicion)
		{
			if(n->getTipoNodo() == CONDICIONAL)
			{
				// El subarbol izquierdo siempre existe y lleva un nodo mas
				i = new Arbol(cVar, tCte, cOpe, prof - 1);
			}
			else
			{
				// El operador =, debe llevar como subarbol izquierdo una constante!!!
				i = new Arbol(cVar.getElementoAleatorio());
			}

			// Aqui es binario como minimo
			d = new Arbol(cVar, tCte, cOpe, prof - 1);

			c = new Arbol(cVar, tCte, cOpe, prof - 1);
			// Al moverse hacia la raiz, hay que recuperar la anterior probabilidad
			probTerm -= incProb;
			return;
		}

		// Aqui es binario
		// El subarbol izquierdo siempre existe y lleva un nodo mas
		i = new Arbol(cVar, tCte, cOpe, prof - 1);
		// Aqui es binario como minimo
		d = new Arbol(cVar, tCte, cOpe, prof - 1);

		c = 0;
		// Al moverse hacia la raiz, hay que recuperar la anterior probabilidad
		probTerm -= incProb;
	}
}

// Constructor copia que duplica el arbol original dado como parametro:
// Puede haber una trampa, debido a que los nodos constantes NO se comparten nunca!!!
Arbol::Arbol(const Arbol &arbOrig)
{
 if(arbOrig.n->esConstante())
  n = new Constante(arbOrig.n->getIndice());
 else
  n = arbOrig.n;
 nIntron = arbOrig.nIntron;			// Copia el intron del original
 i = arbOrig.i ? new Arbol(*arbOrig.i) : 0;
 c = arbOrig.c ? new Arbol(*arbOrig.c) : 0;
 d = arbOrig.d ? new Arbol(*arbOrig.d) : 0;
}

// Evalua numericamente el arbol, en version recursiva
// QPP otras formas de evaluacion mas rapida...
// Esta es la forma mas rapida encontrada (al menos para el BC++ 3.1)
Flt Arbol::evalua()
{
 Flt alfa = .0;			// Valores iniciales de 0 para ambos subarboles
 Flt beta = .0;
 Flt iota = .0;

 if(i)
  alfa = i->evalua();			// Evaluo el subarbol izquierdo
 if(d)
  beta = d->evalua();			// Evaluo el subarbol derecho
 if(c)
  iota = c->evalua();			// Y el central (condicion)
 return n->evalua(&alfa, &beta, &iota);
}

// Para ver si un subarbol esta incluido dentro de otro, es decir, que cuelga de el.
// Esta funcion es primordial para las mutaciones arboreas por intercambio de partes... que solo se
// pueden realizar cuando las partes no tienen ninguna relacion de contencion y son independientes
int Arbol::estaIncluido(Arbol *a)
{
 // Caso basico: que el subarbol sea el propio arbol que llama, entonces esta contenido
 if(a == this)
  return VERDAD;
 // Caso recursivo, habra que irse por las ramas para seguirlo buscando
 if(i)				// Puede estar en la izquierda
  if(i->estaIncluido(a))
   return VERDAD;
 if(c)      			// O bien en la central
  if(c->estaIncluido(a))
   return VERDAD;
 if(d)				// O bien en la derecha
  if(d->estaIncluido(a))
   return VERDAD;
 return FALSO;			// O tambien puede NO estar en ninguna!!!
}

// Para intercambiar entre si dos subarboles independientes pertenecientes al arbol que llama;
// deben ser INDEPENDIENTES, es decir SIN NINGUNA RELACION de INCLUSION o CONTENCION
void Arbol::intercambiaSubArboles(Arbol *a, Arbol *b)
{
 intercambia(a->n, b->n);		// Intercambiar punteros a nodo
 intercambia(a->nIntron, b->nIntron);	// Intercambia numeros de intron
 intercambia(a->i, b->i);		// Intercambia punteros a subarboles
 intercambia(a->d, b->d);
 intercambia(a->c, b->c);
}



// Funciones intrinsecas de la GP ----------------------------------------------

// Para cruzar entre si 2 arboles, es necesario "cortar y pegar" entre 2 posiciones
// de los mismos (es decir intercambiar entre si 2 subarboles de los padres). Esas
// posiciones de "injerto" deben ser calculadas aleatoriamente. Eso se hace aqui.
// Ademas, se supone que no se es tan burro de cruzar un arbol vacio...
// Se da como parametro el numero de nodos de que consta el arbol entero
Arbol *Arbol::getPosicionCruce(int nNod)
{
 // Como lo hago??? Siendo nNod el numero de nodos del arbol, aplico un RND(nNod)
 // a un recorrido en preorden... OJO: Puede ser un metodo POCO igualitario.
 // Este es el metodo de la cuenta de la vieja que va oscilando recursivamente entre
 // los subarboles izquierdo, central y derecho de cada subarbol tratado...
 int    numeroCruce = RND(nNod) + 1;		// Posicion de corte buscada, en intervalo [0, nNod]
 int    numNodParc;				// Numero de nodos de un subarbol parcial por debajo
 Arbol *posCruce = this;			// Posicion para el injerto

 // La idea es que si un subarbol contiene mas nodos de los buscados, el punto estara dentro de el !!!

 while(numeroCruce != 1)			// Mientras que no este sobre el pto buscado
 {
  numeroCruce--;		// Quito el nodo raiz, que no es el buscado
  if(posCruce->i)		// Si hay subarbol izquierdo, veamos si esta en el
  {
   numNodParc = posCruce->i->getNumNodos();	// Nodos del subarbol izquierdo
   // Si hay mas nodos en el izquierdo de los buscados, aqui esta!
   if(numNodParc >= numeroCruce)		// Debe estar en el subarbol izquierdo
   {
    posCruce = posCruce->i;			// Saltamos a ese subarbol
    continue;
   }
   numeroCruce -= numNodParc;
  }
  if(posCruce->c)
  {
   numNodParc = posCruce->c->getNumNodos();
   if(numNodParc >= numeroCruce)
   {
    posCruce = posCruce->c;
    continue;
   }
   numeroCruce -= numNodParc;
  }
  if(posCruce->d)
  {
   numNodParc = posCruce->d->getNumNodos();
   if(numNodParc >= numeroCruce)
   {
    posCruce = posCruce->d;
    continue;
   }
   numeroCruce -= numNodParc;
  }
 }
 return posCruce;
}


// Igual que la anterior, pero teniendo en cuenta los intrones...
// Se da como parametro el numero total de intrones de que consta el arbol entero. La idea basica es separar:
// 0 1 2 3 4 5 6 7 8 9          0 1  2 3   4 5 6 7 8 9
// |_|_|_|_|_|_|_|_|_|          |_|__|_|___|_|_|_|_|_|
Arbol *Arbol::getPosicionCruceIntrones(/*Intron nInt,*/ Intron nIntAleatorio)
{
 // Aplico un RND(nNod) a un recorrido en preorden... con las limitaciones dadas por los intrones, usando el
 // algoritmo del profesor K.Wahall...
 Intron nCruceInt = nIntAleatorio;	// Posicion de corte buscada, en intervalo [0, nInt]
 Intron nParcInt;					// Numero de intrones de un subarbol parcial por debajo
 Arbol *posCruceInt = this;			// Posicion para el injerto

 // La idea es pivotar sucesivamente entre la raiz de un subarbol, su subarbol izquierdo y el derecho sucesiva y
 // recursivamente hasta que el numero de intron de la subraiz alcanzada sea mayor o igual que la actual cuenta de
 // intrones que va decreciendo (restando el numero de intrones de cada raiz que NO es...)
 // Por ejemplo, dado un arbol A con nTotalIntrones = 77, y sacado un numero aleatorio de cruce n = 32, y con esta
 // disposicion de intrones en la raiz y en los subarboles izquierdo y derecho:
 // 32 -> 1      Ojo: 46 + 30 + 1 = 77
 //      / \     Como 32 > 1, se resta 31 = 32 - 1 y se pasa al subarbol izquierdo:
 //     /   \		31 -> 10	10 + 7 + 29 = 46
 //   46     30              /  \       31 > 10 -> 21 = 31 - 10 y pasa al subarbol izquierdo:
 //                         /    \		21 -> 7    21 > 7 -> 14 = 21 - 7 y pasa al subarbol derecho:
 //                        7     29				14 -> 25   ->  14 <= 25 *** Hemos alcanzado el
 //                                                                    |		    punto de cruce buscado
 //								       4

 while(nCruceInt > posCruceInt->nIntron)	// Mientras que no este sobre el pto buscado
 {
  //cout << nCruceInt << ":" << posCruceInt->nIntron << endl;
  //cout << *posCruceInt << endl;
  nCruceInt -= posCruceInt->nIntron;	// Resto el numero de intrones del nodo raiz, ya que este no es el buscado
  if(posCruceInt->i)			// Si hay subarbol izquierdo, veamos si esta en el
  {
   nParcInt = posCruceInt->i->getNumIntrones();	// Nodos del subarbol izquierdo
   //cout << "I:" << nParcInt << endl;
   // Si el numero de intrones en este subarbol izquierdo es MAYOR que el numero de intrones que todavia nos quedan de
   // la posicion de corte, entonces es evidente que el punto de corte esta en este subarbol izquierdo!!!
   if(nParcInt >= nCruceInt)		// Debe estar en el subarbol izquierdo
   {
    posCruceInt = posCruceInt->i;	// Saltamos a ese subarbol
    continue;                    	// Y continuamos el avance por su interior
   }
   nCruceInt -= nParcInt;		// Sino, se descuentan los intrones de este subarbol y se avanza al siguiente
  }
  if(posCruceInt->c)
  {
   nParcInt = posCruceInt->c->getNumIntrones();
   //cout << "I:" << nParcInt << endl;
   if(nParcInt >= nCruceInt)
   {
    posCruceInt = posCruceInt->c;
    continue;
   }
   nCruceInt -= nParcInt;
  }
  if(posCruceInt->d)
  {
   nParcInt = posCruceInt->d->getNumIntrones();
   //cout << "I:" << nParcInt << endl;
   if(nParcInt >= nCruceInt)
   {
    posCruceInt = posCruceInt->d;
    continue;
   }
   nCruceInt -= nParcInt;
  }
 }
 //cout << " >> "<< *posCruceInt << endl;
 return posCruceInt;
}


// Funcion de cruce de 2 arboles:
// - El hijo (identico al padre) es el que hace la llamada, con los parametros (punteros a arbol)
//	Subarbol del padre que cambiamos: trozo del hijo que se pierde
//	Subarbol aportado por la madre que sera implantado en la anterior posicion.
// arbHijo.cruce(subArbPadre, subArbMadre);
// Nota: Para evitar los errores del CODEGUARD y que se cumpla num(new) = num(delete) hago la cosa rara
// de crear 3 subarboles a partir del posMad
void Arbol::cruce(Arbol *posPad, Arbol *posMad)
{
 // Sobre el que llama (this), corto el subarbol posPad (que es parte de el) y lo sustituyo
 // por el subarbol posMad. Como es logico el objeto que llama, que debe ser un hijo
 // (inicialmente igual a su padre), se ve cambiado tras este proceso.
 Arbol *ptrNuevoSubArbolIzq = 0, *ptrNuevoSubArbolDer = 0, *ptrNuevoSubArbolCen = 0;

 if(posMad->i)
  ptrNuevoSubArbolIzq = new Arbol(*posMad->i);
 if(posMad->d)
  ptrNuevoSubArbolDer = new Arbol(*posMad->d);
 if(posMad->c)
  ptrNuevoSubArbolCen = new Arbol(*posMad->c);
 // Si hay un nodo que es constante (fue creado dinamicamente), debe tambien borrarse dinamicamente!!!
 if(posPad->n->esConstante())
  delete posPad->n;
 if(posMad->n->esConstante())
  posPad->n = new Constante(posMad->n->getIndice());
 else
  posPad->n = posMad->n;
 // Borro los subarboles del viejo arbol
 if(posPad->i)
  delete posPad->i;
 if(posPad->d)
  delete posPad->d;
 if(posPad->c)
  delete posPad->c;
 // Aqui se pega el nuevo subarbol pieza a pieza: a partir de los 3 subarboles...
 posPad->i = ptrNuevoSubArbolIzq;
 posPad->d = ptrNuevoSubArbolDer;
 posPad->c = ptrNuevoSubArbolCen;
}

// Lo mismo de antes pero adecuado para el manejo de intrones...
Arbol *Arbol::cruceIntron(Arbol *posPad, Arbol *posMad)
{
 Arbol *ptrNuevoSubArbolIzq = 0, *ptrNuevoSubArbolDer = 0, *ptrNuevoSubArbolCen = 0;

 if(posMad->i)
  ptrNuevoSubArbolIzq = new Arbol(*posMad->i);
 if(posMad->d)
  ptrNuevoSubArbolDer = new Arbol(*posMad->d);
 if(posMad->c)
  ptrNuevoSubArbolCen = new Arbol(*posMad->c);
 // Si hay un nodo que es constante (fue creado dinamicamente), debe tambien borrarse dinamicamente!!!
 if(posPad->n->esConstante())
  delete posPad->n;
 if(posMad->n->esConstante())
  posPad->n = new Constante(posMad->n->getIndice());
 else
  posPad->n = posMad->n;
 // Y el intron correspondiente
 posPad->nIntron = posMad->nIntron;
 // Borro los subarboles del viejo arbol
 if(posPad->i)
  delete posPad->i;
 if(posPad->d)
  delete posPad->d;
 if(posPad->c)
  delete posPad->c;
 // Aqui se pega el nuevo subarbol pieza a pieza: a partir de los 3 subarboles...
 posPad->i = ptrNuevoSubArbolIzq;
 posPad->d = ptrNuevoSubArbolDer;
 posPad->c = ptrNuevoSubArbolCen;
 // Y finalmente devuelvo el puntero pedido
 return posPad;
}

// Para crear una pila a partir de un arbol, es preciso recorrerlo en PRE-ORDEN e ir
// apilando sus nodos empezando por el raiz
int Arbol::apilarPreOrden(EntradaPila *vPila, int numeroEntradas)
{
	static int indice = 0;		// Indice de la pila, comenzando en 0 (nodo raiz)
	int indiceTemp;     		// Para contrarrestar el caracter static de indice...
	
	// Apila en cada paso el nodo raiz
	if(n)
	{
		// Segun el tipo, la entrada de la pila variara...
		vPila[indice].tipoNodo = n->getTipoNodo();

		// Si es operacion, hay que asignar espacio temporal para almacenar su calculo
		if(n->esOperacion())
		{
			vPila[indice].valorNodo = new Flt;
			indiceTemp = indice;
			indice++;

			// Debajo de una operacion, siempre habra mas cosas
			if(n->esUnario())		// Las operaciones unarias tienen su unico parametro
			{                            // justo a continuacion en la pila, sin mas problemas
				i->apilarPreOrden(vPila, numeroEntradas);
				vPila[indiceTemp].param2 = 0;
				vPila[indiceTemp].param3 = 0;
				vPila[indiceTemp].aridad = 1;	// Aridad 1
			}
			else
				if(n->esBinario())		// Las binarias tienen el 1º parametro a continuacion
				{				// y el 2º como raiz de su subarbol derecho
					i->apilarPreOrden(vPila, numeroEntradas);
					vPila[indiceTemp].param2 = d->apilarPreOrden(vPila, numeroEntradas);
					vPila[indiceTemp].param3 = 0;
					vPila[indiceTemp].aridad = 2;	// Aridad 2
				}
				else			// Las ternarias tienen el 1º a continuacion, el 2º como
				{				// raiz de su subarbol central y el 3º en el derecho
					i->apilarPreOrden(vPila, numeroEntradas);
					vPila[indiceTemp].param2 = c->apilarPreOrden(vPila, numeroEntradas);
					vPila[indiceTemp].param3 = d->apilarPreOrden(vPila, numeroEntradas);
					vPila[indiceTemp].aridad = 3;	// Aridad 3
				}
		}
		else
		{
			// Sea variable o constante, se le pega al lugar donde guarda su valor
			vPila[indice].valorNodo = n->getPtrValor();
			
			// En las variables, como sus parametros son inutiles, aprovecho el primero para guardar su nombre
			if(n->esVariable())
				// Para poder usar una FMC de variable, pero que no es de nodo
				vPila[indice].param2 = (int)(((Variable *)n)->getNombre());
			else
				vPila[indice].param2 = 0;
			
			vPila[indice].param3 = 0;
			// Y ademas tiene una aridad nula
			vPila[indice].aridad = 0;
			// Y como por debajo nada puede haber, se retorna!!!
			indiceTemp = indice;
			indice++;
			
			// Esto evita un grave BUG: Pone a cero la variable estatica indice de cara a proximas
			// ejecuciones de esta funcion, pero solo cuando alcanza su valor maximo y se retorna
			if(indice == numeroEntradas)
				indice = 0;
		}
		return indiceTemp;
	}

	DEBUG("Error!!!");
	return 0;
}

// Para visualizar los subarboles interiores con intrones no unitarios
void Arbol::verIntrones()
{
 if(nIntron > 1)
  cout << "I=" << nIntron << " -> " << *this << endl;
 if(i)
  i->verIntrones();
 if(c)
  c->verIntrones();
 if(d)
  d->verIntrones();
}

// Redefino el operador << para un cout en IN-ORDEN: a+b
// Es tonto pasarlo a polaca... y esto es otra muestra de mi talento (asexual)
ostream & operator << (ostream& os, Arbol &a)
{
 if(a.getNodo()->esTerminal())	// Si el nodo es terminal, solo hay que imprimirlo
  a.getNodo()->operator<<(os);
 else
 {
  // Cuando un nodo es NO TERMINAL, se imprime en IN-ORDEN:
  // Para las operaciones BINARIAS:
  //	1) Imprime el subarbol izquierdo.       ->	a + (b + c)
  //	2) Imprime el propio nodo.		->	a + b
  //	3) Imprime el subarbol derecho.
  // Para las operaciones UNARIAS:
  //	1) Imprime el propio nodo               ->	cos(a) , cos(a+b), pow(x, 2.)
  //	2) Imprime el subarbol izquierdo	->     -a, -(a + b)
  // Para las operaciones TERNARIAS:
  //	1) Imprime el subarbol izquierdo (if)	-> (a+b) ? (x) : ((d*c) + d))
  //	2) Imprime algun separador (el nodo)
  //	3) Imprime el subarbol central (then)
  //	4) Imprime algun separador
  //	5) Imprime el subarbol derecho (else)
  // En todo caso, las cosas simples van sin parentesis:	a + b
  // Y las complejas llevan parentesis:                         a + (b * c), -(a + b)
  // Todo lo afectado por un operador unario va entre parentesis, sea simple o complejo
  // Y todo lo afectado por un operador ternario tambien!!!
  if(a.getNodo()->esUnario())
  {
   a.getNodo()->operator<<(os);
   os << "(";
   os << *(a.getArbIzq());
   // Aqui se hace la comprobacion para el cuadrado: x^2 = pow(x, 2.) (unario)
   if(a.getNodo()->getTipoNodo() == CUADRADO)
    os << ", 2.0";
   os << ")";
  }
  else
   if(a.getNodo()->esTernario())	// De momento esto esta solo pensado para el IF
   {
    os << "((";
    os << *(a.getArbIzq());
    os << " > 0.0) ";
    a.getNodo()->operator<<(os);
    os << " (";
    os << *(a.getArbCen());
    os << ") : (";
    os << *(a.getArbDer());
    os << "))";
   }
   else				// Es binario
   {
    if(a.getArbIzq()->getNodo()->esTerminal())
     os << *(a.getArbIzq());
    else
    {
     os << "(";
     os << *(a.getArbIzq());
     os << ")";
    }
    os << " ";
    a.getNodo()->operator<<(os);
    os << " ";
    if(a.getArbDer()->getNodo()->esTerminal())
     os << *(a.getArbDer());
    else
    {
     os << "(";
     os << *(a.getArbDer());
     os << ")";
    }
   }
 }
 return os;
}