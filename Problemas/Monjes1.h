// \file Monjes1.h
// Problema 8
// monjes1______________________________________________________________________
// Tenemos 569 casos de cancer de pecho (de Wisconsin) lirios para clasificar en 2 clases distintas
// usando 30 atributos

#ifndef MONJES1_H
#define MONJES1_H

#include "Problema.h"
#include "../Bateria.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Monjes1 : public Problema
{

 public:
    Monjes1()
    {
        nombre       = "Los monjes (I)";
        batMonjes1   = new Bateria("monjes3.dat");
        numEjemplos  = batMonjes1->getNumEjemplos();
        numAtributos = batMonjes1->getNumAtributos();
    }
  ~Monjes1()
    {
        delete batMonjes1;
        numEjemplos = numAtributos = 0;
        pilaEvaluacion = 0;
    }

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return 0.0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
    { return 0.0; }


 private:

    Bateria  *batMonjes1;		// Bateria de ejemplos a clasificar
    int       numEjemplos;      	// Numero de ejemplos de la bateria
    int       numAtributos;	// Numero de atributos de cada ejemplo
    Variable *var;
    Flt       errTodasMuestras;
    int       ne, na;
    Flt       valEvaluado, valClase;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 
};

#endif // MONJES1_H
