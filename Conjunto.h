/*
  Conjunto.h

  Declaracion de la clase generica Conjunto.
  Para crear un arbol mas o menos aleatorio, debemos tener un conjunto de nodos
  terminales (variables y constantes) y otro de no terminales (que son las
  operaciones), para elegir aleatoriamente en esos conjuntos.

  Por otra parte, de las poblaciones de individuos (arboles + ctes), necesitare
  hacer extracciones tambien aleatorias, e incluso podria llegar a individuos
  formados por subconjuntos de individuos...

  Para todo eso creare la clase de los conjuntos genericos, que utilizan templates,
  justificando este despilfarro por la flexibilidad y la presunta "ingenieria del
  soft"...

  Un conjunto se compone de una serie de punteros a elementos de tipo generico TG.

  Para definir funciones fuera de linea con template seria:
  	
	  template <class TG> conjunto<TG>::conjunto(int n);

  Y para crear un conjunto de punteros a nodos tendria:
  	
	  conjunto<ptrNodo> conjNodo(25);

  Tambien puedo hacer:
 	
	  conjNodo[12] = un_puntero_a_nodo;

*/

#ifndef CONJUNTO_H
#define CONJUNTO_H

#include "Utiles.h"

template <class TG>
class Conjunto
{
		
 public:
	 
	// Constructor de conjunto inicialmente vacio que contendra ese maximo num de elementos
	Conjunto(int maxCardinal)
	{                             				
		numElementos = 0;					// Sin elementos inicialmente
		maxNumElementos = maxCardinal;		// Maximo numero de elementos almacenables
		vector = new TG[maxNumElementos];	// Reserva de espacio
		
		// La inicializacion del espacio con un bucle NO tiene ningun sentido, ya que los
		// objetos a albergar seran de los mas variopintos tipos que imaginarse uno pueda...
	}

	 
	// Constructor copia de un conjunto a partir de otro
	Conjunto(const Conjunto<TG> &cOrig)
	{
		
		numElementos    = cOrig.numElementos;
		maxNumElementos = cOrig.maxNumElementos;
		
		vector = new TG[maxNumElementos];
		for(int n = 0; n < maxNumElementos; n++)
			vector[n] = cOrig.vector[n];
	}
	 
	// Destructor NO vacio: no destruye los elementos apuntados
   ~Conjunto()
	{ 
		if(vector)
		{
			delete [] vector;
			vector = 0;
		}
	}
	 
	// Devuelve numero actual de elementos
	int getCardinal()     	
	{ 
		return numElementos; 
	}
	 
	// Devuelve maximo numero permitido de elementos
	int getMaximoCardinal()	
	{ 
		return maxNumElementos; 
	}
	 
	// Añade un elemento al conjunto
	void agrega(const TG &elem)	
	{
		vector[numElementos] = elem;
		numElementos++;
	}
	 
	// Devuelve elemento escogido aleatoriamente de todos los que forman el conjunto
	TG getElementoAleatorio()	
	{
		return vector[RND(numElementos)];
	}
	 
	// Devuelve el n-simo elemento del conjunto
	TG & operator [] (int n)
	{ 
		return vector[n]; 
	}
	  
	// Para conectar el conjunto a una funcion externa para poder ser ordenado
	void conectaFuncionOrdenacion(int(*nombreFuncion)(const void *a, const void *b))
	{ 
		ptrFuncOrden = nombreFuncion; 
	}
	 
	// El nucleo del proceso de ordenacion: antes de ordenar hay que dar la funcion de orden!!!
	void ordena()
	{ 
		qsort((void *)vector, maxNumElementos, sizeof(vector[0]), ptrFuncOrden); 
	}
	
	
 private:
	 
	// Contenedor de los elementos
	TG *vector;
	 
	// Numero de esos elementos
	int numElementos;		
	 
	// Maximo numero de elementos permitido
	int maxNumElementos;     	
	 
	// Puntero a la funcion EXTERNA que permite la ordenacion de elementos
	int (*ptrFuncOrden) (const void *a, const void *b);

};

//typedef Conjunto<ptrNodo> * ptrConjNodos;

#endif // CONJUNTO_H