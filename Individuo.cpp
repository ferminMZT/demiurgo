/**
  Individuo.cpp
  Implementacion de la clase Individuo GAP
*/


#include "Individuo.h"
#include "Constante.h"


// Constructor individual a partir de las operaciones y variables que forman sus nodos
Individuo::Individuo(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe)
{
 // Parte vectorial
 tabCtes = new Conjunto<Flt>(TAM_TABLA_GA);
 // Parte arborea
 arbolGap = new Arbol(cVar, *tabCtes, cOpe);
 numNodos    = arbolGap->getNumNodos();
 numIntrones = arbolGap->getNumIntrones();
 // De momento el error no esta inicializado (siempre debe estar en [0, INFINITO]
 errorGap = -1.;
 // Cargo con valores aleatorios en el intervalo [0, 1] la parte vectorial
 for(int n = 0; n < TAM_TABLA_GA; n++)
  (*tabCtes)[n] = (Flt) (RND(10001)/10000.);
 // Ahora hay que crear la pila piposeco para la evaluacion
 //pEval = new piposeco(arbolGap, numNodos);
}

// Para mostrar todas las tripas de las que se compone un individuo (direcciones incluidas)
void Individuo::ver()
{
 cout << "T{" << int(this) << "}" << endl;
 cout << "N:" << numNodos << " I:" << numIntrones << " E:" << errorGap << endl;
 cout << "A: [" << (int)arbolGap << "]: " << *arbolGap << endl;
// cout << "C: [" << (int)tabCtes << "] -> ";
// for(int n = 0; n < TAM_TABLA_GA; n++)
//  cout << "(" << n << ":" << (int)&((*tabCtes)[n]) << ")=" << (*tabCtes)[n] << " ";
// cout << endl;
// arbolGap->verIntrones();	// Para romper el arbol en sus intrones utiles
}

// La funcion fundamental es la de CRUCE, el que llama es el padre con la madre como parametro.
// Se devuelve un individuo hijo (en el sentido de que es mas parecido al que efectua la llamada...)
Individuo *Individuo::cruce(Individuo &iMadre)
{
 // OJO: Segun el preclaro Luciano, principe de los ingenios, el cruce sera de la parte arborea XOR
 // parte vectorial. NUNCA de ambas a la vez. Esto simplifica notablemente las cosas...
 // De momento creo a un hijo que es una copia exacta del padre (pero en diferente posicion de memoria)
 Individuo *elHijo = new Individuo(*this);

 if(COC())	     		// Cruce Arboreo: solo parte GP
 {
  Arbol *subArbolPaterno, *subArbolMaterno;	// El 1, se ve sustituido por el 2 en el individuo hijo
  int    nuevoNumNodos;				// Numero de nodos del posible hijo

  while(1)
  {
   // Genero las posiciones de corte para realizar el cruce (injerto)
   subArbolPaterno = elHijo->arbolGap->getPosicionCruce(elHijo->numNodos);
   subArbolMaterno = iMadre.arbolGap->getPosicionCruce(iMadre.numNodos);
   nuevoNumNodos = elHijo->numNodos - subArbolPaterno->getNumNodos() + subArbolMaterno->getNumNodos();
   // Solo admitire que se produzcan CRUCES que no superen el numero maximo de nodos ( <= NUM_MAX_NODOS )
   if(nuevoNumNodos <= NUM_MAX_NODOS)
    break;
  }

  // Y aplico el injerto sobre el propio hijo
  elHijo->arbolGap->cruce(subArbolPaterno, subArbolMaterno);
  // Se debe reajustar el numero de nodos del hijo, que suele diferir del que tenia!!!
  elHijo->numNodos = nuevoNumNodos;
  // Y le reajusto sus ptr a ctes de la tabla de constantes : A SU PROPIA TABLA DE CTES DEL HIJO!!!
  // Poco optimo, habra que revisarlo: es mejor hacerlo SOLO con la parte materna, que es la que cambia...
  elHijo->arbolGap->reajustaNodosCtes(elHijo->tabCtes);
 }
 else				// Cruce Vectorial: solo la parte GA con el vector de ctes
 {
  // De momento hago solo esto: Siendo P[0..N-1] y M[0..N-1] los vectores de ctes del padre y la madre
  // (con N=TAM_TABLA_GA), selecciono un pto de injerto en el intervalo [0, N); de 0 a ese pto, las
  // ctes son las del padre, y de ese pto hasta el final, las ctes son de la madre...
  // Posible mejora: Que ese pto no sea ni 0 ni el maximo?
  for(int ptoInjerto = RND(TAM_TABLA_GA); ptoInjerto < TAM_TABLA_GA; ptoInjerto++)
   (*elHijo)[ptoInjerto] = iMadre[ptoInjerto];
 }
 // En todo caso, es preciso darle a este hijo una estructura piposeco adecuada, ya que en
 // este momento tenemos toda la informacion necesaria para construirla
 //elHijo->pEval = new piposeco(elHijo->arbolGap, elHijo->numNodos);
 return elHijo;
}

