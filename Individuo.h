/* 
  Individuo.h
  Una poblacion estara formada por individuos GAP, constituidos por un arbol, una
  tabla de n constantes, una medida del error cometido por el individuo y el numero
  de nodos del arbol. 
*/


#ifndef INDIVIDUO_H
#define INDIVIDUO_H


#include "Conjunto.h"
#include "Nodo.h"
#include "Arbol.h"
#include "Piposeco.h"
#include "Utiles.h"


class Individuo
{
 public:

  // Constructor de un individuo Gap a partir de un conj de variables y operaciones
  // Las constantes ya las calcula el mismito...
  Individuo(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe);

  // Constructor copia de un individuo a partir de otro dado
  Individuo(const Individuo &iOrig)
  {
   tabCtes  = new Conjunto<Flt>(*iOrig.tabCtes);
   arbolGap = new Arbol(*iOrig.arbolGap);
   numNodos    = iOrig.numNodos;
   numIntrones = iOrig.numIntrones;
   errorGap    = iOrig.errorGap;
   // Ojo, queda por resolver que las nuevas constantes de la parte arborea del individuo recien creado
   // apunten a donde deben, a su propia tabla de ctes y no a la del individuo original!!!
   arbolGap->reajustaNodosCtes(tabCtes);
   // Ojo: No tiene sentido crear aqui la pila piposeco, pues en un cruce lo mas probable es
   // que esta pila no se pareciese casi nada a la de sus progenitores...
   // Esto se hace tras consumar el cruce...
  }

  // Otro constructor, que crea un individuo a partir de un arbol y una tabla de ctes dados
  Individuo(Arbol *pArb, Conjunto<Flt> *pTab)
  {
   tabCtes  = new Conjunto<Flt>(*pTab);
   arbolGap = new Arbol(*pArb);
   numNodos    = arbolGap->getNumNodos();
   numIntrones = arbolGap->getNumIntrones();
   errorGap = 0.0;
   // Ojo, queda por resolver que las nuevas constantes de la parte arborea del individuo recien creado
   // apunten a donde deben, a su propia tabla de ctes y no a la del individuo original!!!
   arbolGap->reajustaNodosCtes(tabCtes);
  }

  // Para inicializar un vector de individuos, hay que tener un constructor por defecto sin argumentos
  Individuo()
  { ; }

  // Destructor : tengase en cuenta que las variables y operaciones a las que apunta el arbol SIEMPRE
  // deben ser destruidas TRAS destruir a ese individuo, NUNCA ANTES
 ~Individuo()
  {
   if(numNodos)
   {
    delete arbolGap;
    delete tabCtes;
    //delete pEval;
    numNodos    = 0;
    numIntrones = 0;
   }
  }

  // Para obtener la parte arborea del individuo Gap
  Arbol *getArbol()
  { return arbolGap; }

  // Y para cambiarla (cuidado, pues borra la anterior que hubiera) y no cambia el numero de nodos e intrones
  void setArbol(Arbol *pArb)
  {
   if(arbolGap)
    delete arbolGap;
   arbolGap = new Arbol(*pArb);
   arbolGap->reajustaNodosCtes(tabCtes);
  }

  // Solo para cambiar parte arborea, sin ningun otro reajuste (refinar...)
  void cambiaArbol(Arbol *pArb)
  { arbolGap = pArb; }

  // Para asignar una pila piposeco a un individuo que aun no la tiene
  //void asignaPila()
  //{ pEval = new piposeco(arbolGap, numNodos); }

  // Para obtener la parte vectorial del individuo
  Conjunto<Flt> *getTablaCtes()
  { return tabCtes; }

  // Para obtener una a una las constantes de la parte vectorial, redefino el operador []
  Flt & operator [] (int n)
  { return (*tabCtes)[n]; }

  // Evaluador de individuos
  Flt evalua()
  {
   return arbolGap->evalua();
   //Flt vPila  = pEval->evalua();
   //return vPila;
  }

  // Para saber el numero de nodos
  int getNumNodos()
  { return numNodos; }

  // Para saber el numero de intrones
  Intron getNumIntrones()
  { return numIntrones; }

  // Para ajustar el error del individuo
  void setError(Flt error)
  { errorGap = error; }

  // Para saber el error
  Flt getError()
  { return errorGap; }

