/**
  Sombrero.h
  Problema ???
  Regresion simbolica de la funcion 3D f(x, y) = sin(x^2 + y^2)/(x^2 + y^2)
  con 20*20 puntos equiespaciados en el entorno [-3, +3]
*/


#ifndef SOMBRERO_H
#define SOMBRERO_H


#include <math.h>


#include "Problema.h"
#include "../Piposeco.h"
#include "../Variable.h"


class Sombrero : public Problema
{

 public:

	Sombrero();

	~Sombrero();

	char *getNombre()
	{ 
		return nombre; 
	}
  
	Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
	{ 
		return 0.0; 
	}

	Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest)
	{ 
		return 0.0; 
	}


 private:
  
	// Nombre del problema a resolver
	char *nombre;		
	
	// Indices de la funcion
	int x, y;

	// Tamaño del lado de la matriz cuadrada de valores
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