// Funcion de CRUCE con intrones; el que llama es el padre con los siguientes parametros:
// la madre, un ptr al arbol paterno, otro al arbol materno y otro al arbol del hijo.
// Esos 3 punteros seran posteriormente utilizados para manejar los intrones...
// Se devuelve un individuo hijo (en el sentido de que es mas parecido al que efectua la llamada...)
Individuo *Individuo::cruceIntron(Individuo &iMadre, ptrArbol & pArbPad, ptrArbol & pArbMad, ptrArbol & pArbHij)
{
 Individuo *elHijo = new Individuo(*this);

 if(COC())	     		// Cruce Arboreo: solo parte GP teniendo en cuenta los intrones
 {
  Arbol  *subArbolPaterno, *subArbolMaterno;	// El 1, se ve sustituido por el 2 en el individuo hijo
  int     nuevoNumNodos;			// Numero de nodos del hijo
  Intron nIntAlePad, nIntAleMad;		// Posiciones aleatorias de cruce con intron para padre y madre

  while(1)
  {
   // Genero las posiciones de corte para realizar el cruce (injerto)
   nIntAlePad = Intron(RND(numIntrones) + 1);		// Posicion de corte buscada para el padre
   nIntAleMad = Intron(RND(iMadre.numIntrones) + 1);	// Posicion de corte buscada para el padre
   subArbolPaterno = elHijo->arbolGap->getPosicionCruceIntrones(/*elHijo->numIntrones,*/ nIntAlePad);
   subArbolMaterno = iMadre.arbolGap->getPosicionCruceIntrones(/*iMadre.numIntrones,*/ nIntAleMad);
   // Averiguo el numero de nodos del posible hijo fruto de tal union
   nuevoNumNodos = elHijo->numNodos - subArbolPaterno->getNumNodos() + subArbolMaterno->getNumNodos();
   // Solo admitire que se produzcan CRUCES que no superen el numero maximo de nodos ( <= NUM_MAX_NODOS )
   if(nuevoNumNodos <= NUM_MAX_NODOS)
    break;
  }
  // Se debe reajustar el numero de nodos e intrones del hijo, que suele diferir del que tenia!!!
  elHijo->numNodos    = nuevoNumNodos;
  elHijo->numIntrones -= subArbolPaterno->getNumIntrones();
  elHijo->numIntrones += subArbolMaterno->getNumIntrones();
  // Ajusto los parametros que se han pasado para devolverlos y manipular posteriormente los intrones
  pArbPad = arbolGap->getPosicionCruceIntrones(/*numIntrones,*/ nIntAlePad);
  pArbMad = subArbolMaterno;
  // Y aplico el injerto sobre el propio hijo
  pArbHij = elHijo->arbolGap->cruceIntron(subArbolPaterno, subArbolMaterno);
  // Y le reajusto sus ptr a ctes de la tabla de constantes : A SU PROPIA TABLA DE CTES DEL HIJO!!!
  // Poco optimo, habra que revisarlo: es mejor hacerlo SOLO con la parte materna, que es la que cambia...
  elHijo->arbolGap->reajustaNodosCtes(elHijo->tabCtes);
 }
 else				// Cruce Vectorial: solo la parte GA con el vector de ctes
 {
  // De momento hago solo esto: Siendo P[0..N-1] y M[0..N-1] los vectores de ctes del padre y la madre
  // (con N=TAM_TABLA_GA), selecciono un pto de injerto en el intervalo [0, N); de 0 a ese pto, las
  // ctes son las del padre, y de ese pto hasta el final, las ctes son de la madre...
  // Posible mejora: Que ese pto no sea ni 0 ni el maximo?
  for(int ptoInjerto = RND(TAM_TABLA_GA); ptoInjerto < TAM_TABLA_GA; ptoInjerto++)
   (*elHijo)[ptoInjerto] = iMadre[ptoInjerto];
  // Y finalmente se advierte de que no ha habido cambio de intron poniendo los ptros parametros a 0
  pArbPad = pArbMad = pArbHij = 0;
 }
 // En todo caso, es preciso darle a este hijo una estructura piposeco adecuada, ya que en
 // este momento tenemos toda la informacion necesaria para construirla
 //elHijo->pEval = new piposeco(elHijo->arbolGap, elHijo->numNodos);
 return elHijo;
}

