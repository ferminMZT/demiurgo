flt Parabola1::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new piposeco(pInd->getArbol(), pInd->getNumNodos());
 // Se cargan las variables
 X = (variable *) (*pConjVariables)[0];
 Y = (variable *) (*pConjVariables)[1];
 Z = (variable *) (*pConjVariables)[2];
 errTodasMuestras = 0.;
 flt num = .0;
 for(x = a; x <= b; x += inc)
  for(y = a; y <= b; y += inc)
   for(z = a; z <= b; z += inc)
   {
    (*X) = x; (*Y) = y; (*Z) = z;
    valEvaluado = pilaEvaluacion->evalua();
    errTodasMuestras += fabs(valEvaluado - x*x - y*y - z*z);
    num++;
   }
 delete pilaEvaluacion;
 return errTodasMuestras/num;
}