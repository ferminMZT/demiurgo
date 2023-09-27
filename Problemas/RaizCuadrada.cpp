RaizCuadrada::RaizCuadrada()
{
 nombre = "La raiz cuadrada (sqrt) en [0, 1]";
 numPuntos = 100;
 valX    = new flt[numPuntos];
 valY    = new flt[numPuntos];
 flt i = 1. / numPuntos;
 flt x = 0.;

 for(n = 0; n < numPuntos; n++)
 {
  valX[n] = x;
  valY[n] = sqrt(x);
  x += i;
 }
}

flt RaizCuadrada::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new piposeco(pInd->getArbol(), pInd->getNumNodos());
 // Se cargan las variables
 X = (variable *) (*pConjVariables)[0];
 errTodasMuestras = 0.;
 for(n = 0; n < numPuntos; n++)
 {
  (*X) = valX[n];
  valEvaluado = pilaEvaluacion->evalua();
  errTodasMuestras += fabs(valEvaluado - valY[n]);
 }
 delete pilaEvaluacion;
 return errTodasMuestras;
}