/*
  EntradaPila.h

  Los arboles se evaluaran mas rapidamente usando una pila de elementos de este tipo.
  Es la base sobre la que se asientan las pilas Piposeco
*/

#ifndef ENTRADAPILA_H
#define ENTRADAPILA_H

#include "Utiles.h"

// La pila se compone de una serie de entradas
struct EntradaPila		
{
	// El tipo del nodo (k, v, o1, o2, o3)
	TipoNodo tipoNodo;	
	
	// Numero de parametros requeridos para la operacion (0, 1, 2)
	int aridad;           
 
	// Valor de k, de v o de la evaluacion ya hecha de una operacion
	Flt *valorNodo;		
 
	// Posicion del valor del segundo parametro de una operacion
	int param2;			
 
	// Idem con el tercer parametro (solo operaciones ternarias)
	int param3;

	// El primer parametro esta en la siguiente entrada...
};				

#endif // ENTRADAPILA_H