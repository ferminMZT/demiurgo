/**
  Clase OperacionSuma
  Implementacion de la operacion suma.
*/


#ifndef OPERACIONSUMA_H
#define OPERACIONSUMA_H


#include "Operacion.h"


class OperacionSuma : public Operacion
{

 public:

	OperacionSuma()
	{ 
		cadOp = SUMA; 
	}
	
	Flt evalua(Flt *a, Flt *b, Flt *c)
	{ 
		return *a + *b; 
	}
  
	ostream & operator << (ostream &salida)
	{ 
		return salida << "+"; 
	}

};

#endif // OPERACIONSUMA_H