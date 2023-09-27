Gudermann::Gudermann()
{
 nombre = "La extraña funcion de Gudermann: log10[1./cos(x) + tan(x)] con x en (-M_PI/2, +M_PI/2)";
 numPuntos = 250;
 valX    = new flt[numPuntos];
 valG    = new flt[numPuntos];
 flt i = (M_PI/4.) / numPuntos;
 flt x = (-M_PI/2.) + i;

 for(n = 0; n < numPuntos; n++)
 {
  valX[n] = x;
  valG[n] = log10(1./cos(x) + sin(x)/cos(x));
  x += i;
 }
}

flt Gudermann::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
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
  errTodasMuestras += fabs(valEvaluado - valG[n]);
 }
 delete pilaEvaluacion;
 return errTodasMuestras;
}