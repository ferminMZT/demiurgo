/**
  Poblacion.h
  Declaracion de la clase Poblacion. 
  Clase creada para facilitar enormemente el trabajo a los futuros usuarios.
  Reune las principales operaciones demiurgicas, tales como la creacion de los
  individuos y las distintas alternativas de evolucion...
*/


#ifndef POBLACION_H
#define POBLACION_H


#include <fstream>


#include "Conjunto.h"
#include "Individuo.h"
#include "Problemas/Problema.h"



class Poblacion
{

 public:

	/// Constructor SOLO necesita los conjuntos de nodos de los futuros arboles: variables y operaciones
	Poblacion(Conjunto<ptrNodo> *cVar, Conjunto<ptrNodo> *cOpr)
	{
		// Ajuste de conjuntos y cardinales
		conjVariables   = cVar;
		numVariables    = conjVariables->getCardinal();
		conjOperaciones = cOpr;
		numOperaciones  = conjOperaciones->getCardinal();
		numIndividuos   =  0;		// Todavia no se crea ningun individuos: no se su numero
		posMejor        = -1;		// ni hay uno mejor
		//ptrFuncFitness =  0;		// No hay funcion externa conectada aun
		errorAceptable   = (Flt) EPSILON;	// Por defecto se busca un error de 0
		errorMedioPoblacion = .0;   // ni tienen error
		numGeneraciones  = 0;
		elProblema       = 0;
		semilla          = 0;		// Por defecto NO hay semilla aleatoria conocida
		tpcMutacion      = TPC_MUTACION;
		numMedioNodos    = .0;
		numMedioIntrones = .0;
	}

	/// Destructor
	~Poblacion();

	/// Crea poblacion inicial de individuos aleatorios usando la estrategia de supra-enriquecimiento inicial
	void creaPoblacionInicial(int nInd = TAM_POBLACION, int fEnr = FACT_ENRIQ, int nElite = ELITE_ENRIQ);

	/// Para asignar un problema que sera resuelto por la poblacion (o al menos lo intentaran...)
	void asignaProblema(ptrProblema unProblema)
	{ 
		elProblema = unProblema; 
	}

	/// Para cambiar en RT el % de individuos que sufriran la mutacion
	void asignaMutacion(int otraMutacion)
	{ 
		tpcMutacion = otraMutacion; 
	}

	/// Para saber el % de mutacion aplicada
	int getMutacion()
	{ 
		return tpcMutacion; 
	}

	/// Para asignar una semilla que podra permitir repetir EXACTAMENTE el mismo experimento
	void asignaSemilla(time_t unaSemilla)
	{ 
		semilla = unaSemilla; 
	}

	/** 
	  Para conectar una funcion externa a la poblacion para evaluar individualmente el
	  error de cada miembro: Si funciona sera la hostia de flexible!!!
    */
	/*
	void conectaFuncionError(Flt(*nombreFuncion)(ptrIndividuo, ptrConjNodos))
	{ 
		ptrFuncFitness = nombreFuncion; 
	}
	*/

  /// Para saber el numero de miembros de los que consta la poblacion
  int getNumIndividuos()
  { return numIndividuos; }

  /// Calcula el error del individuo dado, almacenandolo en el y devolviendolo
  Flt calculaErrorIndividual(Individuo *pInd)
  {
   /// Flt errInd = ptrFuncFitness(pInd, conjVariables);
   Flt errInd = elProblema->fitness(pInd, conjVariables);

   pInd->setError(errInd);
   return errInd;
  }

  /// Devuelve el error ya calculado para ese individuo
  Flt getError(int nInd)
  { return (*conjIndividuos)[nInd]->getError(); }

  /** 
    Actualiza los parametros globales de la poblacion (errMedioPoblacion, numMedioNodos y numMedioIntrones) a partir
    los correspondientes parametros (ya calculados) de cada individuo.
  */
  void actualizaParametros();

  // Devuelve el error medio de la poblacion
  Flt getErrorMedioPoblacion()
  { return errorMedioPoblacion; }

  // Devuelve el error aceptable que se pretende alcanzar en la evolucion
  Flt getErrorAceptable()
  { return errorAceptable; }

  // Fija el error aceptable que se pretende alcanzar
  void setErrorAceptable(Flt errorUsuario)
  { errorAceptable = errorUsuario; }

  // Devuelve la posicion del mejor individuo
  int getPosMejor()
  { return posMejor; }

  // Devuelve las generaciones vividas hasta el momento por la poblacion
  int getNumGeneraciones()
  { return numGeneraciones; }

  // Devuelve un ptr al mejor individuo
  ptrIndividuo getMejorIndividuo()
  { return (*conjIndividuos)[posMejor]; }

  // Devuelve el mejor error, que es el del mejor individuo
  Flt getMejorError()
  { return (*conjIndividuos)[posMejor]->getError(); }

  // Devuelve un ptr al n-simo miembro de la poblacion
  ptrIndividuo & operator[](int n)
  { return (*conjIndividuos)[n]; }

  // Informa de si el error del mejor individuo cumple con el propuesto (aceptable)
  int errorAlcanzado()
  { return (errorAceptable >= (*conjIndividuos)[posMejor]->getError()) ? VERDAD : FALSO; }

