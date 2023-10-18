/**
  Clase OperacionSeno
  Implementacion de la operacion seno.
*/


#ifndef OPERACIONSENO_H
#define OPERACIONSENO_H


#include "Operacion.h"
#include <math.h>


class OperacionSeno : public Operacion
{

 public:
  
	OperacionSeno()
	{ 
		cadOp = SENO; 
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
		return (Flt) sin(*a); 
	}
  
	std::ostream & operator << (std::ostream &salida)
	{ 
		return salida << "sin"; 
	}

};

#endif // OPERACIONSENO_H