/**
  Sombrero.cpp
*/


#include "Sombrero.h"


Sombrero::Sombrero()
{
	nombre = "Sombrero: f(x, y) = sin(x^2 + y^2)/(x^2 + y^2)";
	tam    = 20;
	

	// Asigno memoria para la imagen en 2 pasos

	// Primero las lineas
	valores = new Flt*[tam];		
	// Casca por no poder asignar memoria para las lineas
	if(!valores)					
	{
		DEBUG("Error!");
		cout << "No asigna memoria a la matriz de datos!!!" << endl;
		exit(-1);
	}

	// Despues cada linea columna a columna
	for(y = 0; y < tam; y++)	
	{
		valores[y] = new Flt[tam];
		// Casca por no poder asignar memoria a una linea entera
		if(!valores[y])		
		{
			DEBUG("Error!");
			cout << "No asigna memoria al vector " << y << endl;
			exit(-1);
		}
	}

	// Ya hay memoria asignada y todo va bien!!!
	// Vamos a meter los valores
	a = -3.0;
	b = +3.0;
	inc = (b - a) / tam;

	for(valX = a, x = 0; x < tam; valX += inc, x++)
		for(valY = a, y = 0; y < tam; valY += inc, y++)		
		{
			valores[x][y] = Flt(sin(valX*valX + valY*valY) / (valX*valX + valY*valY));
		}
}

Sombrero::~Sombrero()
{
 for(y = 0; y < tam; y++)
 {
	 if(valores[y])
	 {
		delete [] valores[y];
		valores[y] = 0;
	 }
 }

 if(valores)
 {
	delete [] valores;
	valores = 0;
 }

 x = y = tam = 0;
 pilaEvaluacion = 0;
}

Flt Sombrero::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
	// Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
	pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
	errTotal = 0.;
	// Conecto variables
	Variable *X = (Variable *) (*pConjVariables)[0];
	Variable *Y = (Variable *) (*pConjVariables)[1];

	// Se procesan todos los valores
	for(valX = a, x = 0; x < tam; valX += inc, x++)
		for(valY = a, y = 0; y < tam; valY += inc, y++)		
		{
			(*X) = valX;
			(*Y) = valY;

			valEvaluado = pilaEvaluacion->evalua();
			errTotal += Flt(fabs(valEvaluado - valores[x][y]));
		}

	delete pilaEvaluacion;

	// Un poco de error aleatorio...
	valEvaluado += (Flt) ((COC()) ? valEvaluado/10.0 : -valEvaluado/10.0);
	return errTotal;
}