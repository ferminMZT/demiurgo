/*
  SDI000.h
  Primer problema planteado con los datos del SDI: Establecer una correlacion y = f(x)
  donde y es la posicion del carro y x la entrada del joystick...
*/

#ifndef SDI000_H
#define SDI000_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"

#include <vector>

Flt fG(Flt X);

class SDI000 : public Problema
{

 public:

	SDI000();
   ~SDI000()
	{
		nombre  = "";
		numPtos = 0;
		if(pilaEvaluacion)
		{	
			delete pilaEvaluacion;
			pilaEvaluacion = 0;
		}

	}

	Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables);
	
	Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest);


 private:

	/// Numero de puntos procesados en el dominio.
	int numPtos;

	/// Valores de las variables x (entrada) e y (salida).
	std::vector<Flt> x, y;

	// Por otra parte, pongo aqui unas variables auxiliares para el calculo del
	// fitness para no tener que crearlas y ganar una poca velocidad mas... ?
	Variable *X;

	Flt errTodasMuestras;

	int n;

	Flt valEvaluado;

	// Pila piposeco para la evaluacion
	Piposeco *pilaEvaluacion;	

};

#endif // SDI000_H