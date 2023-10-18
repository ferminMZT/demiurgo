/**
 * \file Espiral.h
 * Problema 1
 * El conocido problema de la clasificacion de las espirales...
*/

#ifndef ESPIRAL_H
#define ESPIRAL_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Espiral : public Problema
{

 public:

	Espiral();
   ~Espiral()
	{
		delete [] valX;
		delete [] valY;
		delete [] valClas;
		nombre = 0;
		numPuntos      = 0;
		pilaEvaluacion = 0;
	}

	Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables);
	
	Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
	{ 
		return 0.0; 
	}


 private:

	// Numero de puntos a clasificar, que son 97 * 2
	int numPuntos;

	// Cada punto tiene una componente X, una Y
	Flt *valX;			
	
	// y un valor obtenido tras clasificarlo...
	Flt *valY;
	
	Flt *valClas;

	// Por otra parte, pongo aqui unas variables auxiliares para el calculo del
	// fitness para no tener que crearlas y ganar una poca velocidad mas... ?
	Variable *X, *Y;

	Flt errTodasMuestras;

	int n;

	Flt valEvaluado;

	// Pila piposeco para la evaluacion
	Piposeco *pilaEvaluacion;	

};

#endif // ESPIRAL_H