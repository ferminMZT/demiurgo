/*
  Nodo.h

  Declaracion de la clase base de la que se derivan todos los tipos de nodos
  especializados para los arboles.

  Es una clase abstracta y todo se derivara virtualmente en sus subclases que
  seran operacion, constante y variable.
*/

#ifndef NODO_H
#define NODO_H

#include "Utiles.h"
#include "Conjunto.h"

class Nodo
{
 
 public:
	
	/* Al ser una clase abstracta, no necesita ningun constructor, pero SI que
       necesita un destructor VIRTUAL para poder llamar a los destructores de
       sus clases derivadas por medio de un puntero a esta clase base...
	*/
	virtual ~Nodo()		// Si no pongo esto, los destructores de los
	{					// hijos NO SERAN LLAMADOS NUNCA. Viva!
		; 
	}				

	// Devuelve el resultado de evaluar el nodo en curso (c responde al nodo
	// ultimo y central)
	virtual Flt evalua(Flt *a=0, Flt *b=0, Flt *c=0) = 0;

	// Devuelve un stream con la representacion del nodo en curso
	virtual std::ostream & operator << (std::ostream &salida) = 0;
  
	// Para saber el tipo de un nodo
	virtual TipoNodo getTipoNodo() = 0;

      //
     // Para los nodos de tipo operacion 
	//

	// Dice si el nodo en curso es una operacion
	virtual int esOperacion() const
	{ 
		return FALSO; 
	}

	// Dice si el nodo en curso es una operacion binaria
	virtual int esBinario() const		
	{ 
		return VERDAD; 
	}

	// Dice si el nodo en curso es una operacion unaria (NEGATIVO)
	virtual int esUnario() const
	{ 
		return FALSO; 
	}

	// Dice si el nodo en curso es una operacion ternaria (CONDICIONAL)
	virtual int esTernario() const	
	{ 
		return FALSO; 
	}

	  //
	 // Para los nodos de tipo cte 
	//

	// Idem con constante
	virtual int esConstante() const 	
	{ 
		return FALSO; 
	}

	virtual int getIndice()
	{ 
		return 0; 
	}
	
	virtual void setIndice(int nuevoIndice)
	{ 
		; 
	}

	// Reajusta el ptr a cte...
	virtual void setPtrCte(Flt *dir=0)	
	{ 
		; 
	}

	  //
	 // Para los nodos de tipo variable
	//
  	 
	// Idem con variable
	virtual int esVariable() const 	
	{ 
		return FALSO; 
	}

	virtual Flt getValor()
	{ 
		return 0; 
	}

	  //
	 // Para los nodos constantes o variables, que almacenan un valor
	//
  	 
	virtual Flt *getPtrValor()
	{ 
		return 0; 
	}

	  //
	 // Para cualquier nodo
	//

	// Para saber si un nodo es terminal
	virtual int esTerminal() const 	
	{ 
		return VERDAD; 
	}

	// Idem para NO terminal
	virtual int esNoTerminal() const 	
	{ 
		return FALSO; 
	}


 private:
  
	// De momento un nodo no tiene ningun DMC propio

};

typedef Nodo              *ptrNodo;
typedef Conjunto<ptrNodo> *ptrConjNodos;

#endif // NODO_H