/**
  Clase OperacionCoseno
  Implementacion de la operacion coseno.
*/


#ifndef OPERACIONCOSENO_H
#define OPERACIONCOSENO_H


#include "Operacion.h"
#include <math.h>

class OperacionCoseno : public Operacion
{

 public:

	OperacionCoseno()
	{ 
		cadOp = COSENO; 
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
		return (Flt) cos(*a); 
	}
  
	std::ostream & operator << (std::ostream &salida)
	{ 
		return salida << "cos"; 
	}

};

#endif // OPERACIONCOSENO_H