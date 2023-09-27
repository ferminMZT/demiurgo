/**
  Clase OperacionResta
  Implementacion de la operacion resta.
*/


#ifndef OPERACIONRESTA_H
#define OPERACIONRESTA_H


#include "Operacion.h"


class OperacionResta : public Operacion
{

 public:

	OperacionResta()
	{ cadOp = RESTA; }
  
	Flt evalua(Flt *a, Flt *b, Flt *c)
	{ return *a - *b; }
  
	ostream & operator << (ostream &salida)
	{ return salida << "-"; }
};

#endif // OPERACIONRESTA_H