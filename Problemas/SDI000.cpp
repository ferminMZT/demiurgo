/**
	\file SDI000.cpp
	Implementacion de la clase SDI000, para la resolucion del problema de las
	SDI000es entrelazadas
*/

#include <math.h>
#include <fstream>
#include <iostream>

#include "SDI000.h"

SDI000::SDI000()
{
	nombre = "Ajuste joystick-carro(Y)";
	numPtos = 0;

	std::string fichero = "datos006.log";

	// Ojo, que sin el ios::nocreate crearia el fichero aunque no existiera.
	std::ifstream f(fichero.c_str(), std::ios::in);

	// Tambien podria usar las fmc fail(), good(), isopen().
	if(!f)
	{
		std::cout << "Error al abrir el fichero " << fichero << std::endl;
		return;
	}

	const int tam = 200;
	char buffer[tam];

	// Posicion de inicio de las variables.
	int posIniVar = 0;

	// Leo todas las lineas que comienzan por #
	do 
	{
		posIniVar = f.tellg();
		// Grave error en los ficheros de Andres que acaban en el caracter
		// de control 13, y no en el 10 habitual!!!
		f.getline(buffer, tam, '\n');
	} while(buffer[0] == '#' && !f.eof());

	if(f.eof())
	{
		std::cout << "Error en el interior del fichero " << fichero << std::endl;
		return;
	}
	// Y esa linea que NO empieza por # debo escupirla para poderla leer...
	// como todavia no se unbuferear (o como se diga) opto por volver atras
	f.clear();
	f.seekg(posIniVar);

	// Vamos a contar el numero de columnas usando la linea almacenada en el buffer
	std::string linea = buffer;
	int numColumnas = 0;
	const char cifras[] = "0123456789.+-eE";


	while(1)
	{
		// std::cout << "<" << linea << ">" <<endl;
		int iniCol = linea.find_first_of(cifras);
		int finCol = linea.find_first_not_of(cifras);
		if(iniCol == 0 && finCol == -1)
		{
			numColumnas++;
			break;
		}

		if(iniCol < finCol)
		{
			linea = linea.substr(finCol, linea.length() - finCol);
			numColumnas++;
		}
		else
		{
			// Quita blancos del principio
			linea = linea.substr(iniCol, linea.length() - iniCol);
		}
	}

	// Calculo del numero de filas
	int numFilas = 0;

	while(1)
	{
		f.getline(buffer, tam, '\n');
		if(f.eof())
			break;
		numFilas++;
	}
	
	std::cout << "Columnas: " << numColumnas << std::endl;
	std::cout << "Filas:    " << numFilas << std::endl;

	// Vuelta al inicio de las variables para cargar los valores
	// Con clear reseteo el bit de EOF
	f.clear();
	f.seekg(posIniVar);

	// Todo empieza en 0 como el C manda!!!
	int columnaX = 1;
	int columnaY = 5;

	// Leemos todas las variables en las columnas requeridas
	int fil, col;
	Flt valor;
	for(fil = 0; fil < numFilas; fil++)
	{
		for(col = 0; col < numColumnas; col++)
		{
			f >> valor;
			// std::cout << valor;
			if(col == columnaX)
			{
				x.push_back(valor);
				// std::cout << "X";
			}
			if(col == columnaY)
			{
				y.push_back(valor);
				// std::cout << "Y";
			}

			// std::cout << " ";
		}
		// std::cout << fil << std::endl;
	}

	numPtos = numFilas;

	// Experimentos varios:
	// 1) La Y que es absoluta la vamos a dejar como incremental:

	for(; fil != 0; fil--)
	{
		y[fil] = y[fil - 1] - y[fil];
		x[fil] = x[fil - 1] - x[fil];
	}

	y[fil] = 0;
	x[fil] = 0;

}

Flt SDI000::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{

 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
 
 // Se cargan las variables
 X = (Variable *) (*pConjVariables)[0];
 errTodasMuestras = 0.;
 for(n = 0; n < numPtos; n++)
 {
  (*X) = x[n];
  valEvaluado = pilaEvaluacion->evalua();
  errTodasMuestras += (Flt) fabs(valEvaluado - y[n]) ;
 }

 delete pilaEvaluacion;

 return errTodasMuestras/numPtos;
}

Flt SDI000::distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
{

// Flt val1, val2;
 Flt dist = 0.0;
/*
 X = (Variable *) (*pConjVariables)[0];
 Y = (Variable *) (*pConjVariables)[1];
 for(n = 0; n < numPuntos; n++)
 {
  (*X) = valX[n];
  (*Y) = valY[n];
  val1 = pInd1->evalua();
  val2 = pInd2->evalua();
  val1 = (Flt) ((val1 < 0.0) ? -1. : +1.);
  val2 = (Flt) ((val2 < 0.0) ? -1. : +1.);
  if(val1 != val2)
  {
   if(val1 != valClas[n]) 	// Solo me interesa comparar en los puntos en los que el individuo 1 falla
   {
    if(val2 == valClas[n])	// El primero falla, pero el segundo acierta
     dist += 10.0;
   }
   else
    dist += 1.0;
  }
 }
*/
 return dist;
}

Flt fG(Flt X)
{
	return Flt((((((X > 0.0) ? ((0.072 / (0.3307 - X)) - (sin(0.3307))) : (X * X))) * X) / ((-(X)) + 0.2422) > 0.0) ?
(((-(((0.3651 / (-(0.3651))) / X) + (((sin(((0.3651 / (-(0.3651))) / X) + (((((((0.072 > 0.0) ? (X) : 
(X * (X - 0.3651))) > 0.0) ? (0.2842) : (X))) * (((((X * (0.072 * 0.3651) > 0.0) ? (X) : (0.4609)))
/ (((X * X) + X) / ((X / X) - 0.2422))) * (-(0.4332 / 0.072)))) / (X * (-(X)))))) * 
(sin(sin(-(((((X > 0.0) ? (X) : (0.4609))) / ((0.072 + X) / ((-(X)) - X))) * (-(0.4332 / 0.072))))))) / X)))
/ 0.2842) * ((0.072 / (0.3307 - X)) / (X - 0.3651))) : (-((((0.0071 > 0.0) ? (0.2422) :
(((((0.3307 - 0.3651 > 0.0) ? (X) : (0.4609))) - X) + 0.0071))) * ((((0.0071 > 0.0) ? (0.2422) : (0.072)))
* (X * (-(X)))))));
}

Flt SDI000::testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
{
	std::ofstream fSal(nomFichTest);
	Flt error = 0;
	
	for(int n = 1; n < numPtos; n++)
	{
		Flt valX = x[n];
		x[n] += x[n-1];
		Flt valY = y[n];
		y[n] += y[n-1];

		fSal << x[n] << " " << y[n] << " " << fG(valX) << " " << (valY - fG(valX)) << std::endl;
		error += (valY - fG(valX));
	}

	return error/numPtos;
}


