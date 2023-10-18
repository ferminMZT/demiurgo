// Problema 3
// Rosenbrock___________________________________________________________________
// La funcion en forma de silla de montar (saddle) de ese sujeto:
// 100. * (x^2 - y^2) + (1. - x)^2              x, y en [-2.048, +2.048]

#ifndef ROSENBROCK_H
#define ROSENBROCK_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Rosenbrock : public Problema
{
 public:

    Rosenbrock()
    {
        nombre = "Silla de montar de Rosenbrock: 100. * (x^2 - y^2) + (1. - x)^2 con x, y en [-2.048, +2.048]";
        nPtos = 50;
        a     = -2.048;
        b     = +2.048;
        inc   = (b - a) / nPtos;
    }

    ~Rosenbrock()
    {
        nombre = 0;
        nPtos          = 0;
        pilaEvaluacion = 0;
    }

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return .0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
    { return .0; }


  private:

    int nPtos;			// Numero de puntos para x, y en [-2.048, +2.048]
    // Por otra parte, pongo aqui unas variables auxiliares para el calculo del
    // fitness para no tener que crearlas y ganar una poca velocidad mas... ?
    Variable *X, *Y;
    Flt       x, y;
    Flt       errTodasMuestras;
    Flt       a, b;
    Flt       inc;
    int       n;
    Flt       valEvaluado;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif // ROSENBROCK_H
