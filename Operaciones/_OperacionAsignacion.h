/**
  Clase OperacionAsignacion
  Implementacion de la operacion asignacion de C: 
  variable = expresion;
*/


#ifndef OPERACIONASIGNACION_H
#define OPERACIONASIGNACION_H


#include "Operacion.h"


class OperacionAsignacion : public Operacion
{

 public:
	  
	OperacionAsignacion()
	{
		cadOp = ASIGNACION; 
	}
    
	/// a = b;
	Flt evalua(Flt *a, Flt *b, Flt *c)
	{
		*a = *b;
		return *a;
	}

	ostream & operator << (ostream &salida)
	{ 
		return salida << "="; 
	}

};

#endif // OPERACIONASIGNACION_H