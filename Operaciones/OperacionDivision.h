/**
  Clase OperacionDivision
  Implementacion de la operacion division.
*/


#ifndef OPERACIONDIVISION_H
#define OPERACIONDIVISION_H


#include "Operacion.h"


class OperacionDivision : public Operacion
{

 public:

	OperacionDivision()
	{ 
		cadOp = DIVISION; 
	}

	Flt evalua(Flt *a, Flt *b, Flt *c)
	{						// Como siempre, la division por 0 es problematica,
		if(esCero(*b))		// por lo cual se usa una division "protegida" que
			return 0.;   	// en situaciones del tipo .../0 devuelve siempre 0
		return *a / *b;
	}

	std::ostream & operator << (std::ostream &salida)
	{ 
		return salida << "/"; 
	}

};

#endif // OPERACIONDIVISION_H