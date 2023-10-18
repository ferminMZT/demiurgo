/* 
  \file Abalones.h
  Problema 10.
  Tenemos 4177 ejemplos de 8 atributos sobre abalones para sacar como clasificacion
  la edad (de 1 a 29). Prefiero tratarlo como una regresion simbolica que produzca
  la edad y medir el error...
*/

#ifndef ABALONES_H
#define ABALONES_H

#include "Problema.h"
#include "../Bateria.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Abalones : public Problema
{

 public:

	 Abalones()
	 {
		 nombre       = "Abalones: La edad de las orejas de mar";
		 batAbalones  = new Bateria("abalones.dat");
		 numEjemplos  = batAbalones->getNumEjemplos();
		 numAtributos = batAbalones->getNumAtributos();
	 }

	 ~Abalones()
	 {
		 delete batAbalones;
		 numEjemplos = numAtributos = 0;
		 pilaEvaluacion = 0;
	 }

	 Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);

	 Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
	 { return 0.0; }

	 Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
	 { return 0.0; }
	 

 private:

	 Bateria  *batAbalones;	// Bateria de ejemplos a clasificar
	 int       numEjemplos;      	// Numero de ejemplos de la bateria
	 int       numAtributos;	// Numero de atributos de cada ejemplo
	 Variable *var;
	 Flt       errTodasMuestras;
	 int       ne, na;
	 Flt       valEvaluado, valClase;
	 Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion


};

#endif // ABALONES_H