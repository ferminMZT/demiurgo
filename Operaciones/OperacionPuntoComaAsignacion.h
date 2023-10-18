/**
  Clase OperacionPuntoComaAsignacion
  Implementacion de la operacion asignacion de C, pero justo tras un punto y coma separador:
  
			;
		   /|\
         /  |  \
	   /    |    \
	expI   var   expD
	  
  No devuelve nada, sino que se limita a que var tome el valor resultante de la evaluacion de la
  expresion derecha.
*/


#ifndef OPERACIONPUNTOCOMAASIGNACION_H
#define OPERACIONPUNTOCOMAASIGNACION_H


#include "Operacion.h"


class OperacionAsignacionComa : public Operacion
{

 public:
	  
	OperacionAsignacionComa()
	{
		cadOp = ASIGNACIONCOMA; //PUNTOCOMAASIGNACION; 
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
//	Flt evalua(Flt *exprIzq, Flt *var, Flt *exprDer)
//	{
//		*var = *exprDer;
//		// Muy bien, siempre devuelve la evaluacion de lo que tiene a la izquierda!!!
//		return *exprIzq;
//	}

	Flt evalua(Flt *var, Flt *exprCen, Flt *exprDer)
	{
		// Asigna lo que debe
		*var = *exprCen;
		// Muy bien, siempre devuelve la evaluacion de lo que tiene a la derecha!!!
		return *exprDer;
	}

	std::ostream & operator << (std::ostream &salida)
	{ 
		return salida << ";"; 
	}

};

#endif // OPERACIONPUNTOCOMAASIGNACION_H