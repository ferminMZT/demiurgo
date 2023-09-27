/*
  Utiles.h
  Implementacion de algunas funciones de uso general
*/

#include <fstream>
#include <math.h>

#include "Utiles.h"
#include "Individuo.h"
#include "Nodo.h"
#include "Variable.h"


// Para los errores fatales y con posterior huida
void Error(char *mensajeError, char *parametroAdicional)
{
 cout << endl;
 cout << "Error: ";
 cout << mensajeError;
 if(parametroAdicional)
  cout << " " << parametroAdicional;
 cout << endl;
 cout << endl;
 exit(-1);
}



// La funcion que ayuda a la ordenacion de las poblaciones...
int fExtOrdenInd(const void *a, const void *b)
{
 // Hay que ser muy precavido, ya que los parametros que llegan a esta funcion NO SON PUNTEROS A OBJETO,
 // SINO PUNTEROS A PUNTEROS A OBJETO. Por eso se han usado esos casting...!!!
 Flt errA = ((Individuo *)*(DIRMEMORIA *)a)->getError();
 Flt errB = ((Individuo *)*(DIRMEMORIA *)b)->getError();
 // Hay que considerar el epsilon...
 if(errA < errB)
  return -1;
 if(errA > errB)
  return +1;
 // Llegados a este punto de error igual, siempre sera mejor aquel individuo con menos nodos...
 int nNodA = ((Individuo *)*(DIRMEMORIA *)a)->getNumNodos();
 int nNodB = ((Individuo *)*(DIRMEMORIA *)b)->getNumNodos();
 if(nNodA < nNodB)
  return -1;
 if(nNodA > nNodB)
  return +1;
 return 0;
}

// A partir de una cadena inicial como "fichero000.ext" va generando nombres de ficheros con numeracion
// correlativa y que AUN no hayan sido creados. Desde luego este programador es bueno...
// Esta funcion asigna memoria!!! Luego sera responsabilidad del zoquete de turno, digo del sufrido
// programador, liberarla adecuadamente...
// No toca la memoria inicialmente pasada como parametro, sino que hace una copia...
char *generaNombre(char *ptrCadOrigen)
{
	if(ptrCadOrigen == 0)
		return 0;

	char *ptrCad = new char[strlen(ptrCadOrigen) + 1];
	if(ptrCad)
		if(ptrCad != strcpy(ptrCad, ptrCadOrigen))
		{
			delete [] ptrCad;
			ptrCad = 0;
			return ptrCad;
		}

	int posPunto;			// Posicion del punto que separa nombre y extension
	int numMax = 1;		// Maximo numero de orden generado, es 10^numDigitos - 1
	int n;				// El misero y humilde indice, que es lo mas importante al final!!!
	// Se cargan las variables
	for(posPunto = 0; ptrCad[posPunto] != '.'; posPunto++)
		if(ptrCad[posPunto] == '0')
			numMax *= 10;
		// Se generan los sucesivos nombres de ficheros
		for(n = 0; n < numMax; n++)
		{
			if(n < 10)					// [0, 9]
				ptrCad[posPunto-1] = (char)('0' + n);
			else
				if(n < 100)					// [10, 99]
				{
					ptrCad[posPunto-1] = (char)('0' + n%10);
					ptrCad[posPunto-2] = (char)('0' + n/10);
				}
				else
					if(n < 1000)                      		// [100, 999]
					{
						ptrCad[posPunto-1] = (char)('0' + (n%100)%10);
						ptrCad[posPunto-2] = (char)('0' + (n%100)/10);
						ptrCad[posPunto-3] = (char)('0' + n/100);
					}
					if(_access(ptrCad, 0) != 0)	// Si el tal fichero no existe, este es el nombre
						return ptrCad;		// que buscabamos y ya podemos volver
		}
		return 0;
}