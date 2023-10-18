// \file Vino.h
// Problema 5
// Vino_________________________________________________________________________
// Tenemos 178 vinos para clasificar en 3 clases distintas usando 13 atributos.

#ifndef VINO_H
#define VINO_H

#include "Problema.h"
#include "../Bateria.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Vino : public Problema
{

 public:
    Vino()
    {
        nombre       = "La clasificacion enologica";
        batVino      = new Bateria("vino.dat");
        numEjemplos  = batVino->getNumEjemplos();
        numAtributos = batVino->getNumAtributos();
    }

  ~Vino()
    {
        delete batVino;
        numEjemplos = numAtributos = 0;
        pilaEvaluacion = 0;
    }

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return 0.0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
    { return 0.0; }


 private:

    Bateria  *batVino;		// Bateria de ejemplos a clasificar
    int       numEjemplos;	// Numero de ejemplos de la bateria
    int       numAtributos;	// Numero de atributos de cada ejemplo
    Variable *var;
    Flt       errTodasMuestras;
    int       ne, na;
    Flt       valEvaluado, valClase;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif // VINO_H