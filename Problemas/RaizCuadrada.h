// \file RaizCuadrada.h
// Problema 13
// RaizCuadrada_________________________________________________________________
// Queremos aproximar la raiz cuadrada en [0, 1]

#ifndef RAIZCUADRADA_H
#define RAIZCUADRADA_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"

class RaizCuadrada : public Problema
{

 public:

    RaizCuadrada();
    ~RaizCuadrada()
    {
        delete [] valX;
        delete [] valY;
        nombre 	  = 0;
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
    Flt *valY;                    // Valor de la Gudermanniana

    // Por otra parte, pongo aqui unas variables auxiliares para el calculo del
    // fitness para no tener que crearlas y ganar una poca velocidad mas... ?
    Variable *X;
    Flt       errTodasMuestras;
    int       n;
    Flt       valEvaluado;
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif // RAIZCUADRADA_H