/**
  KozaRegresionSimbolica1.cpp
  Implementacion de la clase para la resolucion de la regresion simbolica que
  aparece en primer lugar en [Koz92]
*/

#include "KozaRegresionSimbolica1.h"

Flt KozaRegresionSimbolica1::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
 // Se cargan las variables
 X = (Variable *) (*pConjVariables)[0];
 errTodasMuestras = 0.;

 a = (Flt) 0.0001;
 b = 10;
 inc = (b - a)/nPtos;
 for(x = a; x <= b; x += inc)
 {
  (*X) = x;
  valEvaluado = pilaEvaluacion->evalua();
  //errTodasMuestras += fabs(valEvaluado - x*x*x*x - x*x*x - x*x - x);
  //errTodasMuestras += (Flt) fabs(valEvaluado - (x + 1.)*(x + 1.)*(x + 1.));
  errTodasMuestras += (Flt) fabs(valEvaluado - sqrt(2/(M_PI*x))*cos(x));
 }
 delete pilaEvaluacion;
 return errTodasMuestras/(Flt)(nPtos);
}

/*
 testeaProblema: produce el % de error cometido al evaluar el problema tratado
 para el individuo dado, que usa las variables del conjunto y toma una serie
 de nuevos valores que se le dan en esta funcion (el fichero no se suele usar
 para los problemas de regresion, pues es mas facil meter los valores "a mano"
 por medio de un bucle, como aqui hago yo). En este ejemplo se entrena con un
 fitness de 20 puntos y finalmente se pasa un test con 2000 puntos en ese
 intervalo. La salida es el rms, por tanto cuanto menor sea mejor!!!
 Recordar que esto es una regresion simbolica!!!
*/

Flt KozaRegresionSimbolica1::testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest)
{
 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
 // Se cargan las variables
 X = (Variable *) (*pConjVariables)[0];
 errTodasMuestras = 0.;
 Flt numeroPuntos = 2000.;
 a = (Flt) 0.0001;
 b = 10;
 Flt incremento = (b - a) / numeroPuntos;
 for(x = a; x <= b; x += incremento)
 {
  (*X) = x;
  valEvaluado = pilaEvaluacion->evalua();
  //errTodasMuestras += fabs(valEvaluado - x*x*x*x - x*x*x - x*x - x);
  //errTodasMuestras += (Flt) fabs(valEvaluado - (x + 1.)*(x + 1.)*(x + 1.));
  errTodasMuestras += (Flt) fabs(valEvaluado - sqrt(2/(M_PI*x))*cos(x));
 }
 delete pilaEvaluacion;
 return errTodasMuestras/numeroPuntos;
}
