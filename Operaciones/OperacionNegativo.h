/**
  Clase OperacionNegativo
  Implementacion de la operacion cambio de signo.
*/


#ifndef OPERACIONNEGATIVO_H
#define OPERACIONNEGATIVO_H


#include "Operacion.h"


class OperacionNegativo : public Operacion
{

 public:

	OperacionNegativo()
	{ 
		cadOp = NEGATIVO; 
	}
  
	int esUnario() const
	{ 
		return VERDAD; 
	}
  
	int esBinario() const
	{ 
		return FALSO; 
	}
  
	Flt evalua(Flt *a, Flt *b, Flt *c)
	{ 
		return - (*a); 
	}
  
	ostream & operator << (ostream &salida)
	{ 
		return salida << "-"; 
	}
};

#endif // OPERACIONNEGATIVO_H