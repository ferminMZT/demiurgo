/**
  Clase OperacionCondicional
  Implementacion de la operacion condicional de C: var = (cond) ? (exp1) : (exp2);
*/


#ifndef OPERACIONCONDICIONAL_H
#define OPERACIONCONDICIONAL_H


#include "Operacion.h"


class OperacionCondicional : public Operacion
{

 public:
	  
	OperacionCondicional()
	{
		cadOp = CONDICIONAL; 
	}
  
	// Ojo: Un grave bug encontrado: Una operacion SOLO puede tener un tipo!!!
	int esBinario() const
	{ 
		return FALSO; 
	}
  
	int esTernario() const
	{ 
		return VERDAD; 
	}
  
	// Ojo, la condicion es la rama central, el entonces va primero y el sino ultimo
	Flt evalua(Flt *condicion, Flt *sino, Flt *entonces)
	{
		if(*condicion > .0)
			return *entonces;
		return *sino;
	}

	ostream & operator << (ostream &salida)
	{ 
		return salida << "?"; 
	}

};

#endif // OPERACIONCONDICIONAL_H