// Otra funcion primordial es la MUTACION, que aunque menos importante que el cruce, sirve como expansor
// de la diversidad genetica de los individuos, sirviendo como posible alternativa al estrangulamiento o
// estancamiento de las poblaciones. A partir del individuo que hace la llamada se crea otro casi igual,
// pero con una pequeña mutacion aleatoria introducida ya sea en la parte vectorial o en la arborea.
// Como parametros se requieren el conjunto de variables y el de operaciones
Individuo *Individuo::mutacion(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe)
{
 Individuo *iMutado = new Individuo(*this);	// El individuo mutado

 if(COC())			// Mutacion de parte arborea 	(GP -> PA)
 {
  switch(RND(4))
  {
   case 0:
    iMutado->mutArbIntercambio();
    break;
   case 1:
    iMutado->mutArbNodoHomogenea(cVar, cOpe);
    break;
   case 2:
    iMutado->mutArbNodoHeterogenea(cVar);
    break;
   case 3:
    iMutado->mutArbContractiva(cVar, cOpe);
    break;
   /*
   case 4:
    mutArbInjerto();
    mutArbExpansiva();
    mutArbPermutacion();	// Interkambia 2 nodos entre si
    break;
   default:
    DEBUG("Error!");
   */
  }
  /*
  // Y podria hacer falta reasignar el numero de nodos del arbol recien mutado
  if(iMutado->numNodos != iMutado->arbolGap->getNumNodos() || iMutado->numIntrones != iMutado->arbolGap->getNumIntrones())
  {
   iMutado->ver();
   DEBUG("Error!!!");
  }
  */
  // Y posiblemente reajustar los punteros a las constantes
  //iMutado->arbolGap->reajustaNodosCtes(iMutado->tabCtes);
 }
 else				// Mutacion de parte vectorial 	(GA -> PV)
 {
  switch(RND(13))
  {
   case 0:
    iMutado->mutVecRotDer();
    break;
   case 1:
    iMutado->mutVecRotIzq();
    break;
   case 2:
    iMutado->mutVecCorte();
    break;
   case 3:
    iMutado->mutVecPermutacion();
    break;
   case 4:
    iMutado->mutVecIntercalado();
    break;
   case 5:
    iMutado->mutVecInversion();
    break;
   case 6:
    iMutado->mutVecSuavizado();
    break;
   case 7:
    iMutado->mutVecResaltado();
    break;
   case 8:
    iMutado->mutVecIntercambio();
    break;
   case 9:
    iMutado->mutVecAleatoria();
    break;
   case 10:
    iMutado->mutVecTotalAleatoria();
    break;
   case 11:
    iMutado->mutVecIncremental();
    break;
   case 12:
    iMutado->mutVecAdicion();
    break;
   default:
    DEBUG("Error!");
  }
 }
 // En todo caso, es preciso darle a este mutante una estructura piposeco adecuada, ya que en
 // este momento tenemos toda la informacion necesaria para construirla
 //iMutado->pEval = new piposeco(iMutado->arbolGap, iMutado->numNodos);
 return iMutado;
}

