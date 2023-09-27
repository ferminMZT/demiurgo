/**
  Espiral.cpp
  Implementacion de la clase Espiral, para la resolucion del problema de las
  espirales entrelazadas
*/

#include <math.h>

#include "Espiral.h"

Espiral::Espiral()
{
 nombre = "Las espirales inmortales";
 numPuntos = 194;
 valX    = new Flt[numPuntos];
 valY    = new Flt[numPuntos];
 valClas = new Flt[numPuntos];

 Flt x, y, angulo, radio;
 Flt densidad = 1.;
 int indPunto = 0;
 for(n = 0; n < numPuntos/2; n++)
 {
  angulo = (n * (Flt)M_PI) / (16.0f * densidad);
  radio  = 6.5f * ( (104.f * densidad) - n) / (104.f * densidad);
  x = radio*(Flt)cos(angulo);
  y = radio*(Flt)sin(angulo);
  valX[indPunto]    = x;
  valY[indPunto]    = y;
  valClas[indPunto] = -1.;
  indPunto++;
  valX[indPunto]    = -x;
  valY[indPunto]    = -y;
  valClas[indPunto] = +1.;
  indPunto++;
 }
}

Flt Espiral::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
 // Se cargan las variables
 X = (Variable *) (*pConjVariables)[0];
 Y = (Variable *) (*pConjVariables)[1];
 errTodasMuestras = 0.;
 for(n = 0; n < numPuntos; n++)
 {
  (*X) = valX[n];
  (*Y) = valY[n];
  valEvaluado = pilaEvaluacion->evalua();
  valEvaluado = (Flt) ((valEvaluado < 0.0) ? -1. : +1.);
  if(valEvaluado != valClas[n])
   errTodasMuestras++;
 }
 delete pilaEvaluacion;
 return errTodasMuestras;
}

Flt Espiral::distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
{
 Flt val1, val2;
 Flt dist = 0.0;

 X = (Variable *) (*pConjVariables)[0];
 Y = (Variable *) (*pConjVariables)[1];
 for(n = 0; n < numPuntos; n++)
 {
  (*X) = valX[n];
  (*Y) = valY[n];
  val1 = pInd1->evalua();
  val2 = pInd2->evalua();
  val1 = (Flt) ((val1 < 0.0) ? -1. : +1.);
  val2 = (Flt) ((val2 < 0.0) ? -1. : +1.);
  if(val1 != val2)
  {
   if(val1 != valClas[n]) 	// Solo me interesa comparar en los puntos en los que el individuo 1 falla
   {
    if(val2 == valClas[n])	// El primero falla, pero el segundo acierta
     dist += 10.0;
   }
   else
    dist += 1.0;
  }
 }
 return dist;
}
