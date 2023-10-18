// \file Monjes1.cpp

#include "Monjes1.h"

Flt Monjes1::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
    // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
    pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
    errTodasMuestras = 0.;
    // Se procesan todos los ejemplos
    for(ne = 0; ne < numEjemplos; ne++)
    {
        // Obtengo el valor real de la clasificacion
        valClase = batMonjes1->getClase(ne);
        // Se cargan las variables con los valores de los atributos
        for(na = 0; na < numAtributos; na++)
        {
            var    = (Variable *)(*pConjVariables)[na];
            (*var) = batMonjes1->getAtributo(ne, na);
        }
        // Y tras cargar todas las variables (implicadas o no), evaluo al individuo
        valEvaluado = pilaEvaluacion->evalua();
        // El wrapper para clasificar las multiples salidas posibles en 2 intervalos, correspondientes a cada clase
        valEvaluado = (valEvaluado >= 0) ? .0 : 1.;
        if(valEvaluado != valClase)
        errTodasMuestras++;
    }
    delete pilaEvaluacion;
    return errTodasMuestras;
}