// Mutacion de la parte ARBOREA de un individuo por INTERCAMBIO de 2 subarboles suyos independientes en
// el sentido de que no esten contenidos entre si. Ejemplo:
/*
 				+   		f(X, Y) = 7*Y + (2*5 - X)
                               / \
                              /   \             Podria intercambiar entre si los subarboles
                             /     \            7*Y y 2*5
                            *       -
                           / \     / \          Pero no podria intercambiar 2*5 - X y 2*5 ya que el
                          /   \   /   \         primero contiene al segundo!!!
                         Y    7  *     X
  				/ \             Esta mutacion no afecta al numero de nodos ni al de
                               /   \		intrones
 			      2	    5
*/
void Individuo::mutArbIntercambio()
{
 Arbol *a, *b;			// Las ramas que se van a intercambiar entre si
 // Si el arbol es demasiado pequeño, es inutil intentarlo
 if(numNodos < 15)
  return;
 while(1)			// Hasta que no consiga 2 ramas independientes no paro:
 {				// Genero posiciones aleatorias y compruebo su independencia
  a = arbolGap->getPosicionCruce(numNodos);
  b = arbolGap->getPosicionCruce(numNodos);
  if( !(a->estaIncluido(b)) && !(b->estaIncluido(a)) )
   break;			// Cuando no hay inclusion, salto para el intercambio
 }
 // Mas facil que nada, intercambio entre si todos los punteros!!!
 arbolGap->intercambiaSubArboles(a, b);
}

