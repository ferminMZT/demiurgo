/*
  Problema.h

  Problema es la clase base abstracta de la que derivare virtualmente todos los
  subproblemas concretos a los que me enfrentare...
*/

#ifndef PROBLEMA_H
#define PROBLEMA_H

#include "../Utiles.h"
#include "../Individuo.h"

class Problema
{

 public:

	 virtual ~Problema()
	 { ; }

	 // Da el fitness de un individuo
	 virtual Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables) = 0;
	 
	 // Devuelve una cierta distancia entre 2 individuos calculada en cierta forma usando el fitness
	 virtual Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables) = 0;
	 
	 // Para obtener el nombre del problema tratado
	 virtual char *getNombre()
	 {
		 return nombre;
	 }
	 
	 // Comprueba un individuo solucion sometiendolo a una bateria de pruebas en el fichero dado.
	 // Produce un fichero .sal con los resultados de la comparacion y devuelve el % de error.
	 virtual Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest) = 0;

 private:
  
	char *nombre;	// Nombre del problema a resolver

};

typedef Problema * ptrProblema;

#endif // PROBLEMA_H