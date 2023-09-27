/*
  SDI000.h
  Primer problema planteado con los datos del SDI: Establecer una correlacion y = f(x)
  donde y es la posicion del carro y x la entrada del joystick...
*/

#ifndef SDI000_H
#define SDI000_H

#include <vector>
#include <string>

#include "../Piposeco.h"
#include "../Variable.h"

#include "Problema.h"

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

	char *getNombre()
	{ 
		return (char *) nombre.c_str(); 
	}

	Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables);
	
	Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest);


 private:

	/// Nombre del problema.
	 std::string nombre;

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