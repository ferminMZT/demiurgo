/*
  \file Bupa.h

  Problema 9
  Tenemos 345 ejemplos de 6 atributos (5 de analisis de sangre y 1 de consumo
  alcoholico) para clasificar en 2 clases de riesgo
*/

#ifndef BUPA_H
#define BUPA_H

#include "Problema.h"
#include "../Bateria.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Bupa : public Problema
{

 public:

	Bupa()
	{
		nombre       = "BUPA: El riesgo de alcoholemia";
		batBupa      = new Bateria("bupa.dat");
		numEjemplos  = batBupa->getNumEjemplos();
		numAtributos = batBupa->getNumAtributos();
	}

   ~Bupa()
	{
	   delete batBupa;
	   numEjemplos = numAtributos = 0;
	   pilaEvaluacion = 0;
	}
  
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

	// Bateria de ejemplos a clasificar
	Bateria *batBupa;

	// Numero de ejemplos de la bateria
	int numEjemplos;      	

	// Numero de atributos de cada ejemplo
	int numAtributos;	
  
	Variable *var;
  
	Flt errTodasMuestras;
  
	int ne, na;
  
	Flt valEvaluado, valClase;
  
	// Pila piposeco para la evaluacion
	Piposeco *pilaEvaluacion;	

};

#endif // BUPA_H