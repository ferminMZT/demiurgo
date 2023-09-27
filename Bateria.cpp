/*
  Bateria.cpp
  Implementacion de las fmc de la clase Bateria
*/

#include <fstream>

#include "Bateria.h"

// Constructor a partir de un fichero
Bateria::Bateria(char *nomFich)
{
 fstream fBateria(nomFich, ios::in);	// Fichero con datos de entrada
 int nEj;

 if(!fBateria)			// Si casca al no encontrar el fichero,
 {				// bueno es saberlo y pirarse
  DEBUG("Error!");
  cout << "No puedo abrir el fichero bateria: " << nomFich << endl;
  exit(-1);
 }
 fBateria >> numClases;
 //cout << numClases << endl;
 fBateria >> numAtributos;
 //cout << numAtributos << endl;
 fBateria >> numEjemplos;
 //cout << numEjemplos << endl;
 // Asigno memoria para los ejemplos: en 2 pasos
 atributos = new Flt * [numEjemplos];
 if(!atributos)			// Casca por no poder asignar memoria
 {
  DEBUG("Error!");
  cout << "No asigna memoria a los atributos de los ejemplos (1): " << (numEjemplos*numAtributos) << endl;
  exit(-1);
 }
 for(nEj = 0; nEj < numEjemplos; nEj++)
 {
  atributos[nEj] = new Flt[numAtributos];
  if(!atributos[nEj])		// Casca por no poder asignar memoria
  {
   DEBUG("Error!");
   cout << "No asigna memoria a los atributos de los ejemplos (2): " << nEj << endl;
   exit(-1);
  }
 }
 // Asigno memoria para los datos de salida
 clases = new Flt * [numEjemplos];
 if(!clases)			// Casca por no poder asignar memoria
 {
  DEBUG("Error!");
  cout << "No asigna memoria a las clases (1): " << (numEjemplos*numClases) << endl;
  exit(-1);
 }
 for(nEj = 0; nEj < numEjemplos; nEj++)
 {
  clases[nEj] = new Flt[numClases];
  if(!clases[nEj])		// Casca por no poder asignar memoria
  {
   DEBUG("Error!");
   cout << "No asigna memoria a las clases (2): " << nEj << endl;
   exit(-1);
  }
 }
 // Aqui todo va bien y se cargan los ejemplos como clases + atributos
 for(nEj = 0; nEj < numEjemplos; nEj++)
 {
  for(int nCla = 0; nCla < numClases; nCla++)
  {
   fBateria >> clases[nEj][nCla];
   //cout << "C:" << clases[nEj][nCla] << " ";
  }
  for(int nAtr = 0; nAtr < numAtributos; nAtr++)
  {
   fBateria >> atributos[nEj][nAtr];
   //cout << atributos[nEj][nAtr] << " ";
  }
  //cout << endl;
 }
 fBateria.close();
}

// Destructor
Bateria::~Bateria()
{
 if(numEjemplos)
 {
  for(int n = 0; n < numEjemplos; n++)
  {
   delete [] clases[n];
   delete [] atributos[n];
  }
  delete [] clases;
  delete [] atributos;
  numAtributos = numClases = numEjemplos = 0;
 }
}