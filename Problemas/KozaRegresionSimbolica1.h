/*
  KozaRegresionSimbolica1.h
  Problema 0
  La regresion simbolica de x^4 + x^3 + x^2 + x con 20 valores escogidos al azar
  en [-1, +1]. Planteado en [Koza92] apartado 7.3, pag 162
*/

#ifndef KOZAREGRESIONSIMBOLICA1_H
#define KOZAREGRESIONSIMBOLICA1_H

#include <math.h>

#include "../Piposeco.h"
#include "../Variable.h"
#include "Problema.h"

class KozaRegresionSimbolica1 : public Problema
{
 
 public:

	KozaRegresionSimbolica1()
	{
		nombre = "La regresion simbolica x^4 + x^3 + x^2 + x con x en [-1., +1.]";
		nPtos = 200;
		a     = -10.;
		b     = +10.;
		inc   = (b - a) / nPtos;
	}
		
 ~KozaRegresionSimbolica1()
	{
		nombre = 0;
		nPtos          = 0;
		pilaEvaluacion = 0;
	}
  
	char *getNombre()
	{ 
		return nombre; 
	}

	Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
	{ 
		return .0; 
	}

	Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest);


 private:

	// Nombre del problema a resolver
	char *nombre;	
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

#endif // KOZAREGRESIONSIMBOLICA1_H