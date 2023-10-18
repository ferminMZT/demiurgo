// \file Lentes.h
// Problema 12
// Lentes_______________________________________________________________________
// Tenemos 24 ejemplos con 4 atributos sobre pacientes del oftalmologo, y como
// clasificacion se produce 1 de 3 valores.

#ifndef LENTES_H
#define LENTES_H

#include "Problema.h"
#include "../Bateria.h"
#include "../Piposeco.h"
#include "../Variable.h"

class Lentes : public Problema
{

 public:

    Lentes()
    {
        nombre       = "Lentes: Debo usarlas duras, blandas o no usarlas?";
        batLentes    = new Bateria("lenses.dat");
        numEjemplos  = batLentes->getNumEjemplos();
        numAtributos = batLentes->getNumAtributos();
    }

    ~Lentes()
    {
        delete batLentes;
        numEjemplos = numAtributos = 0;
        pilaEvaluacion = 0;
    }

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return 0.0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
    { return 0.0; }


 private:

    Bateria  *batLentes;		// Bateria de ejemplos a clasificar
    int       numEjemplos;      	// Numero de ejemplos de la bateria
    int       numAtributos;	// Numero de atributos de cada ejemplo
    Variable *var;
    Flt       errTodasMuestras;
    int       ne, na;
    Flt       valEvaluado, valClase;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif // LENTES_H
