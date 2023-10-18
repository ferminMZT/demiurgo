// \file Lirio.h
// Problema 6
// Lirio________________________________________________________________________
// Tenemos 150 lirios para clasificar en 3 clases distintas usando 4 atributos.

#ifndef LIRIO_H
#define LIRIO_H

#include "Problema.h"
#include "../Bateria.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Lirio : public Problema
{
public:

    Lirio()
    {
        nombre       = "Clasificacion de los lirios";
        batLirio     = new Bateria("lirios.dat");
        numEjemplos  = batLirio->getNumEjemplos();
        numAtributos = batLirio->getNumAtributos();
    }
  ~Lirio()
    {
        delete batLirio;
        numEjemplos = numAtributos = 0;
        pilaEvaluacion = 0;
    }

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return 0.0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest);


 private:

    Bateria  *batLirio;		// Bateria de ejemplos a clasificar
    int       numEjemplos;      	// Numero de ejemplos de la bateria
    int       numAtributos;	// Numero de atributos de cada ejemplo
    Variable *var;
    Flt       errTodasMuestras;
    int       ne, na;
    Flt       valEvaluado, valClase;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 
 };

 #endif // LIRIO_H
