/*
  	\file Problema.h

  	Problema es la clase base abstracta (aka ABC) de la que derivare virtualmente todos los subproblemas concretos
	a los que me enfrentare...
	Algunos problemas son directamente calculables por codigo, mientras que otros requieren cargar ciertas baterias de
	datos que contienen casos con atributos. Esas baterias estan sacadas y adaptadas de:

	Los problemas de clasificación han sido extraidos del UCI Machine Learning Repository:
	http://www.ics.uci.edu/AI/ML/MLDBRepository.html
	en donde se pueden encontrar bases de datos, ficheros de texto, teorías de dominio y generadores de datos que son
	ampliamente utilizados por la comunidad científica para analizar empíricamente diversos algoritmos de aprendizaje.
	Esa página está mantenida por C.J. Merz
	http://www.ics.uci.edu/~cmerz/)
	del Departamento de Información y Ciencias de la Computación de la Universidad de California.
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
	 virtual const char *getNombre()
	 {
		 return nombre;
	 }
	 
	 // Comprueba un individuo solucion sometiendolo a una bateria de pruebas en el fichero dado.
	 // Produce un fichero .sal con los resultados de la comparacion y devuelve el % de error.
	 virtual Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest) = 0;

 protected:
  
	// Nombre del problema a resolver
	const char *nombre;

};

typedef Problema *ptrProblema;

#endif // PROBLEMA_H