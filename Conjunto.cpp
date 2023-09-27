/*
  Conjunto.cpp

  Implementacion de algunas fmc de la clase generica Conjunto.
*/

#include "Conjunto.h"

// Constructor de conjunto inicialmente vacio que contendra ese maximo num de elementos
template <class TG>
Conjunto<TG>::Conjunto(int maxCardinal)
{                             		
	// No es on-line por el for
 
	numElementos = 0;					// Sin elementos inicialmente
	maxNumElementos = maxCardinal;		// Maximo numero de elementos almacenables
	vector = new TG[maxNumElementos];	// Reserva de espacio
 
	// La inicializacion del espacio con un bucle NO tiene ningun sentido, ya que los
	// objetos a albergar seran de los mas variopintos tipos que imaginarse uno pueda...
}

// Constructor copia de un conjunto a partir de otro
template <class TG>
Conjunto<TG>::Conjunto(const Conjunto<TG> &cOrig)
{
 
	numElementos    = cOrig.numElementos;
	maxNumElementos = cOrig.maxNumElementos;
	
	vector = new TG[maxNumElementos];
	for(int n = 0; n < maxNumElementos; n++)
		vector[n] = cOrig.vector[n];
}