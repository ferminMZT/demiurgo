/** 
  Operacion.h
  Clase Operacion padre de todas las operaciones
  Clase de los nodos que contienen una operacion: Hay un pequeño problema...
  Quiero evitar tener que andar usando case o if para averiguar el tipo de
  operacion al que me enfrento en cada evaluacion de nodo de tipo operacion;
  esto lo soluciono creando una clase hija para cada operacion...

  \todo Mejorar esto mediante ptr a fmc, functores o metatemplates...
*/


#ifndef OPERACION_H
#define OPERACION_H


#include "Nodo.h"


/// Clase abstracta, madre de las distintas operaciones
class Operacion : public Nodo
{

 protected:
	
	// El tipo de la operacion es necesario para alguna mutacion
	TipoNodo cadOp;		


 public:

	/** Al ser abstracta, no necesita constructor, pero si un destructor vacio y virtual para
	    llamar a los destructores de sus clases derivadas por medio de un ptr a esta clase base.
		Si no pongo esto, los destructores de los hijos no serian llamados...
	*/
	virtual ~Operacion()
	{ 
		; // Nada
	}

	virtual Flt evalua(Flt *a=0, Flt *b=0, Flt *c=0) = 0;
	
	virtual ostream & operator << (ostream &salida) = 0;

	/// Para saber que un nodo contiene una operacion
	int esOperacion() const
	{ 
		return VERDAD; 
	}

	// Una operacion NO es terminal
	int esTerminal() const	
	{ 
		return FALSO; 
	}

	// Una operacion SI es NO terminal
	int esNoTerminal() const	
	{ 
		return VERDAD; 
	}

	// Para saber tipo de una operacion	
	TipoNodo getTipoNodo()	
	{ 
		return cadOp; 
	}

};

#endif // OPERACION_H