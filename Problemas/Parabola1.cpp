// \file Parabola1.cpp
#include "Parabola1.h"
#include <math.h>

Flt Parabola1::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
    // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
    pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
    // Se cargan las variables
    X = (Variable *) (*pConjVariables)[0];
    Y = (Variable *) (*pConjVariables)[1];
    Z = (Variable *) (*pConjVariables)[2];
    errTodasMuestras = 0.;
    Flt num = .0;
    for(x = a; x <= b; x += inc)
        for(y = a; y <= b; y += inc)
            for(z = a; z <= b; z += inc)
            {
                (*X) = x; (*Y) = y; (*Z) = z;
                valEvaluado = pilaEvaluacion->evalua();
                errTodasMuestras += fabs(valEvaluado - x*x - y*y - z*z);
                num++;
            }
    delete pilaEvaluacion;
    return errTodasMuestras/num;
}