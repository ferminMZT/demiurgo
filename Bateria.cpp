/*
    \file Bateria.cpp
    Implementacion de las fmc de la clase Bateria.
*/

#include "Bateria.h"
#include <fstream>

// Constructor a partir de un fichero.
Bateria::Bateria(const char *nomFich)
{
    // Fichero con datos de entrada.
    std::fstream fBateria(nomFich, std::ios::in);	
    int nEj;

    // Si casca al no encontrar el fichero, bueno es saberlo y pirarse.
    if(!fBateria)			
    {				
        DEBUG("Error!");
        std::cout << "No puedo abrir el fichero bateria: " << nomFich << std::endl;
        exit(-1);
    }

    fBateria >> numClases;
    // std::cout << numClases << std::endl;
    fBateria >> numAtributos;
    // std::cout << numAtributos << std::endl;
    fBateria >> numEjemplos;
    // std::cout << numEjemplos << std::endl;

    // Asigno memoria para los ejemplos: en 2 pasos.
    atributos = new Flt *[numEjemplos];

    // Casca por no poder asignar memoria
    if(!atributos)			
    {
        DEBUG("Error!");
        std::cout << "No asigna memoria a los atributos de los ejemplos (1): " << (numEjemplos*numAtributos) << std::endl;
        exit(-1);
    }
    for(nEj = 0; nEj < numEjemplos; nEj++)
    {
        atributos[nEj] = new Flt[numAtributos];
        if(!atributos[nEj])		// Casca por no poder asignar memoria
        {
            DEBUG("Error!");
            std::cout << "No asigna memoria a los atributos de los ejemplos (2): " << nEj << std::endl;
            exit(-1);
        }
    }
    // Asigno memoria para los datos de salida
    clases = new Flt * [numEjemplos];
    // Casca por no poder asignar memoria.
    if(!clases)
    {
        DEBUG("Error!");
        std::cout << "No asigna memoria a las clases (1): " << (numEjemplos*numClases) << std::endl;
        exit(-1);
    }
    for(nEj = 0; nEj < numEjemplos; nEj++)
    {
        clases[nEj] = new Flt[numClases];
        if(!clases[nEj])		// Casca por no poder asignar memoria
        {
            DEBUG("Error!");
            std::cout << "No asigna memoria a las clases (2): " << nEj << std::endl;
            exit(-1);
        }
    }
    // Aqui todo va bien y se cargan los ejemplos como clases + atributos
    for(nEj = 0; nEj < numEjemplos; nEj++)
    {
        for(int nCla = 0; nCla < numClases; nCla++)
        {
            fBateria >> clases[nEj][nCla];
            // std::cout << "C:" << clases[nEj][nCla] << " ";
        }
        for(int nAtr = 0; nAtr < numAtributos; nAtr++)
        {
            fBateria >> atributos[nEj][nAtr];
            // std::cout << atributos[nEj][nAtr] << " ";
        }
        // std::cout << std::endl;
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