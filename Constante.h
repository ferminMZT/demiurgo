/*
  Constante.h
  Clase de los nodos que contienen un puntero a una constante numerica (FP) de la tabla de ctes.
*/

#ifndef CONSTANTE_H
#define CONSTANTE_H

#include <iomanip>

#include "Nodo.h"
#include "Utiles.h"

class Constante : public Nodo
{

 public:

	// Constructor de ptr a una posicion aleatoria de la tabla de ctes en el rango [0, TAM_TABLA_GA-1]
	Constante(Conjunto<Flt> &tCtes)
	{
		indCte = RND(TAM_TABLA_GA);
		ptrCte = &(tCtes[indCte]);
	}

	// Constructor a partir de un indice y un ptr a cte
	Constante(int nuevoInd = 0, Flt *nuevoPtr = 0)
	{
		indCte = nuevoInd;
		ptrCte = nuevoPtr;
	}

	// Para saber el tipo
	TipoNodo getTipoNodo()
	{ 
		return CONSTANTE; 
	}
  
	// Para acceder al ptr donde se guarda el valor almacenado
	virtual Flt *getPtrValor()
	{ 
		return ptrCte; 
	}

	// Destructor que no hace nada
	~Constante()			
	{
		ptrCte = 0;
		indCte = -1;			// Un valor que no valga para nada...
	}

	// Reajusta el ptr a cte para que apunte a la direccion dada
	void setPtrCte(Flt *dir)	
	{ 
		ptrCte = dir; 
	}

	// Para saber que el nodo contiene una constante	
	int esConstante() const	
	{ 
		return VERDAD; 
	}

	// Devuelve el valor de la cte apuntada
	Flt evalua(Flt *a=0, Flt *b=0, Flt *c=0)	
	{ 
		return *ptrCte; 
	}

	// Devuelve el indice de esa cte dentro de la tabla de ctes
	int getIndice()		
	{ 
		return indCte; 
	}

	// Cambia el indice de cte (despues habra que ajustar el puntero)
	void setIndice(int nuevoInd)	
	{ 
		indCte = nuevoInd; 
	}
  
	ostream & operator << (ostream &salida)
	{ 
		return salida << setprecision(4) << *ptrCte; 
	}


 private:

	// Posicion ocupada por la cte en la tabla de ctes
	Flt *ptrCte;      	

	// Indice de esa posicion (para los cruces...)
	int indCte;			

};

#endif // CONSTANTE_H