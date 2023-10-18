#include "RaizCuadrada.h"
#include <math.h>

RaizCuadrada::RaizCuadrada()
{
    nombre = "La raiz cuadrada (sqrt) en [0, 1]";
    numPuntos = 100;
    valX = new Flt[numPuntos];
    valY = new Flt[numPuntos];
    Flt i = 1. / numPuntos;
    Flt x = 0.;

    for(n = 0; n < numPuntos; n++)
    {
        valX[n] = x;
        valY[n] = sqrt(x);
        x += i;
    }
}

Flt RaizCuadrada::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
    // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
    pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
    // Se cargan las variables
    X = (Variable *) (*pConjVariables)[0];
    errTodasMuestras = 0.;
    for(n = 0; n < numPuntos; n++)
    {
        (*X) = valX[n];
        valEvaluado = pilaEvaluacion->evalua();
        errTodasMuestras += fabs(valEvaluado - valY[n]);
    }
    delete pilaEvaluacion;
    return errTodasMuestras;
}