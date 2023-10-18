/**
   Carro.h
   Regresion simbolica para el control de un carro.
*/

#ifndef CARRO_H
#define CARRO_H

#include <math.h>

#include "../Piposeco.h"
#include "../Variable.h"
#include "Problema.h"

class Carro : public Problema
{
 
 public:

	Carro()
	{
		nombre = "Movimiento de carro";
		nPtos = 200;
		a     = -10.;
		b     = +10.;
		inc   = (b - a) / nPtos;
	}
		
 ~Carro()
	{
		nombre = 0;
		nPtos          = 0;
		pilaEvaluacion = 0;
	}
  
	Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
	{ 
		return .0; 
	}

	Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest);


 private:

	// Numero de puntos para x, y, z en [-5.12, +5.12]
	int nPtos; 		

	// Por otra parte, pongo aqui unas variables auxiliares para el calculo del
	// fitness para no tener que crearlas y ganar una poca velocidad mas... ?
	Variable *X;

	Flt x;

	Flt errTodasMuestras;

	Flt a, b;

	Flt inc;

	int n;

	Flt valEvaluado;

	// Pila piposeco para la evaluacion
	Piposeco *pilaEvaluacion;	

};

#endif // CARRO_H