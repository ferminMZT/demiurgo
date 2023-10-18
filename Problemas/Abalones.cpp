#include "Abalones.h"
#include <math.h>

Flt Abalones::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
    // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
    pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
    errTodasMuestras = 0.;
    // Se procesan todos los ejemplos
    for(ne = 0; ne < numEjemplos; ne++)
    {
        // Obtengo el valor real de la clasificacion
        valClase = batAbalones->getClase(ne);
        // Se cargan las variables con los valores de los atributos
        for(na = 0; na < numAtributos; na++)
        {
            var = (Variable *)(*pConjVariables)[na];
            (*var) = batAbalones->getAtributo(ne, na);
        }
        // Y tras cargar todas las variables (implicadas o no), evaluo al individuo
        valEvaluado = pilaEvaluacion->evalua();
        // Aqui no hay wrapper, ya que trato el problema como una regresion simbolica.
        // Me limito a devolver la diferencia entre la edad obtenida y la real.
        errTodasMuestras += fabs(valEvaluado - valClase);
    }
    delete pilaEvaluacion;
    return errTodasMuestras;
}