  // Para ajustar metodo de generacion (solo es cosmetica)
  void setMetodoGeneracion(int metodo)
  { metodoGeneracion = metodo; }

  // Para saber el numero medio de nodos de los individuos de la poblacion
  Flt getNumMedioNodos()
  { return numMedioNodos; }

  // Para saber el numero medio de intrones de los individuos de la poblacion
  Flt getNumMedioIntrones()
  { return numMedioIntrones; }

  // Para obtener el mejor individuo de la poblacion dada como parametro y colocarlo como el mejor de la que llama
  void asignaMejorIndividuo(Poblacion &otraPoblacion)
  {
   delete (*conjIndividuos)[posMejor];		// Suprimo al mejor
   // Lo clono a partir del mejor de la otra poblacion
   (*conjIndividuos)[posMejor] = new Individuo(*(otraPoblacion.getMejorIndividuo()));
   //(*conjIndividuos)[posMejor]->asignaPila();	// Le doy una nueva pila (ya que no lo hace el constructor copia)
  }

  // Para intercambiar entre si a los mejores individuos de 2 poblaciones
  void intercambiaMejorIndividuo(Poblacion &otraPoblacion)
  {
   ptrIndividuo iMejor       = (*conjIndividuos)[posMejor];
   ptrIndividuo otroIMejor   = otraPoblacion.getMejorIndividuo();

   intercambia(iMejor, otroIMejor);
  }

  // NUCLEO DEL SISTEMA: REALIZA UNA GENERACION COMPLETA
  // Las versiones acabadas en I usan los intrones

  // Modo steady-state democratico con un solo hijo
  int generacionA();
  int generacionAI();
  // Modo steady-state democratico con hijo e hija
  int generacionB();
  int generacionBI();
  // Modo steady-state pseudo-democratico con hijo e hija
  int generacionC();
  // Modo familia numerosa (en vez de crear 1 o 2 hijos por pareja se crean nHijos)
  int generacionD(int nHijos);
  // Modo de eleccion de la pareja mas complementaria entre el numero de novias dado
  int generacionE(int nNovias);

  // Refinar los miembros de una poblacion, pudiendo mejorar al mejor!!!
  void refinaPoblacion();
  // Metodo para intentar recuperar la diversidad genetica de la poblacion: Algoritmo de Karl Wahall
  void nemesis(int tpcElite);
  // Tras acabar un entrenamiento, escribe los 2 ficheros de salida
  void escribeResultados(void (*ptrFuncEscritura)(ptrIndividuo, ptrConjNodos, fstream &), int t = 0);


 private:
  // Los individuos que creare se compondran de elementos de los conjuntos de variables y operaciones
  // Los vectores de constantes seran cosa particular de cada individuo...
  Conjunto<ptrNodo> *conjVariables;
  Conjunto<ptrNodo> *conjOperaciones;
  // Estos son los cardinales respectivos de esos conjuntos:
  int numVariables;
  int numOperaciones;
  // La poblacion es un conjunto de (ptr a) individuos de cardinal numIndividuos:
  Conjunto<ptrIndividuo> *conjIndividuos;
  int numIndividuos;            // Tamaño de la poblacion
  int factorEnriq;		// Factor de enriquecimiento de la poblacion inicial
  int eliteEnriq;		// Tamaño de la elite enriquecida de la poblacion inicial
  int numGeneraciones;		// Numero de generaciones vividas por la poblacion
  int posMejor;			// Posicion en la poblacion del mejor miembro
  Flt errorMedioPoblacion;      // Error medio total de la poblacion, o sea de la generacion
  Flt errorAceptable;		// Error buscado (por defecto 0); al obtenerlo se acaba la evolucion
  int metodoGeneracion;		// Para saber el metodo usado de entrenamiento en un fichero de salida
  int tpcMutacion;		// Porcentaje de mutacion aplicado a la poblacion
  // LA GRAN PREGUNTA: COMO INTEGRO MUCHAS POSIBLES FORMAS DE ENTRADA DE DATOS???
  // Es decir, una vez que he creado un individuo, como lo enfrento al conjunto de datos
  // reales para calcular el error cometido? Por ejemplo a una matriz 2D de flotantes...
  // LA GRAN RESPUESTA: CON UN PUNTERO A UNA FUNCION QUE DEVUELVE FLOTANTE
  // EL INCONVENIENTE: Y SUS PARAMETROS?
  // RESPUESTA: Con el operador ... no hace falta saberlos!!!
  //Flt (*ptrFuncion)(...);
  // Probare esta funcion con 2 parametros: un ptr al individuo a evaluar y un ptr al conj de variables
  //Flt (*ptrFuncFitness)(ptrIndividuo, ptrConjNodos);
  //----------------------------------------------------------------------------
  // La clase problema sirve para conectar la poblacion con la tareas que debe resolver
  ptrProblema elProblema;
  // Almacenando la semilla aleatoria del GNA sera posible repetir experimentos sin aleatoriedad
  time_t      semilla;
  Flt numMedioNodos;		// Numero medio de nodos de la poblacion
  Flt numMedioIntrones;		// Numero medio de intrones de la poblacion
  //----------------------------------------------------------------------------

};

typedef Poblacion              * ptrPoblacion;
typedef Conjunto<ptrPoblacion> * ptrConjPoblaciones;

#endif // POBLACION_H