  // Para mostrar todas las tripas de las que se compone un individuo (direcciones incluidas)
  void ver();
  // Un visor de individuos simplificados...
  void verSimplificado()
  {
   // Se construye una pila piposeco a partir del individuo dado, sobre la cual se simplificara
   Piposeco pilaSimplificacionEvaluacion(arbolGap, numNodos);
   // Y se ve esa pila simplificada
   pilaSimplificacionEvaluacion.verSimplificado();
   cout << endl;
  }

  // La funcion fundamental es la de CRUCE, el que llama es el padre con la madre como parametro.
  // Se devuelve un individuo hijo (en el sentido de que es mas parecido al que efectua la llamada...)
  Individuo *cruce(Individuo &iMadre);

  // Si en el cruce se emplean intrones, es necesario tener identificados los subarboles paterno, materno y del hijo
  // para poder re-asignar sus intrones... La direccion es necesaria para poder reasignar los punteros parametro
  Individuo *cruceIntron(Individuo &iMadre, ptrArbol & pArbPad, ptrArbol & pArbMad, ptrArbol & pArbHij);

  // Para manejar los intrones en ciertas posiciones del arbol del individuo
  void incIntron(ptrArbol pArb)
  {
   pArb->incIntron();
   numIntrones++;
  }
  void decIntron(ptrArbol pArb)
  {
   if(pArb->getIntron() > 1)
   {
    pArb->decIntron();
    numIntrones--;
   }
  }
  // Otra funcion muy importante es la mutacion, que a partir del individuo que hace la llamada crea
  // otro casi igual, pero con una pequeña mutacion aleatoria introducida. Podria mutarse el intron???
  Individuo *mutacion(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe);

  // Mi modesta aportacion: el refinado de individuos, comprobando sus
  // subarboles con la funcion externa. Si alguno de ellos es mejor que
  // el arbol total del individuo, el mejor subarbol toma ese puesto.
  // Solo para los primeros descendientes del nodo raiz.
  // Devuelve VERDAD (1) si ha habido refinamiento
  int refinaIndividuo(Flt (*pFuncExtErr)(Individuo *, ptrConjNodos), Conjunto<ptrNodo> *cVar)
  {
   Arbol *pArb = arbolGap;		// Arbol del individuo
   Nodo  *pNod = pArb->getNodo();	// Nodo raiz de ese arbol
   // Compruebo que NO reciba un arbol degenerado compuesto por un solo nodo (y terminal). Se podria hacer
   // solo con el numero de nodos (mas rapido), pero con mi forma se podra usar la recursividad...
   if(pNod->esTerminal())
    return FALSO;

   Flt    errMejor = errorGap;   	// El mejor error es el del arbol completo
   Arbol *elMejor  = 0;			// De momento no hay subarbol mejor
   Flt    errIndIzq, errIndDer = INFINITO, errIndCen = INFINITO;
   // Proceso el subarbol izquierdo (que siempre existe), para evaluar su error
   Arbol *pArbIzq = pArb->getArbIzq();

   arbolGap  = pArbIzq;			// Lo coloco como arbol raiz
   errIndIzq = pFuncExtErr(this, cVar); // Y lo evaluo

   // Proceso el subarbol derecho (que no existe solo para nodos unarios)
   Arbol *pArbDer = pNod->esUnario() ? 0 : pArb->getArbDer();

   if(pArbDer)
   {
    arbolGap  = pArbDer;			// Lo coloco como raiz
    errIndDer = pFuncExtErr(this, cVar); 	// Y lo evaluo
   }

   // Y proceso el subarbol central, que solo existe en nodos ternarios
   Arbol *pArbCen = pNod->esTernario() ? pArb->getArbCen() : 0;

   if(pArbCen)
   {
    arbolGap  = pArbCen;			// Lo coloco como raiz
    errIndCen = pFuncExtErr(this, cVar); 	// Y lo evaluo
   }

   // Veamos cual de todos (raiz, izquierdo, derecho y central) es el mejor
   if(errIndIzq < errMejor)
   {
    elMejor  = pArbIzq;
    errMejor = errIndIzq;
   }
   if(errIndDer < errMejor)
   {
    elMejor  = pArbDer;
    errMejor = errIndDer;
   }
   if(errIndCen < errMejor)
   {
    elMejor  = pArbCen;
    errMejor = errIndCen;
   }
   // Si hay una rama mejor, puedo podar las partes malas del arbol
   if(elMejor)
   {
    // La mejor rama se despega del arbol y podo las otras
    if(elMejor == pArbIzq)
     pArb->setArbIzq(0);
    else
     if(elMejor == pArbDer)
      pArb->setArbDer(0);
     else
      pArb->setArbCen(0);
    delete pArb;		// Se poda el resto del arbol (sin rama buena)
    arbolGap = elMejor;  	// Y la rama buena se incorpora al individuo
    errorGap = errMejor;	// Y se le da ese error mejor ya calculado
    numNodos = arbolGap->getNumNodos();		// Nuevo numero de nodos
    return VERDAD;				// Hay refinamiento OK !!!
   }
   else
    arbolGap = pArb;	// El raiz era el mejor y asi vuelve a quedar
   return FALSO;	// Nada ha cambiado
  }

