/*
  Variable.h

  Clase de los nodos que contienen una variable numerica (FP).
*/


#ifndef VARIABLE_H
#define VARIABLE_H


#include <iostream>


#include "Nodo.h"
#include "Utiles.h"


class Variable : public Nodo
{

 public:
	
	/// Constructor
	Variable(char nombre, Flt valor)	
	{
		nom = new char[2];
		nom[0] = nombre;
		nom[1] = 0;
		val = valor;
	}

	/// Para saber el tipo
	TipoNodo getTipoNodo()
	{ 
		return VARIABLE; 
	}

	/// Para acceder al ptr donde se guarda el valor almacenado
	virtual Flt * getPtrValor()
	{ 
		return &val; 
	}

	/// Destructor NO vacio
	~Variable()			
	{
		if(nom)
		{
			delete [] nom;
			nom = 0;
		}
	}

	/// Para comprobar que el nodo contiene una variable
	int esVariable() const	
	{ 
		return VERDAD; 
	}
  
	/// Devuelve el valor
	Flt evalua(Flt *a=0, Flt *b=0, Flt *c=0)	
	{ 
		return val; 
	}
  
	/// Igual, pero por no llevar parametros debe ser mas rapida
	Flt getValor() 		
	{ 
		return val; 
	}

	// Esto es un invento...
	char getNombre()
	{ 
		return nom[0]; 
	}

	/** 
	  Para manejar variables, es imprescindible poder cambiar sus valores de una manera
      facil: para ello redefino el operador =, para que si X es una variable, se pueda
      hacer: X = 3.14; OJO: esto NO es un constructor copia!!!
      Esta es mi propia y heretica version de la redefinicion del operador =
	*/
	void operator = (const Flt nuevoValor)
	{ 
		val = nuevoValor; 		// Esto deberia ser mas rapido todavia???
	}

	// Lo mismo de antes, sera mas rapido???
	void asignaValor(const Flt nuevoValor)
	{ 
		val = nuevoValor; 
	}
  
	ostream & operator << (ostream &salida)
	{ 
		return salida << nom; 
	}


 private:
  
	/// Nombre de la variable
	char *nom;			
	/// Valor contenido en esta variable
	Flt val;			

};

#endif // VARIABLE_H