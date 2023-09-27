/*
  Arbol.h

  Declaracion de la clase de los arboles binarios tipicos definidos en la forma
  recursiva descendente habitual con ptros a nodo y a subarboles izquierdo y
  derecho, partiendo desde la raiz hasta llegar a los nodos terminales (hojas).

  Como es logico, las hojas contienen nodos constante o variable, mientras que
  el resto de los nodos (no terminales) contienen operaciones.

  Introduzco un nodo central para las operaciones ternarias; CONVENIO:
  Para las operaciones:
	UNARIAS: Solo i
	BINARIAS: i y d
	TERNARIAS: i, d y c en ese orden

  Ademas, el subarbol central siempre se da como ultimo parametro debido a su
  caracter excepcional...
*/

#ifndef ARBOL_H
#define ARBOL_H

#include <iostream>

#include "Conjunto.h"
#include "EntradaPila.h"
#include "Nodo.h"
#include "Utiles.h"



class Arbol
{

 public:
	
	// Constructor aleatorio de arbol con unos 20 nodos a partir de 3 conj: var, ctes y operaciones
	Arbol(Conjunto<ptrNodo> &cVar, Conjunto<Flt> &tCte, Conjunto<ptrNodo> &cOpe, int prof = PROF_INICIAL);
	
	// Constructor simple de arbol a partir de un nodo y 2 subarboles (tercero central opcional)
	Arbol(Nodo *nod, Arbol *izq = 0, Arbol *der = 0, Arbol *cen = 0);
  
	// Un constructor de arboles completos, es decir que tienen TODOS los nodos y hasta cierta profundidad
	Arbol(int prof, Conjunto<ptrNodo> &cVar, Conjunto<Flt> &tCte, Conjunto<ptrNodo> &cOpe);

	// Tambien es fundamental un CONSTRUCTOR COPIA para facilitar algunas cosas
	Arbol(const Arbol &arbOrig);
 
	/* Destruye y libera memoria; tras mucho molestar he conseguido que exprima
	   TODA la memoria: Es fundamental que los componentes de un arbol se destruyan
	   DESPUES del propio arbol, ya que en caso contrario se armara la de dios al
	   acceder a una FMC de un objeto nodo destruido.
	*/
   ~Arbol()		
	{			
		if(n)	// Si el ptr apunta a un nodo,
		{
			if(n->esConstante())	// Es un nodo creado dinamicamente,
				delete n;			// y tambien lo destruyo dinamicamente
			n       = 0;			// lo desconecto y lo anulo
			nIntron = 0;			// No queda ya intron
		}

		if(i)	// Si hay subarbol derecho,
		{
			delete i;				// lo destruyo
			i = 0;					// y lo anulo para evitar postreros errores
		}

		if(c)	// Idem el central
		{
			delete c;
			c = 0;
		}

		if(d)	// Idem con el subarbol derecho
		{
			delete d;
			d = 0;
		}
	}
	
	// Devuelve la evaluacion numerica del arbol
	Flt evalua();			

	// Devuelve el ptr al nodo del arbol actual
	Nodo *getNodo()		
	{ 
		return n; 
	}

	// Idem con el subarbol izquierdo
	Arbol *getArbIzq()		
	{ 
		return i; 
	}

	// Idem con el subarbol central
	Arbol *getArbCen()		
	{ 
		return c; 
	}

	// Idem con el derecho
	Arbol *getArbDer()		
	{ 
		return d; 
	}

	// Para colocar subarbol izquierdo
	void setArbIzq(Arbol *arb)	
	{ 
		i = arb; 
	}

	// Para colocar subarbol izquierdo
	void setArbCen(Arbol *arb)	
	{ 
		c = arb; 
	}

	// Idem derecho
	void setArbDer(Arbol *arb)
	{
		d = arb; 
	}

	// Idem con el nodo
	void setNodo(Nodo *nod)	
	{ 
		n = nod; 
	}

	// Dice el numero de nodos de que consta un arbol (usada en el cruce)
	int getNumNodos()		
	{ 
		return 1 + (i ? i->getNumNodos() : 0) + (c ? c->getNumNodos() : 0) + (d ? d->getNumNodos() : 0); 
	}

	  //
	 // Fmc para tratamiento de intrones
	//

	// Ajusta el numero de intron para el nodo en curso
	void setIntron(Intron unNumIntron)
	{ 
		nIntron = unNumIntron; 
	}

	// Devuelve el numero de intron actual
	Intron getIntron()			
	{ 
		return nIntron; 
	}

	// Incrementa el numero de intron actual
	void incIntron()			
	{ 
		nIntron++; 
	}

	// Decrementa el numero de intron actual llegando como minimo a 1 ???
	void decIntron()			
	{ 
		nIntron--; 
	}

	// Dice el numero total de intrones de que consta un arbol (usada en el cruce)
	Intron getNumIntrones()		
	{ 
		return Intron(nIntron + (i ? i->getNumIntrones() : 0) + (c ? c->getNumIntrones() : 0) + (d ? d->getNumIntrones() : 0)); 
	}

	// Para ver si un subarbol esta incluido dentro de otro
	int estaIncluido(Arbol *a);

	// Para intercambiar entre si dos subarboles independientes pertenecientes al arbol que llama
	void intercambiaSubArboles(Arbol *a, Arbol *b);

	  //
	 // Fmc orientadas a la GP
	//
  	
	// Devuelve ptr a algun subarbol del que llama; el parametro es el numero de nodos del que llama
	Arbol *getPosicionCruce(int nNod);
  
	// Como la anterior, pero teniendo en cuenta los intrones (numero total y numero aleatorio interior...)
	Arbol *getPosicionCruceIntrones(/*Intron nInt, */ Intron nIntAleatorio);

	// Cruce de arboles
	void cruce(Arbol *posPadre, Arbol *posMadre);

	// Lo mismo pero devolviendo el puntero al punto de cruce (usado por los intrones...)
	Arbol *cruceIntron(Arbol *posPadre, Arbol *posMadre);

	// Cuando un arbol necesita variar la tabla de ctes a la que apuntan sus nodos ctes
	void reajustaNodosCtes(Conjunto<Flt> *otraTabCtes)
	{
		if(n)						// Si el ptr apunta a un nodo,
			if(n->esConstante())	// Es un nodo constante, y se reajusta para que apunte a otra parte
				n->setPtrCte(&(*otraTabCtes)[n->getIndice()]);

		if(i)		// Si hay subarbol derecho,
			i->reajustaNodosCtes(otraTabCtes);
		if(c)		// Idem el central
			c->reajustaNodosCtes(otraTabCtes);
		if(d)		// Idem con el subarbol derecho
			d->reajustaNodosCtes(otraTabCtes);
	}

	// Para crear una pila PIPOSECO
	int apilarPreOrden(EntradaPila *vPila, int numeroEntradas);
  
	// Para visualizar los subarboles interiores con intrones no unitarios
	void verIntrones();


 private:
  
	// El nodo contenido
	Nodo *n;
	
	// El valor de su numero de intron
	Intron  nIntron;		
  
	// Ptr al subarbol izquierdo
	Arbol *i;			
  
	// Ptr al subarbol central
	Arbol *c;			
  
	// Ptr al subarbol derecho
	Arbol *d;			

};

// Operador << redefinido para un cout de arboles en IN-ORDEN: a+b
ostream & operator << (ostream& os, Arbol &a);

typedef Arbol              *ptrArbol;
typedef Conjunto<ptrArbol> *ptrConjArboles;

#endif // ARBOL_H