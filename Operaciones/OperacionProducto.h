/**
  Clase OperacionProducto
  Implementacion de la operacion producto.
*/


#ifndef OPERACIONPRODUCTO_H
#define OPERACIONPRODUCTO_H


#include "Operacion.h"


class OperacionProducto : public Operacion
{

 public:

	OperacionProducto()
	{ cadOp = PRODUCTO; }

	Flt evalua(Flt *a, Flt *b, Flt *c)
	{ return *a * *b; }

	ostream & operator << (ostream &salida)
	{ return salida << "*"; }

};

#endif // OPERACIONPRODUCTO_H