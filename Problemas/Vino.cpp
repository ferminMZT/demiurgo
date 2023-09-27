flt Vino::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new piposeco(pInd->getArbol(), pInd->getNumNodos());
 errTodasMuestras = 0.;
 // Se procesan todos los ejemplos
 for(ne = 0; ne < numEjemplos; ne++)
 {
  // Obtengo el valor real de la clasificacion
  valClase = batVino->getClase(ne);
  // Se cargan las variables con los valores de los atributos
  for(na = 0; na < numAtributos; na++)
  {
   var    = (variable *)(*pConjVariables)[na];
   (*var) = batVino->getAtributo(ne, na);
  }
  // Y tras cargar todas las variables (implicadas o no), evaluo al individuo
  valEvaluado = pilaEvaluacion->evalua();
  // El wrapper para clasificar las multiples salidas posibles en 3 intervalos, correspondientes a cada clase
  if(valEvaluado > 30. || valEvaluado < -30.)
   valEvaluado = .0;
  else
   if(valEvaluado > 10.)
    valEvaluado = 3.;
   else
    if(valEvaluado < -10.)
     valEvaluado = 2.;
    else
     valEvaluado = 1.;
  if(valEvaluado != valClase)
   errTodasMuestras++;
 }
 delete pilaEvaluacion;
 return errTodasMuestras;
}