// Mutacion de la parte ARBOREA de un individuo por CAMBIO HOMOGENEO de un nodo escogido al azar.
// var1 <-> var2 (distinta y escogida del conjunto de variables)
// ope1 <-> ope2 (distinta, sacada del conjunto de operaciones y con el mismo numero de parametros)
// cte1 <-> cte2 (distinta y sacada de la tabla de constantes del individuo)
void Individuo::mutArbNodoHomogenea(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe)
{
 Arbol *pArb     = arbolGap->getPosicionCruce(numNodos);	// Posicion del nodo a sustituir
 Nodo  *pNodOrig = pArb->getNodo();				// El propio nodo origen a sustituir
 Nodo  *pNod;						        // El sustituto homogeneo

 if(pNodOrig->esVariable())	// Variable por variable
  while(1)                      // Busco hasta dar con una variable distinta a la que tengo
  {
   pNod = cVar.getElementoAleatorio();
   // Contemplo que si hay 1 solo elemento, ese SI pueda ser escogido, pero si hay mas de 1, pues NO
   if( pNod != pArb->getNodo() || cVar.getCardinal() == 1 )
   {
    pArb->setNodo(pNod);		// Realizo el empalme
    numIntrones -= pArb->getIntron();	// Quito el anterior intron y agrego uno unitario
    pArb->setIntron(1);
    numIntrones++;
    return;			// Y a otra cosa mariposa!!!
   }
  }
 else
  if(pNodOrig->esOperacion())	// Operacion enaria por operacion enaria (de momento solo binaria)
  {
   if(pNodOrig->esBinario()) 	// Si es operacion binaria
    while(1)                    // Busco hasta dar con una operacion binaria y distinta a la que tengo
    {
     pNod = cOpe.getElementoAleatorio();
     if(pNod->esBinario() && pNod->getTipoNodo() != pNodOrig->getTipoNodo())
     {
      pArb->setNodo(pNod);		// Realizo el empalme
      numIntrones -= pArb->getIntron(); // Quito el anterior intron y agrego uno unitario
      pArb->setIntron(1);
      numIntrones++;
      return;			// Y a otra cosa mariposa!!!
     }
    }
   if(pNodOrig->esUnario()) 	// Si es unaria y hay mas unarias disponibles
    while(1)                    // Busco hasta dar con una operacion unaria (aunque sea igual a la que tengo, ya que
    {				// podria ser que solo hubiera una unaria)
     pNod = cOpe.getElementoAleatorio();
     if(pNod->esUnario())
     {
      pArb->setNodo(pNod);		// Realizo el empalme
      numIntrones -= pArb->getIntron(); // Quito el anterior intron y agrego uno unitario
      pArb->setIntron(1);
      numIntrones++;
      return;			// Y a otra cosa mariposa!!!
     }
    }
  }
  else				// Cte por Cte distinta y sacada de la tabla de ctes
  {
   int nuevoIndiceCte = RND(TAM_TABLA_GA);	// Nueva posicion de constante a la que se apuntara
   int viejoIndiceCte = pNodOrig->getIndice();	// Vieja posicion de cte a la que se apuntaba

   while(nuevoIndiceCte == viejoIndiceCte)      // Me aseguro de que cambie!!!
    nuevoIndiceCte = RND(TAM_TABLA_GA);
   pNodOrig->setIndice(nuevoIndiceCte);			// Cambio el indice y asi apunto a otra constante
   pNodOrig->setPtrCte(&(*tabCtes)[nuevoIndiceCte]);    // Ajusto el ptr para apuntar a esa nueva cte
   numIntrones -= pArb->getIntron();	  		// Quito el anterior intron y agrego uno unitario
   pArb->setIntron(1);
   numIntrones++;
  }
}

// Cambia un nodo por otro distinto pero de distinto tipo, por eso es heterogeneo
// Mutacion de la parte ARBOREA de un individuo por CAMBIO HETEROGENEO de un nodo escogido al azar, pero
// que sea TERMINAL: solo CTE o VAR.
// var1 <-> cte2
// cte1 <-> var2
void Individuo::mutArbNodoHeterogenea(Conjunto<ptrNodo> &cVar)
{
 Arbol *pArb     = arbolGap->getPosicionCruce(numNodos);	// Posicion del nodo a sustituir
 Nodo  *pNodOrig = pArb->getNodo();				// El propio nodo origen a sustituir

 while(pNodOrig->esNoTerminal())	// Me aseguro de conseguir un nodo TERMINAL (var o cte)
 {
  pArb     = arbolGap->getPosicionCruce(numNodos);
  pNodOrig = pArb->getNodo();
 }

 if(pNodOrig->esVariable())	     		// Var por cte
 {
  pNodOrig = new Constante(*tabCtes);		// Creo nodo cte dinamico (reemplaza el ptr a var statico)
  pArb->setNodo(pNodOrig);			// y lo pego al arbol en la posicion correcta
 }
 else						// Cte por var
 {
  delete pNodOrig;				// Borro la antigua cte (dinamica)
  pArb->setNodo(cVar.getElementoAleatorio());	// Realizo el empalme con la nueva variable (estatica)
 }
 // Hay que quitar el viejo intron del nodo (posiblemente distinto de 1) y poner uno unitario
 numIntrones -= pArb->getIntron();		// Quito el intron y agrego uno unitario
 pArb->setIntron(1);
 numIntrones++;
}

