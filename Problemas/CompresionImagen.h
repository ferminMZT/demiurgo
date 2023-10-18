/**
 * \file CompresionImagen.h
 * Problema 11:
 * Tenemos una imagen de 25*25 y la queremos codificar en una expresion.
 * Es una regresion simbolica un tanto extra�a, pero puede ser espectacular.
 */

#ifndef COMPRESIONIMAGEN_H
#define COMPRESIONIMAGEN_H

#include "Problema.h"
#include "../Variable.h"
#include "../Piposeco.h"


class CompresionImagen : public Problema
{
 public:

    CompresionImagen();
    ~CompresionImagen();

    Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
    Flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
    { return 0.0; }
    Flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, const char *nomFichTest)
    { return 0.0; }


 private:

    int       x, y;               // Indices de la imagen
    int       xTam, yTam;		// Tama�o de la imagen;
    Flt       xTamInv, yTamInv;	// Inversas
    Variable *X, *Y;		// Variables para cargar individuos
    Byte    **pixels;		// Los atributos, datos de entrada (que se usan para clasificar...)
    Flt       errTotalImagen;	// Error acumulado al descomprimir toda la imagen
    Flt       valEvaluado;	// Valor tras evaluar un pixel
    Flt       invNumPixels;	// 1/numero total de pixels
    Piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif // COMPRESIONIMAGEN_H
