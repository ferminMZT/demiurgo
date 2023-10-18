/**
  Clase OperacionCuadrado
  Implementacion de la operacion cuadrado.
*/


#ifndef OPERACIONCUADRADO_H
#define OPERACIONCUADRADO_H


#include "Operacion.h"


class OperacionCuadrado : public Operacion
{

 public:

	OperacionCuadrado()
	{ 
		cadOp = CUADRADO; 
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
		return *a * *a; 
	}
  
	std::ostream & operator << (std::ostream &salida)
	{ 
		return salida << "pow"; 
	}

};

#endif // OPERACIONCUADRADO_H
