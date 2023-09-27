/*
  Utiles.h

  Esto es el cajon de sastre con todas las definiciones, macros, etc que necesita
  el inexistente caballero Demiurgo para su lucha contra las hordas del caos.
*/

#ifndef UTILES_H
#define UTILES_H

#include <windows.h>	// MessageBox
#include <stdlib.h>		// rand, srand
#include <iostream>		// ostream
#include <io.h>			// _access
#include <float.h>		// FLT_MAX

	  //
	 // Unos utiles typedef
	//

// El tipo de numeros en coma flotante empleados
typedef float Flt;

// Esto ye muy util oh!!!
typedef unsigned char Byte;

// Para que los numeros de los intrones desperdicien la menor cantidad de memoria
typedef unsigned short Intron;

  //
 // Unas macros
//

#define DEBUG(mensaje) 	::MessageBox(0, mensaje, "_Debug_", MB_OK)
#define SRND(numero)	srand(numero)
#define RND(numero)		(rand() % numero)
//#define RND(numero)		( (int) ( ( ( rand()/(RAND_MAX + 1.0) ) * (numero) ) ) )
#define COC()			(rand() & 1)	// Cara o cruz, 0 o 1
//#define MITAD 			RAND_MAX / 2
//#define COC()			rand() < MITAD	// Cara o cruz, 0 o 1
#define DIRMEMORIA		int		// Tipo para acceder a una posicion de memoria
#define MAQUINA			"PC"
#define M_PI			3.14159265358979323846

#define VERDAD		 1
#define FALSO		 0
#define EPSILON		 0.0000001	// Una 10-millonesima tiende a 0
#define INFINITO	FLT_MAX		// Mayor valor imposible de manejar
#define TAM_POBLACION  20000		// Tamaño por defecto de la poblacion
#define FACT_ENRIQ      1		// Factor para el tamaño de la poblacion enriquecida
#define ELITE_ENRIQ     1000		// Tamaño de la elite para supra-enriquecimiento incremental
#define FACT_INCREM      0.1 //0.01	// Porcentaje para in/de cremento de las ctes
#define CANT_INCREM      0.1 //0.01	// Cantidad para in/de cremento de las ctes
#define TAM_TABLA_GA   10		// Numero de constantes en un individuo GAP
#define NUM_MAX_NODOS 150	//150	// Maximo numero de nodos admisible en un arbol GAP
#define NUM_MAX_GENER 100 		// Maximo numero de generaciones alcanzable en la evolucion
#define PROF_INICIAL    10		// Profundidad maxima de arboles aleatorios de poblacion inicial
// Metodos de generacion
#define GENERACION_A     1		// Democratico, hijo
#define GENERACION_B     2		// Democratico, hijo-hija
#define GENERACION_C	 3		// Democratico falso, hijo-hija
#define GENERACION_D	 4		// Democratico con prole numerosa (n hijos)
#define GENERACION_E     5		// Democratico con eleccion de novia complementaria (entre varias)
#define GENERACION_BI    12		// Democratico, hijo-hija con intrones

#define TPC_MUTACION	 1		// Porcentaje de las mutaciones
// Metodos de mutacion de la parte VECTORIAL
#define MUT_PV_ROT_DER   0		// Mutacion de parte vectorial por rotacion a la derecha
#define MUT_PV_ROT_IZQ   1		// Mut. PV por rotacion a la izquierda
#define MUT_PV_INTERCAM  2		// Idem por intercambio de componentes individuales
#define MUT_PV_CORTE	 3		// Intercambio de las 2 secciones
#define MUT_PV_INVERS	 4		// Inversion de la PV
#define MUT_PV_SUAVE	 5		// Difuminado de la PV
#define MUT_PV_RESALTE   6		// Resalte de la PV
#define MUT_PV_CALAR	 7		// Intercambio de los elementos sucesivos
#define MUT_PV_INCREM	 8		// Mutacion individual gradientina
#define MUT_PV_PERMUT    9		// Mutacion con permutacion total de todos los elementos
#define MUT_PV_ALEAT    10		// Mutacion totalmente aleatoria de un elemento
#define MUT_PV_TOTALEAT 11		// Mutacion totalmente aleatoria de todos los elementos
#define MUT_PV_ADICION	12		// Mutacion individual con suma/resta de una cantidad
#define NUM_MUT_PV	13		// Numero disponible de mutaciones de la PV

// Metodos de mutacion de la parte ARBOREA
#define MUT_PA_HOMOGEN   0		// Mutacion de la parte arborea por cambio homogeneo de nodos
#define MUT_PA_HETEROG   1		// Idem por cambio heterogeneo
#define MUT_PA_INTERCAM  2		// Intercambio de subarboles internos
#define MUT_PA_CONTRACT  3		// Por contraccion de un subarbol/expansion arborea de un nodo
#define NUM_MUT_PA	 4		// Numero disponible de mutaciones de la PA

	// Definiciones de macros: siempre son mas rapidas que evaluar funciones!

#define minimo(x, y)		(((x) < (y)) ? (x) : (y))
#define maximo(x, y)		(((x) > (y)) ? (x) : (y))
#define absoluto(x)		(((x) >= .0) ? (x) : (-x))
#define esCero(x)		(((x) < EPSILON && (x) > -EPSILON) ? VERDAD : FALSO)
//#define esUno(x)		(esCero(1. - (x)) && esCero(1. - (x)))
//#define iguales(x, y)		((absoluto((x)-(y)) <= EPSILON) ? VERDAD : FALSO)

	// Prototipos de alguna funcion de uso cosmopolita

template <class TG> void intercambia(TG &a, TG &b)      // Intercambia entre si lo que sea
{   							// de Tipo Generico TG
 TG temporal = a;					// Si las templates son lentas, pero
							// comodas, debere hacer una funcion
 a = b;                                                 // para cada caso concreto de los
 b = temporal;						// que quiera intercambiar
}

extern Flt   f(const Flt &x); 
int   fExtOrdenInd(const void *a, const void *b); 	// Para ordenar los individuos
char *generaNombre(char *ptrCad);			// Generador de nombre correlativos de ficheros

// Defino el conjunto de nodos (varias operaciones, constante o variable) que utilizo
// Ojo: EL orden es fundamental, ya que la funcion pila::evalua utiliza este preciso
// orden para el tratamiento de su CASE interno...
enum TipoNodo 
{
	SUMA = 0, 
	RESTA, 
	PRODUCTO, 
	DIVISION, 
	NEGATIVO,
	CONDICIONAL, 
	SENO, 
	COSENO, 
	CUADRADO, 
	CONSTANTE, 
	VARIABLE, 
	// PUNTOCOMAASIGNACION 
	ASIGNACIONCOMA
};






void Error(char *mensajeError, char *parametroAdicional = 0);



// Para simplificar las funciones y las templates

#endif // UTILES_H