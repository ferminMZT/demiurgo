/*
  Piposeco.h
  
  Clase de la PIla POlaca SEmi COnstante (PIPOSECO) usada para evaluar de manera
  rapida (afirmo!!!) un arbol sin usar la recursividad. Se crea a partir de un
  arbol, con tantas entradas como nodos tenga este...
*/

#ifndef PIPOSECO_H
#define PIPOSECO_H

#include "Arbol.h"
#include "EntradaPila.h"
#include "Utiles.h"

class Piposeco
{

 public:
	
	// Constructor a partir de un arbol con un numero de nodos dado
	Piposeco(Arbol *pArb, int numeroNodos)
	{
		// Lo primero es asignar tantas entradas como nodos tenga el arbol
		numEntradas = numeroNodos;
		entradas    = new EntradaPila[numEntradas];
		// Y ahora habria que recorrer el arbol en PRE-ORDEN asignando entradas de la pila
		// crecientemente desde 0 (con el nodo raiz) hasta numEntradas (con el ultimo)
		pArb->apilarPreOrden(entradas, numEntradas);
		// verSimplificado();
		// std::cout << std::endl;
		// Y tras tan bonita construccion, que cosa mejor que simplificarla!!!
		simplifica();
	}

	// Constructor copia
	Piposeco(Piposeco &pPil)	
	{ 
		;
	}

	// Destructor
	~Piposeco();
  
	// Estas 2 funciones son de importancia extrema!!!

	// Simplifica los terminos constantes
	void simplifica();	

	// Evaluador de la pila PIPOSECO
	Flt evalua();			
  
	// Para ver un PIPOSECO simplificado a partir de la posicion dada (es recursiva!!!)
	void verSimplificado(int nPipo = 0);


 private:

	// Cada entrada de la pila
	EntradaPila *entradas;

	// El numero de entradas de la pila
	int numEntradas;	

};

#endif // PIPOSECO_H