// Mutacion de la parte ARBOREA de un individuo por contraccion/expansion a partir de una posicion
// aleatoriamente seleccionada:
// -EXPANSION: Si hay un terminal (var o cte) se le suprime y se cuelga en su lugar un pequeño subarbol.
// -CONTRACCION: Si hay un NO terminal, se destruye ese subarbol y se cambia por un terminal (var o cte).
void Individuo::mutArbContractiva(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe)
{
 Arbol *pArb     = arbolGap->getPosicionCruce(numNodos);	// Posicion a comprimir/expandir
 Nodo  *pNodOrig = pArb->getNodo();				// El nodo en esa posicion
 Arbol *aIzq, *aDer = 0, *aCen = 0;				// Subarboles auxiliares a crear/borrar

 if(pNodOrig->esNoTerminal())	// CONTRACCION de no terminal; se poda y se pega terminal
 {
  // Si tiene pocos nodos, es preferible olvidarse de la contraccion
  if(numNodos <= 3)
   return;
  while(pNodOrig->esTerminal() || pArb == arbolGap)		// Me aseguro de NO tocar al nodo raiz,
  {								// repitiendo la seleccion del nodo
   pArb     = arbolGap->getPosicionCruce(numNodos);		// no terminal si fuera menester
   pNodOrig = pArb->getNodo();
  }
  // Actualizo el numero de nodos e intrones
  numNodos    -= pArb->getNumNodos();
  numIntrones -= pArb->getNumIntrones();
  // Destruyo todos los subarboles colgando de esa posicion, haciendo una brutal y emasculativa poda
  aIzq = pArb->getArbIzq();
  aDer = pArb->getArbDer();
  aCen = pArb->getArbCen();

  delete aIzq;                 	// SIEMPRE habra subarbol izquierdo al menos
  pArb->setArbIzq(0);
  if(aDer)
  {
   delete aDer;
   pArb->setArbDer(0);
  }
  if(aCen)
  {
   delete aCen;
   pArb->setArbCen(0);
  }
  // Y ahora pego un terminal por debajo, que escojo aleatoriamente (var o cte)
  pArb->setNodo((COC()) ? cVar.getElementoAleatorio() : new Constante(*tabCtes));
  // Incremento los nodos, le pongo un intron y aumento el numero de intrones
  numNodos++;
  pArb->setIntron(1);
  numIntrones++;
 }
 else				// Expansion de un terminal, convirtiendolo en un subarbol adicional
 {
  // Si tiene muchos nodos, es preferible olvidarse de la expansion, ya que
  // violaria el numero maximo de nodos...
  if(numNodos >= NUM_MAX_NODOS / 2) //NUM_MAX_NODOS - NUM_MAX_NODOS/10)
   return;
  // En todo caso le quito el intron que tuviera
  numIntrones -= pArb->getIntron();
  // Borrado del terminal
  if(pNodOrig->esConstante())	// Es un nodo constante, creado dinamicamente,
   delete pNodOrig;		// y tambien lo destruyo dinamicamente
  // Cojo una operacion aleatoriamente
  pNodOrig = cOpe.getElementoAleatorio();
  // Y le doy un intron unitario
  pArb->setIntron(1);
  numIntrones++;
  // La pego en la posicion de marras
  pArb->setNodo(pNodOrig);
  // Y segun el numero de parametros de la operacion, creo los subarboles pertinentes, pero
  // respetando el numero maximo de nodos permitido en un arbol (NUM_MAX_NODOS)
  int profLocal = PROF_INICIAL;
  while(1)
  {
   int    nuevoNumNodosTotal    = 0;
   Intron nuevoNumIntronesTotal = 0;

   profLocal--;

   // La operacion escogida es unaria, binaria o ternaria; pero siempre existira el subarbol izquierdo
   aIzq = new Arbol(cVar, *tabCtes, cOpe, profLocal);
   nuevoNumNodosTotal    += aIzq->getNumNodos();
   nuevoNumIntronesTotal += aIzq->getNumIntrones();
   if( !pNodOrig->esUnario() )	// Un nodo NO unario tiene siempre subarbol derecho (sea bin o ternario)
   {
    aDer = new Arbol(cVar, *tabCtes, cOpe, profLocal);
    nuevoNumNodosTotal    += aDer->getNumNodos();
    nuevoNumIntronesTotal += aDer->getNumIntrones();
    if(pNodOrig->esTernario())  // Y si ademas es ternario, pues tendra subarbol central
    {
     aCen = new Arbol(cVar, *tabCtes, cOpe, profLocal);
     nuevoNumNodosTotal    += aCen->getNumNodos();
     nuevoNumIntronesTotal += aCen->getNumIntrones();
    }
   }
   // Comprobamos si tenemos mas nodos de los permitidos
   if(numNodos + nuevoNumNodosTotal < NUM_MAX_NODOS)
   {
    numNodos    += nuevoNumNodosTotal;		// Actualizo el nuevo numero de nodos
    numIntrones += nuevoNumIntronesTotal;	// Y de intrones
    break;
   }
   // Sino, se destruyen los subarboles creados y se continua el proceso hasta lograr el numero adecuado
   if(aIzq)
    delete aIzq;
   if(aDer)
    delete aDer;
   if(aCen)
    delete aCen;
  }
  // Una vez llegado aqui, procedo a realizar el empalme (con perdon por la expresion) con los subarboles
  pArb->setArbIzq(aIzq);		// Ojo los que no sean necesario ya eran 0 (NULL)
  pArb->setArbDer(aDer);
  pArb->setArbCen(aCen);
  //arbolGap->reajustaNodosCtes(tabCtes); // Y se ajustan los nodos ctes con respecto a la actual tabla
  // Optimiza esto cabron!!!
 }
}

