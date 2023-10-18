// Problema 7
// CancerPecho__________________________________________________________________
// Tenemos 569 casos de cancer de pecho (de Wisconsin) lirios para clasificar en 2 clases distintas
// usando 30 atributos

#ifndef CANCERPECHO_H
#define CANCERPECHO_H

#include "Problema.h"
#include "../Bateria.h"
#include "../Variable.h"
#include "../Piposeco.h"

class CancerPecho : public Problema
{
 public:

    CancerPecho()
    {
        nombre = "El cancer de pecho de Wisconsin";
        batcancer = new Bateria("cancerPecho.dat");
        numEjemplos = batcancer->getNumEjemplos();
        numAtributos = batcancer->getNumAtributos();
    }

    ~CancerPecho()
    {
        if (batcancer != nullptr) {
            delete batcancer;
            batcancer = nullptr;
        }
        numEjemplos = numAtributos = 0;
        if (pilaEvaluacion != nullptr) {
            delete pilaEvaluacion;
            pilaEvaluacion = nullptr;
        }        
    }

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return 0.0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
    { return 0.0; }


 private:

    // Bateria de ejemplos a clasificar.
    Bateria *batcancer;
    // Numero de ejemplos de la bateria.
    int numEjemplos;
    // Numero de atributos de cada ejemplo.     	
    int numAtributos;
    Variable *var;
    Flt errTodasMuestras;
    int ne, na;
    Flt valEvaluado, valClase;
    // Pila piposeco para la evaluacion.
    Piposeco *pilaEvaluacion;

};

#endif // CANCERPECHO_H
