// Problema 4
// Gudermann____________________________________________________________________
// log10[1./cos(x) + tan(x)]		x en (-M_PI/2, +M_PI/2)

#ifndef GUDERMANN_H
#define GUDERMANN_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Gudermann : public Problema
{

 public:

    Gudermann();
    ~Gudermann()
    {
        delete [] valX;
        delete [] valG;
        numPuntos      = 0;
        pilaEvaluacion = 0;
    }

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return 0.0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
    { return 0.0; }


 private:

    int numPuntos;		// Numero de puntos a tratar
    Flt *valX;			// Componente X
    Flt *valG;                    // Valor de la Gudermanniana

    // Por otra parte, pongo aqui unas variables auxiliares para el calculo del
    // fitness para no tener que crearlas y ganar una poca velocidad mas... ?
    Variable *X;
    Flt       errTodasMuestras;
    int       n;
    Flt       valEvaluado;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif // GUDERMANN
