// \file Rosenbrock.cpp
#include "Rosenbrock.h"
#include <math.h>

Flt Rosenbrock::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
    // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
    pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
    // Se cargan las variables
    X = (Variable *) (*pConjVariables)[0];
    Y = (Variable *) (*pConjVariables)[1];
    errTodasMuestras = 0.;
    Flt num = .0;
    for(x = a; x <= b; x += inc)
        for(y = a; y <= b; y += inc)
        {
            (*X) = x; (*Y) = y;
            valEvaluado = pilaEvaluacion->evalua();
            errTodasMuestras += fabs(valEvaluado - (100.*(x*x - y*y) + (1. - x)*(1. - x)));
            num++;
        }
    delete pilaEvaluacion;
    return errTodasMuestras/num;
}
