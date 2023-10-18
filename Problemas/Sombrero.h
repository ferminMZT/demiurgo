/**
 * \file Sombrero.h
 * Problema ???
 * Regresion simbolica de la funcion 3D f(x, y) = sin(x^2 + y^2)/(x^2 + y^2)
 * con 20*20 puntos equiespaciados en el entorno [-3, +3].
 */

#ifndef SOMBRERO_H
#define SOMBRERO_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"

#include <math.h>

class Sombrero : public Problema
{

 public:

	Sombrero();

	~Sombrero();
  
	Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
	{ 
		return 0.0; 
	}

	Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
	{ 
		return 0.0; 
	}


 private:
  
	// Indices de la funcion
	int x, y;

	// Tama�o del lado de la matriz cuadrada de valores
	int tam;

	// Variables para cargar individuos
	Variable *X, *Y;		
  
	// Los valores de la evaluacion de la funcion sombrero, datos de entrada
	// (que se usan para calcular el error...)
	Flt **valores;		
  
	// Error acumulado en todos los puntos
	Flt errTotal;

	// Valor tras evaluar un punto
	Flt valEvaluado;

	// Intervalo de uso
	Flt a;
	Flt b;
	Flt inc;
	Flt valX, valY;
	
	// Pila piposeco para la evaluacion
	Piposeco *pilaEvaluacion;

};

#endif // SOMBRERO_H