// Mutacion de la parte VECTORIAL de un individuo por ROTACION a la DERECHA: 12345 -> 51234
void Individuo::mutVecRotDer()
{
 for(int n = TAM_TABLA_GA - 1; n > 0; n--)
  intercambia((*tabCtes)[n], (*tabCtes)[n-1]);
}

// Mutacion de la parte VECTORIAL de un individuo por ROTACION a la IZQUIERDA: 12345 -> 23451
void Individuo::mutVecRotIzq()
{
 for(int n = 0; n < TAM_TABLA_GA - 1; n++)
  intercambia((*tabCtes)[n], (*tabCtes)[n+1]);
}

// Se parte aleatoriamente a partir de un pto y se combinan ambas mitades: 123*45678 -> 45678123
void Individuo::mutVecCorte()
{
 int posCorte = RND(TAM_TABLA_GA);

 if(posCorte < TAM_TABLA_GA/2)
  for( ; posCorte; posCorte--)
   mutVecRotIzq();
 else
  for( ; posCorte < TAM_TABLA_GA; posCorte++)
   mutVecRotDer();
}

// Reordena totalmente la parte vectorial, permutando sus elementos
void Individuo::mutVecPermutacion()
{
 for(int n = 0; n < TAM_TABLA_GA; n++)
  intercambia((*tabCtes)[n], (*tabCtes)[RND(TAM_TABLA_GA)]);
}

// Intercambia e intercala los elementos: 123456 -> 214365
void Individuo::mutVecIntercalado()
{
 for(int n = 0; n < TAM_TABLA_GA/2; n++)
  intercambia((*tabCtes)[2*n], (*tabCtes)[2*n + 1]);
}

// Invierte el orden de los elementos: 123456 -> 654321
void Individuo::mutVecInversion()
{
 for(int n = 0; n < TAM_TABLA_GA/2; n++)
  intercambia((*tabCtes)[n], (*tabCtes)[TAM_TABLA_GA - 1 - n]);
}

