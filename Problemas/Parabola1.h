// Problema 2
// Parabola1____________________________________________________________________
// sumatorio(i=1,3) (x(i)*x(i)  con x(i) en [-5.12, +5.12]
// Esto lo entiendo como x*x + y*y + z*z en ese intervalo

#ifndef PARABOLA1_H
#define PARABOLA1_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"

class Parabola1 : public Problema
{
 public:

    Parabola1()
    {
        nombre = "La hiperparabola x^2 + y^2 + z^2 con x, y, z en [-5.12, +5.12]";
        nPtos = 10;
        a     = -5.12;
        b     = +5.12;
        inc   = (b - a) / nPtos;
    }

    ~Parabola1()
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

    int       nPtos; 		// Numero de puntos para x, y, z en [-5.12, +5.12]
    // Por otra parte, pongo aqui unas variables auxiliares para el calculo del
    // fitness para no tener que crearlas y ganar una poca velocidad mas... ?
    Variable *X, *Y, *Z;
    Flt       x, y, z;
    Flt       errTodasMuestras;
    Flt       a, b;
    Flt       inc;
    int       n;
    Flt       valEvaluado;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif // PARABOLA1_H
