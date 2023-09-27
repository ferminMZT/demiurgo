flt Lirio::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
 pilaEvaluacion = new piposeco(pInd->getArbol(), pInd->getNumNodos());
 errTodasMuestras = 0.;
 // Se procesan todos los ejemplos
 for(ne = 0; ne < numEjemplos; ne++)
 {
  // Obtengo el valor real de la clasificacion
  valClase = batLirio->getClase(ne);
  // Se cargan las variables con los valores de los atributos
  for(na = 0; na < numAtributos; na++)
  {
   var    = (variable *)(*pConjVariables)[na];
   (*var) = batLirio->getAtributo(ne, na);
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

/*
 testeaProblema: produce el % de error cometido al evaluar el problema tratado
 para el individuo dado, que usa las variables del conjunto y toma los valores
 de los ejemplos del fichero dado, que suele ser un fichero de test, distinto
 de los ficheros de entrenamiento habituales (pero que debe tener el mismo
 formato).
 Por ejemplo se entrena con lirios.trn con 50 ejemplos y se le pasa finalmente
 un fichero con 150 ejemplos. Cuantos mas se resuelvan correctamente, menor
 sera el % de error producido... Recordar que esto es una clasificacion
*/

flt Lirio::testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest)
{
 // Se crea una nueva bateria de ejemplos a partir del nuevo fichero de test
 bateria batTest(nomFichTest);
 // El error cometido al evaluar los nuevos ejemplos
 flt errTest = 0;

 // Y ahora para cambiar el menor codigo posible, vamos a intercambiar los datos
 // nuevos por los antiguos de test para asi aprovechar la funcion de fitness,
 // que volveremos a evaluar con los nuevos datos...

 // Cambiamos la bateria y el numero de ejemplos, salvando los anteriores
 // Ojo con otros problemas la variable de la bateria no sera batLirio!!!
 bateria *antBateria = batLirio;
 int      numEjemAnt = numEjemplos;

 batLirio    = &batTest;
 numEjemplos = batLirio->getNumEjemplos();

 // Y ya solo queda evaluar al individuo con estos nuevos datos
 // y aprovechando la funcion de fitness que ya teniamos
 errTest = fitness(pInd, pConjVariables);
 // Y ese error que esta en el entorno [0, numEjemplos], lo pasamos a [0, 100]
 errTest = (errTest * 100.) / numEjemplos;

 // Dejamos las cosas como estaban (por si se quisiera continuar la evolucion...)
 batLirio    = antBateria;
 numEjemplos = numEjemAnt;

 return errTest;
 // Y el destructor de la bateria recien creada se llamaria automaticamente
}