  	// Los tipos pormenorizados de mutacion son:

  // Mutaciones de la parte arborea del individuo

  // Intercambia 2 subarboles independientes entre si que formen parte del individuo que llama
  void mutArbIntercambio();
  // Cambia un nodo por otro distinto pero del mismo tipo, de ahi la homogeneidad
  void mutArbNodoHomogenea(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe);
  // Cambia un nodo por otro distinto pero de distinto tipo, por eso es heterogeneo
  void mutArbNodoHeterogenea(Conjunto<ptrNodo> &cVar);
  // Contrae un subarbol a un terminal o expande un terminal a un subarbol
  void mutArbContractiva(Conjunto<ptrNodo> &cVar, Conjunto<ptrNodo> &cOpe);

  // Mutaciones de la parte vectorial del individuo

  void mutVecRotDer();		// Rotacion a la derecha:  	12345 -> 51234
  void mutVecRotIzq();		// Idem hacia la izquierda:	12345 -> 23451            *
  void mutVecCorte();		// Parte aleatoriamente y se combinan ambas mitades: 1234567 -> 4567123
  void mutVecPermutacion();	// Recombina totalmente el vector de ctes: 	     123456 -> 241635
  void mutVecIntercalado();	// Intercambia e intercala elementos:      	     123456 -> 214365
  void mutVecInversion();	// Invierte el vector de ctes:                       123456 -> 654321
  void mutVecSuavizado();	// Suaviza el vector de ctes mediante medias vecinales...
  void mutVecResaltado();	// Agudiza el vector de ctes con medias vecinales ponderadas...
  void mutVecIntercambio();	// Permuta entre si dos elementos escogidos al azar
  void mutVecAleatoria();	// Se cambia totalmente al azar un elemento escogido aleatoriamente
  void mutVecTotalAleatoria();	// Se cambian totalmente al azar todos los elementos de la PV
  void mutVecIncremental();	// Un elemento crece o decrece en un porcentaje
  void mutVecAdicion();		// Lo mismo en una cierta cantidad

  // Para saber si el individuo que hace la llamada es mejor que el dado como parametro
  int  esMejorQue(Individuo *otroIndividuo)
  {
   // En principio solo se tiene en cuenta el error
   if(errorGap < otroIndividuo->errorGap)
    return VERDAD;
   if(errorGap > otroIndividuo->errorGap)
    return FALSO;
   // A igualdad de errores, se da preferencia al que tenga mayor relacion intrones/nodos
   Flt in1 = Flt(numIntrones)/Flt(numNodos);
   Flt in2 = Flt(otroIndividuo->numIntrones)/Flt(otroIndividuo->numNodos);

   if(in1 < in2)
    return VERDAD;
   if(in1 > in2)
    return FALSO;
   // Y finalmente, se da preferencia al que tenga el menor arbol
   if(numNodos < otroIndividuo->numNodos)
    return VERDAD;
   //if(numNodos > otroIndividuo->numNodos)
    return FALSO;
  }

 private:
  Arbol         *arbolGap;	// Parte arborea del individuo Gap
  Conjunto<Flt> *tabCtes;	// Parte vectorial con los valores de las ctes
  int            numNodos;	// Numero de nodos del arbol ya precalculado
  Intron	 numIntrones;	// Numero total de intrones del arbol
  Flt		 errorGap;	// Error propio de este individuo mediante la func.ext.err.




};




typedef Individuo           *ptrIndividuo;
typedef Conjunto<Individuo> *ptrConjIndividuos;

#endif // INDIVIDUO_H