// Suaviza los componentes del vector de ctes mediante una media con los vecinos:
// PV[i] = (PV[i-1] + PV[i] + PV[i+1])/3.
void Individuo::mutVecSuavizado()
{
 Conjunto<Flt> cCtesViejo(*tabCtes);		// Para NO machacar los valores

 // El primer elemento depende del ultimo
 (*tabCtes)[0] = (Flt) (( cCtesViejo[TAM_TABLA_GA-1] + cCtesViejo[0] + cCtesViejo[1] ) / 3.);
 // Y el ultimo depende del primero
 (*tabCtes)[TAM_TABLA_GA-1] = (Flt) (( cCtesViejo[TAM_TABLA_GA-2] + cCtesViejo[TAM_TABLA_GA-1] + cCtesViejo[0] ) / 3.);
 // Los restantes dependen de sus vecinos a izquierda y derecha
 for(int n = 1; n < TAM_TABLA_GA - 1; n++)
  (*tabCtes)[n] = (Flt) (( cCtesViejo[n-1] + cCtesViejo[n] + cCtesViejo[n+1] ) / 3.);
}

// Resalta los componentes del vector de ctes mediante una media ponderada con los vecinos:
// PV[i] = (.5*PV[i-1] + 2.*PV[i] + .5*PV[i+1])/3.
void Individuo::mutVecResaltado()
{
 Conjunto<Flt> cCtesViejo(*tabCtes);		// Para NO machacar los valores

 // El primer elemento depende del ultimo
 (*tabCtes)[0] = (Flt) (( .5*cCtesViejo[TAM_TABLA_GA-1] + 2.*cCtesViejo[0] + .5*cCtesViejo[1] ) / 3.);
 // Y el ultimo depende del primero
 (*tabCtes)[TAM_TABLA_GA-1] = (Flt) ((( .5*cCtesViejo[TAM_TABLA_GA-2] + 2.*cCtesViejo[TAM_TABLA_GA-1] + .5*cCtesViejo[0] ) / 3.));
 // Los restantes dependen de sus vecinos a izquierda y derecha
 for(int n = 1; n < TAM_TABLA_GA - 1; n++)
  (*tabCtes)[n] = (Flt) (( .5*cCtesViejo[n-1] + 2.*cCtesViejo[n] + .5*cCtesViejo[n+1] ) / 3.);
}

// Escoge al azar 2 elementos del vector de ctes y los intercambia entre si
void Individuo::mutVecIntercambio()
{
 int pos1 = RND(TAM_TABLA_GA), pos2 = RND(TAM_TABLA_GA);

 if(pos1 != pos2)
  intercambia((*tabCtes)[pos1], (*tabCtes)[pos2]);
}

// Cambia totalmente un componente escogido al azar
void Individuo::mutVecAleatoria()
{
 (*tabCtes)[RND(TAM_TABLA_GA)] = (Flt) (RND(10001)/10000.);
}

// Cambia todos los componentes al azar
void Individuo::mutVecTotalAleatoria()
{
 for(int n = 0; n < TAM_TABLA_GA; n++)
  (*tabCtes)[n] = (Flt) (RND(10001)/10000.);
}

// Un elemento escogido al azar aumenta o disminuye su valor en un cierto porcentaje FACT_INCREM
void Individuo::mutVecIncremental()
{
 int pos = RND(TAM_TABLA_GA);

 if(COC())
  (*tabCtes)[pos] += (Flt) ((*tabCtes)[pos] * FACT_INCREM);
 else
  (*tabCtes)[pos] -= (Flt) ((*tabCtes)[pos] * FACT_INCREM);
}

// El elemento aumenta o disminuye su valor en una cantidad CANT_INCREM
void Individuo::mutVecAdicion()
{
 int pos = RND(TAM_TABLA_GA);

 if(COC())
  (*tabCtes)[pos] += (Flt) CANT_INCREM;
 else
  (*tabCtes)[pos] -= (Flt) CANT_INCREM;
}
