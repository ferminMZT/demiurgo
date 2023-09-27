/**
  Poblacion.cpp
  Implementacion de la clase Poblacion
*/


#include "Poblacion.h"


// Destructor off-line por el bucle; OJO: tambien destruye a los habitantes
Poblacion::~Poblacion()
{
 numVariables  = numOperaciones  = 0;
 conjVariables = conjOperaciones = 0;
 if(numIndividuos)				// Ya se habia creado una poblacion
 {
  for(int n = 0; n < numIndividuos; n++)	// Borra cada miembro de la poblacion
   delete (*conjIndividuos)[n];
  delete conjIndividuos;		        // Y borra los propios conjuntos
  numIndividuos = 0;
 }
 //ptrFuncFitness = 0;				// Desconecta funcion externa
 elProblema =  0;
 semilla    = -1;
}

// Creacion de una poblacion de nInd individuos aleatorios.
// Usa la tecnica conocida como enriquecimiento inicial del sistema, consistente en crear una
// enorme poblacion inicial, FACT_ENRIQ veces superior a lo previsto y nos quedamos solo
// con los mejores. Parece que mejora la de dios la cosa...
// Como paso PREVIO OBLIGATORIO, hay que CONECTAR LA FUNCION DE ERROR EXTERNA!!!
// La mejoro utilizando lo que denomino SUPRA-ENRIQUECIMIENTO INCREMENTAL:
// Por ejemplo, para una poblacion de 1500 individuos, con factor de enriquecimiento
// de 2 y tamaño de elite de 100 individuos; se crean 1500/100 = 15 poblaciones
// gigantes de 1500 * 2 = 3000 individuos, de los cuales nos quedamos con los 100
// mejores, hasta llegar a completar los 1500 que nos hacen falta.
void Poblacion::creaPoblacionInicial(int nInd, int fEnr, int nElite)
{
 // Esto es lo que primero se debe hacer para NO SER UN BURRRRRU!!! (3 semanas por este puto fallo, MCD!!!)
 numIndividuos = nInd;
 factorEnriq   = fEnr;
 eliteEnriq    = nElite;
 // Reserva espacio para los miembros de la poblacion y para su error individual
 conjIndividuos = new Conjunto<ptrIndividuo>(numIndividuos);
 // Aprovecho para conectar al conjunto de individuos de la poblacion la funcion externa de ordenacion,
 // para evitar tener que hacerlo en el futuro y poder realizar ordenaciones directamente sin mas...
 conjIndividuos->conectaFuncionOrdenacion(fExtOrdenInd);

 // Inicialmente creo un conjunto grande inicial de cardinal numIndividuos*FACT_ENRIQ; calculo el error de cada
 // miembro y segun este, los ordeno crecientemente.
 // Y de cada poblacion gigante me quedo con los nElite individuos primeros!!!
 int tGrande = numIndividuos * factorEnriq;				// Tamaño del conjunto grande
 Conjunto<ptrIndividuo> *cGrande = new Conjunto<ptrIndividuo>(tGrande);	// El conjunto grande de individuos
 int nIndCreados;	// Individuos ya creados
 int n;                 // Indice para individuos

 // Conecto la funcion de ordenacion para la poblacion gigante del conjunto grande
 cGrande->conectaFuncionOrdenacion(fExtOrdenInd);
 // Crea la poblacion suprarrica a partir de unas elites que saco de varias poblaciones grandes
 for(nIndCreados = 0; nIndCreados < numIndividuos; nIndCreados += eliteEnriq)
 {
  // Creo los individuos de la poblacion grande aleatoriamente y calculo su error
  for(n = 0; n < tGrande; n++)
  {
   (*cGrande)[n] = new Individuo(*conjVariables, *conjOperaciones);
   (*cGrande)[n]->setError(elProblema->fitness((*cGrande)[n], conjVariables));
  }
  // Ahora habria que ordenar esa poblacion grande inicial crecientemente segun el error individual
  cGrande->ordena();
  // Tras la ordenacion me quedo solo con una elite dada por eliteEnriq
  // Traslado a la elite a la poblacion que sobrevivira
  for(n = 0; n < eliteEnriq; n++)
  {
   (*conjIndividuos)[nIndCreados + n] = new Individuo( *((*cGrande)[n]) );
   //(*conjIndividuos)[nIndCreados + n]->asignaPila();
  }
  cout << (nIndCreados + n) << "/" << numIndividuos << endl;
  // Y destruyo la chusma de la poblacion gigante ENTERA, ya que la elite ya ha sido copiada
  for(n = 0; n < tGrande; n++)
   delete (*cGrande)[n];
 }
 // Tras esto destruyo la poblacion gigante
 delete cGrande;
 // Ordeno la poblacion final y calculo su error total medio
 conjIndividuos->ordena();
 errorMedioPoblacion = .0;
 for(n = 0; n < numIndividuos; n++)
  errorMedioPoblacion += (*conjIndividuos)[n]->getError();
 errorMedioPoblacion /= numIndividuos;
 posMejor = 0;		// Obviamente!!!
}

// Actualiza los parametros globales de la poblacion (errMedioPoblacion, numMedioNodos y numMedioIntrones) a partir
// de los correspondientes parametros (ya calculados) de cada individuo
void Poblacion::actualizaParametros()
{
 errorMedioPoblacion = numMedioNodos = numMedioIntrones = .0;
 for(int n = 0; n < numIndividuos; n++)
 {
  errorMedioPoblacion += (*conjIndividuos)[n]->getError();
  numMedioNodos       += (*conjIndividuos)[n]->getNumNodos();
  numMedioIntrones    += (*conjIndividuos)[n]->getNumIntrones();
 }
 errorMedioPoblacion /= numIndividuos;
 numMedioNodos       /= numIndividuos;
 numMedioIntrones    /= numIndividuos;
}

// Completa una sola generacion y actualiza todos los parametros de la poblacion
// Cada pareja padre-madre democraticamente escogidos al azar, produce un hijo (similar al padre)
// Saldria antes si algun individuo de los creados tuviera un error aceptable
// Uso la estrategia STEADY-STATE: los mejores se quedan y los peores fenecen
int Poblacion::generacionA()
{
 int        numIteraciones = 0;	// Numero de iteraciones para llevar a cabo la generacion
 int        numNuevos = 0;	// Numero de nuevos individuos creados en la generacion
 Individuo *padre,   *madre;	// Los progenitores
 Individuo *hijo;    		// El retoño, mayormente parecido a su popo
 int        posPadre, posMadre;	// Posiciones ocupadas por los padres en la poblacion
 Flt        errPadre, errMadre;	// Los errores de los padres
 Flt        errHijo;		// El error del fiu

 while(numIteraciones != numIndividuos && !errorAlcanzado())
 {
  //cout << numIteraciones << "\b\r";
  // Saco 2 bolitas con los numeros de los padres
  posPadre = RND(numIndividuos);
  posMadre = RND(numIndividuos);
  // Esto da problemas por un doble delete, pero podia usarse para mutar en ese caso prob=1/tam^2
  if(posPadre == posMadre)
    continue;
  // Obtengo los errores de padre y madre
  errPadre = (*conjIndividuos)[posPadre]->getError();
  errMadre = (*conjIndividuos)[posMadre]->getError();
  // Para facilitar las cosas, hare que el padre sea el de menor error de ambos
  if(errPadre > errMadre)		// SOLO hay que intercambiar sus indices y errores
  {
   intercambia(posPadre, posMadre);
   intercambia(errPadre, errMadre);
  }
  // Obtengo a ambos progenitores y sus errores respectivos
  padre = (*conjIndividuos)[posPadre];
  madre = (*conjIndividuos)[posMadre];

  // A veces, el hijo surgira espontaneamente como una mutacion de su padre, pero la mayoria de las veces
  // es el fruto de un cruce

//  if(RND(100) < TPC_MUTACION)		// Mutatio habemus!!!
//   hijo = padre->mutacion(*conjVariables, *conjOperaciones);
//  else  			// Tras el "ayuntamiento carnal con fembra placentera" viene el PARTO
   hijo = padre->cruce(*madre);

  // En este momento tenemos 3 individuos para 2 plazas: se quedan los mejores!
  // Para ello hay que evaluar los pecados (error) del hijo, pues los de los padres ya
  // eran conocidos con anterioridad. Uso para ello la funcion externa de error
  //hijo->setError(ptrFuncFitness(hijo, conjVariables));
  hijo->setError(elProblema->fitness(hijo, conjVariables));
  errHijo = hijo->getError();
  // Enfrento a 2 generaciones: El padre es mejor que la madre, veamos si el hijo esta a la altura...
  // NAZISMO: Se quedan los 2 individuos de menor error y se extermina al defectuoso.
  if(errHijo < errMadre)	// Si el hijo es mejor que la madre: hay que matarla
  {				// errHijo < errPadre < errMadre
   delete madre;		// Se mata a la madre
   // En este punto, hay que comprobar si el hijo es mejor que el padre:
   if(errHijo < errPadre)	// El hijo es el mejor
   {
    (*conjIndividuos)[posPadre] = hijo;		// El hijo se convierte en viril padre
    (*conjIndividuos)[posMadre] = padre;	// y el padre se afemina y hace madre
    // Aqui queda comprobar si el hijo es mejor que el mejor individuo...
    if(errHijo < (*conjIndividuos)[posMejor]->getError())
     posMejor = posPadre;
   }
   else				// El padre sigue siendo el mejor (de esa familia)
    (*conjIndividuos)[posMadre] = hijo;		// Y el hijo pasa a ser la madre
   // En todo caso, se ha creado 1 nuevo individuo: el hijo
   numNuevos++;
  }
  else		// El vastago es tan rematadamente malo que no merece vivir: apologia abortiva
   delete hijo;
  numIteraciones++;
 }
 numGeneraciones++;
 //cout << " [Nuevos: " << numNuevos << " (" << 100.*numNuevos/numIndividuos << "%)] " << endl;
 return numNuevos;
}

// La misma que antes, pero con intrones...
// Metodo padre-madre-hijo democratico.
int Poblacion::generacionAI()
{
 int        numIteraciones = 0;	// Numero de iteraciones para llevar a cabo la generacion
 int        numNuevos      = 0;	// Numero de nuevos individuos creados en la generacion
 int        numMutados     = 0; // Numero total de mutaciones en esta generacion
 int        numMutadosOK   = 0; // Numero de mutaciones aceptadas
 int        hayMutacion;	// Para saber cuando se da una mutacion
 Individuo *padre,   *madre;	// Los progenitores
 Individuo *hijo;    		// El retoño, mayormente parecido a su popo
 int        posPadre, posMadre;	// Posiciones ocupadas por los padres en la poblacion
 Flt        errPadre, errMadre;	// Los errores de los padres
 Flt        errHijo;		// El error del fiu
 // Para los 3 posibles intrones susceptibles de manejar (1 padre + 1 madre + 1 hijo)
 ptrArbol   pAPHijo, pAMHijo, pArbHijo;
 // Para saber si ha habido un cruce de la parte GP (no GA) y por tanto se manejan los intrones
 int        usaIntron;

 // En cada generacion se intenta rellenar la poblacion entera...
 while(numIteraciones != numIndividuos && !errorAlcanzado())
 {
  // Resetea los punteros para los intrones
  pAPHijo = pAMHijo = pArbHijo = 0;
  // Saco 2 bolitas con los numeros de los padres
  posPadre = RND(numIndividuos);
  posMadre = RND(numIndividuos);
  // Esto da problemas por un doble delete, pero podia usarse para mutar en ese caso prob=1/tam^2
  if(posPadre == posMadre)
    continue;
  // Obtengo los errores de padre y madre
  errPadre = (*conjIndividuos)[posPadre]->getError();
  errMadre = (*conjIndividuos)[posMadre]->getError();
  // Para facilitar las cosas, hare que el padre sea el de menor error de ambos
  if(errPadre > errMadre)		// SOLO hay que intercambiar sus indices y errores
  {
   intercambia(posPadre, posMadre);
   intercambia(errPadre, errMadre);
  }
  // Obtengo a ambos progenitores y sus errores respectivos
  padre = (*conjIndividuos)[posPadre];
  madre = (*conjIndividuos)[posMadre];

  // A veces, el hijo surgira espontaneamente como una mutacion de su padre, pero la mayoria de las veces
  // es el fruto de un cruce

  if(RND(100) < TPC_MUTACION)		// Mutatio habemus!!!
  {
   hayMutacion = VERDAD;	// Es cierto, es una linda mutacion!!!
   usaIntron   = FALSO;		// Y no se usa el intron
   hijo = padre->mutacion(*conjVariables, *conjOperaciones);
   numMutados++;
  }
  else
  {  			// Tras el "ayuntamiento carnal con fembra placentera" viene el PARTO
   hijo = padre->cruceIntron(*madre, pAPHijo, pAMHijo, pArbHijo);
   usaIntron = (pArbHijo) ? VERDAD : FALSO;
   hayMutacion = FALSO;
  }
  // En este momento tenemos 3 individuos para 2 plazas: se quedan los mejores!
  // Para ello hay que evaluar los pecados (error) del hijo, pues los de los padres ya
  // eran conocidos con anterioridad. Uso para ello la funcion externa de error
  //hijo->setError(ptrFuncFitness(hijo, conjVariables));
  hijo->setError(elProblema->fitness(hijo, conjVariables));
  errHijo = hijo->getError();
  // Enfrento a 2 generaciones: El padre es mejor que la madre, veamos si el hijo esta a la altura...
  // NAZISMO: Se quedan los 2 individuos de menor error y se extermina al defectuoso.
  if(errHijo < errMadre)	// Si el hijo es mejor que la madre: hay que matarla
  {				// errHijo < errPadre < errMadre
   delete madre;		// Se mata a la madre

   // Puesto que se usan intrones y el hijo ha sobrevivido, se incrementa su intron propio, pero no el materno
   // ya que la madre ha sido ejecutada
   if(usaIntron)
    hijo->incIntron(pArbHijo);

   // En este punto, hay que comprobar si el hijo es mejor que el padre:
   if(errHijo < errPadre)	// El hijo es el mejor
   {
    (*conjIndividuos)[posPadre] = hijo;		// El hijo se convierte en viril padre
    (*conjIndividuos)[posMadre] = padre;	// y el padre se afemina y hace madre
    // Y como el hijo es mejor que su padre, esto hay que celebrarlo, incrementando su intron
    if(usaIntron)
     hijo->incIntron(pArbHijo);

    // Aqui queda comprobar si el hijo es mejor que el mejor individuo...
    if(errHijo < (*conjIndividuos)[posMejor]->getError())
    {
     posMejor = posPadre;
     if(usaIntron)		// Sumo 5 al intron del mejor individuo
     {
      hijo->incIntron(pArbHijo);
      hijo->incIntron(pArbHijo);
      hijo->incIntron(pArbHijo);
      hijo->incIntron(pArbHijo);
      hijo->incIntron(pArbHijo);
     }
    }
   }
   else				// El padre sigue siendo el mejor (de esa familia)
    (*conjIndividuos)[posMadre] = hijo;		// Y el hijo pasa a ser la madre
   // En todo caso, se ha creado 1 nuevo individuo: el hijo
   numNuevos++;
  }
  else
  {
   delete hijo;                 // El vastago es tan rematadamente malo que no merece vivir: apologia abortiva
   if(usaIntron)		// Y como el cistron materno es malo, hay que constatarlo sobre ese progenitor
    madre->decIntron(pAMHijo);
  }
  numIteraciones++;
 }
 numGeneraciones++;
 //cout << " [Nuevos: " << numNuevos << " (" << 100.*numNuevos/numIndividuos << "%)] " << endl;
 return numNuevos;
}

// La pareja padre-madre (aleatoriamente escogidos), produce un hijo (similar al padre)
// asi como una hija (similar a la madre).
int Poblacion::generacionB()
{
 int        numIteraciones = 0;	// Numero de iteraciones para llevar a cabo la generacion
 int        numNuevos      = 0;	// Numero de nuevos individuos creados en la generacion
 int        numMutados     = 0; // Numero total de mutaciones en esta generacion
 int        numMutadosOK   = 0; // Numero de mutaciones aceptadas
 int        hayMutacion;	// Para saber cuando se da una mutacion
 Individuo *padre,   *madre;	// Los progenitores
 Individuo *hijo;    		// El retoño, mayormente parecido a su popo
 Individuo *hija;    		// La retoña, mayormente parecida a su momo
 int        posPadre, posMadre;	// Posiciones ocupadas por los padres en la poblacion
 Flt        errPadre, errMadre;	// Los errores de los padres
 Flt        errHijo,  errHija;	// Errores de los fios

 // En cada generacion se intenta rellenar la poblacion entera...
 while(numIteraciones != numIndividuos /*&& !errorAlcanzado()*/)
 {
  // Saco 2 bolitas con los numeros de los padres
  posPadre = RND(numIndividuos);
  posMadre = RND(numIndividuos);
  // Esto da problemas por un doble delete, pero podia usarse para mutar en ese caso prob=1/tam^2
  if(posPadre == posMadre)
    continue;
  // Obtengo los errores de padre y madre
  errPadre = (*conjIndividuos)[posPadre]->getError();
  errMadre = (*conjIndividuos)[posMadre]->getError();
  // Para facilitar las cosas, hare que el padre sea el de menor error de ambos
  if(errPadre > errMadre)		// SOLO hay que intercambiar sus indices y errores
  {
   intercambia(posPadre, posMadre);
   intercambia(errPadre, errMadre);
  }
  // Obtengo a ambos progenitores y sus errores respectivos
  padre = (*conjIndividuos)[posPadre];
  madre = (*conjIndividuos)[posMadre];

  // A veces, los hijos surgen como una mutacion de sus padres, pero mayormente son el fruto de un cruce
  if(RND(100) < TPC_MUTACION)	// Mutatio habemus!!!
  {                             // Me parecio ver una linda mutacion
   hayMutacion = VERDAD;	// Es cierto, es una linda mutacion!!!
   hijo = padre->mutacion(*conjVariables, *conjOperaciones);
   hija = madre->mutacion(*conjVariables, *conjOperaciones);
   numMutados += 2;
  }
  else
  {
   // Tras consumar el "ayuntamiento carnal con fembra placentera" viene el PARTO de ambos retoños
   hijo = padre->cruce(*madre);
   hija = madre->cruce(*padre);
   hayMutacion = FALSO;
  }
  // En este momento tenemos 4 individuos para 2 plazas: se quedan los mejores!
  // Evaluo los errores de los hijos
  hijo->setError(elProblema->fitness(hijo, conjVariables));
  errHijo = hijo->getError();
  //hija->setError(ptrFuncFitness(hija, conjVariables));
  hija->setError(elProblema->fitness(hija, conjVariables));
  errHija = hija->getError();
  // Hago que el hijo sea mejor que la hija.
  // No es machismo, sino un convenio (y/o una arcaica forma de vivir?)
  if(errHijo > errHija)
  {
   intercambia(hijo, hija);		// Intercambia punteros a los objetos
   intercambia(errHijo, errHija);	// ademas de los correspondientes errores
  }

  // Enfrento a 2 generaciones: El padre es mejor que la madre, y el hijo mejor que la hija
  // NAZISMO: Se quedan los 2 individuos de menor error y se extermina a los restantes...
  if(errHija < errPadre)	// Si la hija es mejor que el padre, ambos hijos se salvan
  {				// ya que errHijo < errHija < errPadre < errMadre
   delete padre;                // Cria cuervos y te sacaran los ojos!
   delete madre;		// Mata a los padres.
   (*conjIndividuos)[posPadre] = hijo;		// El hijo asume el puesto del padre (EDIPO)
   (*conjIndividuos)[posMadre] = hija;		// Y la hija el de su difunta madre (ELECTRA)
   // Podria darse el caso de que el hijo sea mejor que el mejor individuo...
   if(errHijo < (*conjIndividuos)[posMejor]->getError())
   {
    posMejor = posPadre;
    if(hayMutacion)
     cout << "!";
   }
   // Se han creado 2 nuevos individuos: ambos hijos
   numNuevos += 2;
   // Para llevar la cuenta de las mutaciones
   if(hayMutacion)
    numMutadosOK += 2;
  }
  else // Queda por tener en cuenta que a igualdad de errores, siempre debe ser preferible el individuo menor...
  {
   delete hija;                 // La hija es peor que el padre y hay que matarla
   if(errHijo < errMadre)	// Si el hijo es mejor que la madre: hay que matarla
   {				// errHijo < min(errHija, errMadre)
    delete madre;		// Se mata a la madre
    // En este punto, hay que comprobar si el hijo es mejor que el padre:
    if(errHijo < errPadre)	// El hijo es el mejor
    {
     (*conjIndividuos)[posPadre] = hijo;	// El hijo se convierte en viril padre
     (*conjIndividuos)[posMadre] = padre;	// y el padre se afemina y hace madre
     // Aqui queda comprobar si el hijo es mejor que el mejor individuo...
     if(errHijo < (*conjIndividuos)[posMejor]->getError())
     {
      posMejor = posPadre;
      if(hayMutacion)
       cout << "!";
     }
    }
    else					// El padre sigue siendo el mejor (de esa familia)
     (*conjIndividuos)[posMadre] = hijo;	// Y el hijo pasa a ser la madre
    // En todo caso, se ha creado 1 nuevo individuo: el hijo
    numNuevos++;
    // Y por si es mutado
    if(hayMutacion)
     numMutadosOK++;
   }
   else			// El vastago es tan rematadamente malo que no merece vivir: apologia abortiva
    delete hijo;        // ya que incluso era peor que su madre (errHijo > errMadre)
  }
  numIteraciones++;
 }
 numGeneraciones++;
 //cout << " [Nuevos: " << numNuevos << " (" << 100.*numNuevos/numIndividuos << "%)] " << endl;
 cout << "#Mut: " << numMutadosOK << "/" << numMutados << endl;
 return numNuevos;
}

// La pareja padre-madre (aleatoriamente escogidos), produce un hijo (similar al padre)
// asi como una hija (similar a la madre). Version con intrones.
int Poblacion::generacionBI()
{
 int        numIteraciones = 0;	// Numero de iteraciones para llevar a cabo la generacion
 int        numNuevos      = 0;	// Numero de nuevos individuos creados en la generacion
 int        numMutados     = 0; // Numero total de mutaciones en esta generacion
 int        numMutadosOK   = 0; // Numero de mutaciones aceptadas
 int        hayMutacion;	// Para saber cuando se da una mutacion
 Individuo *padre, *madre;	// Los progenitores
 Individuo *hijo;    		// El retoño, mayormente parecido a su popo
 Individuo *hija;    		// La retoña, mayormente parecida a su momo
 int        posPadre, posMadre;	// Posiciones ocupadas por los padres en la poblacion
 Flt        errPadre, errMadre;	// Los errores de los padres
 Flt        errHijo,  errHija;	// Errores de los fios
 // Para los 6 posibles intrones susceptibles de manejar (2 padre + 2 madre + 1 hijo + 1 hija)
 ptrArbol   pAPHijo, pAMHijo, pAPHija, pAMHija, pArbHijo, pArbHija;
 Individuo *pPadreHijo, *pMadreHijo, *pPadreHija, *pMadreHija;
 // Para saber si ha habido un cruce de la parte GP (no GA) y por tanto se manejan los intrones
 int        usaIntronHijo, usaIntronHija;
 // En cada generacion se intenta rellenar la poblacion entera...
 while(numIteraciones != numIndividuos /*&& !errorAlcanzado()*/)
 {
  // Resetea los punteros para los intrones
  pAPHijo = pAMHijo = pAPHija = pAMHija = pArbHijo = pArbHija = 0;
  // Saco 2 bolitas con los numeros de los padres
  posPadre = RND(numIndividuos);
  posMadre = RND(numIndividuos);
  // Esto da problemas por un doble delete, pero podia usarse para mutar en ese caso prob=1/tam^2
  if(posPadre == posMadre)
   continue;
  // Obtengo los errores de padre y madre
  errPadre = (*conjIndividuos)[posPadre]->getError();
  errMadre = (*conjIndividuos)[posMadre]->getError();
  // Para facilitar las cosas, hare que el padre sea el de menor error de ambos
  //if(errPadre > errMadre)		// SOLO hay que intercambiar sus indices y errores
  if((*conjIndividuos)[posMadre]->esMejorQue((*conjIndividuos)[posPadre]))
  {
   intercambia(posPadre, posMadre);
   intercambia(errPadre, errMadre);
  }
  // Obtengo a ambos progenitores y sus errores respectivos
  padre = (*conjIndividuos)[posPadre];
  madre = (*conjIndividuos)[posMadre];
  // El padre y la madre son reflexivos para con los hijos...
  pPadreHijo = pMadreHija = padre;
  pMadreHijo = pPadreHija = madre;
  // A veces, los hijos surgen como una mutacion de sus padres, pero mayormente son el fruto de un cruce

  if(RND(100) < TPC_MUTACION)	// Mutatio habemus!!!
  {                             // Me parecio ver una linda mutacion
   hayMutacion = VERDAD;	// Es cierto, es una linda mutacion!!!
   usaIntronHijo = usaIntronHija = FALSO;
   hijo = padre->mutacion(*conjVariables, *conjOperaciones);
   hija = madre->mutacion(*conjVariables, *conjOperaciones);
   numMutados += 2;
  }
  else
  {
   // Tras consumar el "ayuntamiento carnal con fembra placentera" viene el PARTO de ambos retoños
   hijo = padre->cruceIntron(*madre, pAPHijo, pAMHijo, pArbHijo);
   usaIntronHijo = (pArbHijo) ? VERDAD : FALSO;
   /*
   if(usaIntronHijo)
   {
    cout << "Para el hijo, Padre:" << endl;
    padre->ver();
    cout << *pAPHijo;
    cout << "Madre:" << endl;
    madre->ver();
    cout << *pAMHijo;
    cout << "Hijo:" << endl;
    hijo->ver();
    cout << *pArbHijo;
   }
   */
   hija = madre->cruceIntron(*padre, pAPHija, pAMHija, pArbHija);
   usaIntronHija = (pArbHija) ? VERDAD : FALSO;
   /*
   if(usaIntronHija)
   {
    cout << "Para la hija, Padre:" << endl;
    madre->ver();
    cout << *pAPHija;
    cout << "Madre:" << endl;
    padre->ver();
    cout << *pAMHija;
    cout << "Hija:" << endl;
    hija->ver();
    cout << *pArbHija;
   }
   */
   hayMutacion = FALSO;
  }
  // En este momento tenemos 4 individuos para 2 plazas: se quedan los mejores!
  // Evaluo los errores de los hijos
  hijo->setError(elProblema->fitness(hijo, conjVariables));
  errHijo = hijo->getError();
  hija->setError(elProblema->fitness(hija, conjVariables));
  errHija = hija->getError();
  // Hago que el hijo sea mejor que la hija.
  // No es machismo, sino un convenio (y/o una arcaica forma de vivir?)
  //if(errHijo > errHija)
  if(hija->esMejorQue(hijo))
  {
   intercambia(hijo, hija);			// Intercambia punteros a los objetos
   intercambia(errHijo, errHija);		// ademas de los correspondientes errores
   if(usaIntronHijo || usaIntronHija)
   {
    intercambia(usaIntronHijo, usaIntronHija);   // y el uso de intrones...
    intercambia(pArbHijo, pArbHija);
    intercambia(pAPHijo, pAPHija);
    intercambia(pAMHijo, pAMHija);
    intercambia(pPadreHijo, pPadreHija);
    intercambia(pMadreHijo, pMadreHija);
   }
  }
  // Enfrento a 2 generaciones: El padre es mejor que la madre, y el hijo mejor que la hija
  // NAZISMO: Se quedan los 2 individuos de menor error y se extermina a los restantes...
  if(hija->esMejorQue(padre))
  //if(errHija < errPadre)	// Si la hija es mejor que el padre, ambos hijos se salvan
  {				// ya que errHijo < errHija < errPadre < errMadre
   delete padre;                // Cria cuervos y te sacaran los ojos!
   delete madre;		// Mata a los padres.
   (*conjIndividuos)[posPadre] = hijo;		// El hijo asume el puesto del padre (EDIPO)
   (*conjIndividuos)[posMadre] = hija;		// Y la hija el de su difunta madre (ELECTRA)

   // Cuando los intrones se emplean para el cruce y los descendientes sobreviven, se incrementa su intron
   if(usaIntronHijo)
    hijo->incIntron(pArbHijo);
   if(usaIntronHija)
    hija->incIntron(pArbHija);
   // Podria darse el caso de que el hijo sea mejor que el mejor individuo...
   //if(errHijo < (*conjIndividuos)[posMejor]->getError())
   if(hijo->esMejorQue((*conjIndividuos)[posMejor]))
   {
    posMejor = posPadre;
    if(hayMutacion)
     cout << "M!";
    if(usaIntronHijo)		// Sumo 5 al intron del mejor individuo
    {
     //cout << " I! " << *pArbHijo << endl;
     hijo->incIntron(pArbHijo);
     hijo->incIntron(pArbHijo);
     hijo->incIntron(pArbHijo);
     hijo->incIntron(pArbHijo);
     hijo->incIntron(pArbHijo);
    }
   }
   // Se han creado 2 nuevos individuos: ambos hijos
   numNuevos += 2;
   // Para llevar la cuenta de las mutaciones
   if(hayMutacion)
    numMutadosOK += 2;
  }
  else // Queda por tener en cuenta que a igualdad de errores, siempre debe ser preferible el individuo menor...
  {
   delete hija;                 // La hija es peor que el padre y hay que matarla
   // Y como el cistron perteneciente a su madre es muy malo, hay que constatarlo sobre ese progenitor
   if(usaIntronHija)
    pMadreHija->decIntron(pAMHija);
   if(hijo->esMejorQue(madre))
   //if(errHijo < errMadre)	// Si el hijo es mejor que la madre: hay que matarla
   {				// errHijo < min(errHija, errMadre)
    delete madre;		// Se mata a la madre
    // Y como el hijo ha sido lo suficientemente bueno para sobrevivir, su intron debe incrementarse
    if(usaIntronHijo)
    {
     hijo->incIntron(pArbHijo);
     // Y se incrementa el intron que le dio la vida, es decir el de su madre si es que todavia vive
     if(madre != pMadreHijo)
      pMadreHijo->incIntron(pAMHijo);
    }
    // En este punto, hay que comprobar si el hijo es mejor que el padre:
    //if(errHijo < errPadre)	// El hijo es el mejor
    if(hijo->esMejorQue(padre))
    {
     (*conjIndividuos)[posPadre] = hijo;	// El hijo se convierte en viril padre
     (*conjIndividuos)[posMadre] = padre;	// y el padre se afemina y hace madre
     // Aqui queda comprobar si el hijo es mejor que el mejor individuo...
     //if(errHijo < (*conjIndividuos)[posMejor]->getError())
     if(hijo->esMejorQue((*conjIndividuos)[posMejor]))
     {
      posMejor = posPadre;
      if(hayMutacion)
       cout << "M!";
      if(usaIntronHijo)		// Sumo 5 al intron del mejor individuo
      {
       //cout << " I! " << *pArbHijo << endl;
       hijo->incIntron(pArbHijo);
       hijo->incIntron(pArbHijo);
       hijo->incIntron(pArbHijo);
       hijo->incIntron(pArbHijo);
       hijo->incIntron(pArbHijo);
       // Y se incrementa el intron que le dio la vida, es decir el de su madre si es que todavia vive
       if(madre != pMadreHijo)
       {
        pMadreHijo->incIntron(pAMHijo);
        pMadreHijo->incIntron(pAMHijo);
        pMadreHijo->incIntron(pAMHijo);
        pMadreHijo->incIntron(pAMHijo);
        pMadreHijo->incIntron(pAMHijo);
       }
      }
     }
    }
    else					// El padre sigue siendo el mejor (de esa familia)
     (*conjIndividuos)[posMadre] = hijo;	// Y el hijo pasa a ser la madre
    // En todo caso, se ha creado 1 nuevo individuo: el hijo
    numNuevos++;
    // Y por si es mutado
    if(hayMutacion)
     numMutadosOK++;
   }
   else
   {			// El vastago es tan rematadamente malo que no merece vivir: apologia abortiva
    delete hijo;        // ya que incluso era peor que su madre (errHijo > errMadre)
    // Y como su cistron materno  es muy malo, hay que constatarlo sobre ese progenitor
    if(usaIntronHijo)
    {
     pMadreHijo->decIntron(pAMHijo);
    }
   }
  }
  numIteraciones++;
 }
 numGeneraciones++;
 //cout << " [Nuevos: " << numNuevos << " (" << 100.*numNuevos/numIndividuos << "%)] " << endl;
 cout << "#Mut: " << numMutadosOK << "/" << numMutados << endl;
 return numNuevos;
}

// Este es como el democratico normal, pero con una falsa democracia, ya que cada posible
// matrimonio se realiza entre LOS MEJORES progenitores de entre los escogidos al azar.
// Produce hijo e hija, y los malos individuos no se comeran un rosco... (real como la vida)
// Aqui hay reordenacion de la poblacion, insertando los mejores y expulsando a los peores
// (aplicacion de las teorias nacional socialistas tan conocidas...)
int Poblacion::generacionC()
{
 int        numIteraciones = 0;	// Numero de iteraciones para llevar a cabo la generacion
 int        numNuevos = 0;	// Numero de nuevos individuos creados en la generacion
 Individuo *padre,   *madre;	// Los progenitores
 Individuo *hijo;    		// El retoño, mayormente parecido a su popo
 Individuo *hija;    		// La retoña, mayormente parecida a su momo
 int        posPadre, posMadre;	// Posiciones ocupadas por los padres en la poblacion
 int        posPadr2, posMadr2; // La competencia
 Flt        errHijo,  errHija;	// Errores de los fios

 // Ojo, se esta suponiendo que la poblacion YA esta ordenada...
 while(numIteraciones != numIndividuos && !errorAlcanzado())
 {
  posPadre = RND(numIndividuos);
  posMadre = RND(numIndividuos);
  posPadr2 = RND(numIndividuos);
  posMadr2 = RND(numIndividuos);
  // Saco 2 bolitas con los numeros de los padres (escogiendo a los mejores de un par...)
  posPadre = minimo(posPadre, posPadr2);
  posMadre = minimo(posMadre, posMadr2);
  // Esto da problemas por un doble delete, pero podia usarse para mutar en ese caso prob=1/tam^2
  if(posPadre == posMadre)
    continue;
  // Obtengo a ambos progenitores y sus errores respectivos
  padre = (*conjIndividuos)[posPadre];
  madre = (*conjIndividuos)[posMadre];

  // A veces, los hijos surgen como una mutacion de sus padres, pero mayormente son el fruto de un cruce
//  if(RND(100) < TPC_MUTACION)	// Mutatio habemus!!!
//  {
//   hijo = padre->mutacion(*conjVariables, *conjOperaciones);
//   hija = madre->mutacion(*conjVariables, *conjOperaciones);
//  }
//  else
//  {
   // Tras consumar el "ayuntamiento carnal con fembra placentera" viene el PARTO de ambos retoños
   hijo = padre->cruce(*madre);
   hija = madre->cruce(*padre);
//  }
  // Evaluo los errores de los hijos
  //hijo->setError(ptrFuncFitness(hijo, conjVariables));
  hijo->setError(elProblema->fitness(hijo, conjVariables));
  errHijo = hijo->getError();
  //hija->setError(ptrFuncFitness(hija, conjVariables));
  hija->setError(elProblema->fitness(hija, conjVariables));
  errHija = hija->getError();
  // Hago que el hijo sea mejor que la hija.
  // No es machismo, sino un convenio (y/o una arcaica forma de vivir?)
  if(errHijo > errHija)
  {
   intercambia(hijo, hija);		// Intercambia punteros a los objetos
   intercambia(errHijo, errHija);	// ademas de los correspondientes errores
  }
  // En este momento, hay una poblacion de numIndividuos a la que pretenden incorporarse los
  // 2 nuevos hijos. Solo seran aceptados si hay individuos que sean peores que ellos.
  if(errHijo < (*conjIndividuos)[numIndividuos - 1]->getError())
  {
   int n;

   // Me interesa meter a ambos hijos, por lo cual miro primero si puedo colocar a la hija
   if(errHija < (*conjIndividuos)[numIndividuos - 1]->getError())
    for(n = 0; n < numIndividuos; n++)			// Busca un individuo con mayor
     if((*conjIndividuos)[n]->getError() > errHija)	// error que el de la hija
     {
      intercambia(hija, (*conjIndividuos)[n]);		// Y lo intercambia...
      numNuevos++;
      break;
     }
   for(n = 0; n < numIndividuos; n++)			// Busca un individuo con mayor
    if((*conjIndividuos)[n]->getError() > errHijo)	// error que el del hijo
    {
     intercambia(hijo, (*conjIndividuos)[n]);		// Y lo intercambia...
     numNuevos++;
     break;
    }
  }
  delete hijo;			// Destruccion de los malos individuos
  delete hija;
  numIteraciones++;
 }
 numGeneraciones++;
 //cout << " [Nuevos: " << numNuevos << " (" << 100.*numNuevos/numIndividuos << "%)] " << endl;
 return numNuevos;
}

// La pareja padre-madre (aleatoriamente escogidos), tienen una enorme cantidad de descendencia.
// Esto, en las familias humanas, puede ser una forma de asegurar la consecucion de algun hijo excepcional.
int Poblacion::generacionD(int nHijos)
{
 int        numIteraciones = 0;	// Numero de iteraciones para llevar a cabo la generacion
 int        numNuevos      = 0;	// Numero de nuevos individuos creados en la generacion
 Individuo *padre, *madre;	// Los progenitores
 Individuo *hijo0, *hijo1;      // Sus 2 mejores hijos (el primogenito y el segundon)
 Conjunto<ptrIndividuo> *cHijos = new Conjunto<ptrIndividuo>(nHijos);	// Familia numerosa (como del opus...)
 int        posPadre, posMadre;	// Posiciones ocupadas por los padres en la poblacion
 int	    n;			// Un indice
 Flt        errPadre, errMadre;	// Los errores de los padres
 Flt        errHijo0, errHijo1; // Y de sus 2 mejores hijos

 // Conecto la funcion de ordenacion para ordenar a los hijos
 cHijos->conectaFuncionOrdenacion(fExtOrdenInd);

 // En cada generacion se intenta rellenar la poblacion entera...
 while(numIteraciones != numIndividuos && !errorAlcanzado())
 {
  // Saco 2 bolitas con los numeros de los padres
  posPadre = RND(numIndividuos);
  posMadre = RND(numIndividuos);
  // Esto da problemas por un doble delete, pero podia usarse para mutar en ese caso prob=1/tam^2
  if(posPadre == posMadre)
    continue;
  // Obtengo los errores de padre y madre
  errPadre = (*conjIndividuos)[posPadre]->getError();
  errMadre = (*conjIndividuos)[posMadre]->getError();
  // Para facilitar las cosas, hare que el padre sea el de menor error de ambos
  if(errPadre > errMadre)		// SOLO hay que intercambiar sus indices y errores
  {
   intercambia(posPadre, posMadre);
   intercambia(errPadre, errMadre);
  }
  padre = (*conjIndividuos)[posPadre];
  madre = (*conjIndividuos)[posMadre];

  // Ahora esta feliz pareja va a tener nHijos querubines... (alguno bueno deberia salir...)
  for(n = 0; n < nHijos; n++)
  {
   (*cHijos)[n] = (COC()) ? padre->cruce(*madre) : madre->cruce(*padre);
   (*cHijos)[n]->setError(elProblema->fitness((*cHijos)[n], conjVariables));
  }
  // Se ordena a los hijitos segun su bondad
  cHijos->ordena();
  // Y obtengo los errores de los 2 mejores (los 2 primeros)
  hijo0 = (*cHijos)[0];
  hijo1 = (*cHijos)[1];
  errHijo0 = hijo0->getError();
  errHijo1 = hijo1->getError();
  // Enfrento a los padres contra su numerosa prole, solo los 2 mejores sobreviviran
  if(errHijo1 < errPadre)	// Si el 2º hijo es mejor que el padre, los 2 primeros hijos se salvan
  {				// ya que errHijo1 < errHijo2 < errPadre < errMadre
   delete padre;                // Cria cuervos y te sacaran los ojos!
   delete madre;		// Mata a los padres.
   (*conjIndividuos)[posPadre] = new Individuo(*hijo0);	// El primogenito suplanta a su padre
   //(*conjIndividuos)[posPadre]->asignaPila();		// Se dan pilas (el constructor copia NO lo hace)
   (*conjIndividuos)[posMadre] = new Individuo(*hijo1);	// Y el segundon suplanta a su madre
   //(*conjIndividuos)[posMadre]->asignaPila();
   // Podria darse el caso de que el hijo sea mejor que el mejor individuo...
   if(errHijo0 < (*conjIndividuos)[posMejor]->getError())
    posMejor = posPadre;
   // Se han creado 2 nuevos individuos: ambos hijos
   numNuevos += 2;
  }
  else // Queda por tener en cuenta que a igualdad de errores, siempre debe ser preferible el individuo menor...
  {
   if(errHijo0 < errMadre)	// Si el primogenito es mejor que la madre: hay que matarla
   {
    delete madre;		// Se mata a la madre
    // En este punto, hay que comprobar si el primogenito es mejor que el padre:
    if(errHijo0 < errPadre)	// El primogenito es el mejor
    {
     (*conjIndividuos)[posPadre] = new Individuo(*hijo0);	// El hijo se convierte en viril padre
     //(*conjIndividuos)[posPadre]->asignaPila();
     (*conjIndividuos)[posMadre] = padre;			// y el padre se afemina y hace madre
     // Aqui queda comprobar si el primogenito es mejor que el mejor individuo...
     if(errHijo0 < (*conjIndividuos)[posMejor]->getError())
      posMejor = posPadre;
    }
    else					// El padre sigue siendo el mejor (de esa familia)
    {
     (*conjIndividuos)[posMadre] = new Individuo(*hijo0);	// Y el hijo pasa a ser la madre
     //(*conjIndividuos)[posMadre]->asignaPila();
    }
    // En todo caso, se ha creado 1 nuevo individuo: el hijo primogenito
    numNuevos++;
   }
  }
  // Finalmente, hay que desprenderse de los 25 tiernos infantes. Herodes, donde estas???
  for(n = 0; n < nHijos; n++)
   delete (*cHijos)[n];
  numIteraciones++;
 }
 // Destruyo la simpatica union familiar
 delete cHijos;
 numGeneraciones++;
 //cout << " [Nuevos: " << numNuevos << " (" << 100.*numNuevos/numIndividuos << "%)] " << endl;
 return numNuevos;
}

// Una vez escogido el novio padre de las criaturas, se le apega una novia lo mas complementaria posible, en el sentido
// de que DEBE TRATAR BIEN LOS PUNTOS QUE EL NOVIO NO PUEDE (aunque en el resto de puntos sea una ceporra!!!)
int Poblacion::generacionE(int nNovias)
{
 int        numIteraciones = 0;	// Numero de iteraciones para llevar a cabo la generacion
 int        numNuevos      = 0;	// Numero de nuevos individuos creados en la generacion
 int        numMutados     = 0; // Numero total de mutaciones en esta generacion
 int        numMutadosOK   = 0; // Numero de mutaciones aceptadas
 int        hayMutacion;	// Para saber cuando se da una mutacion
 Individuo *padre, *madre;	// Los progenitores
 Individuo *hijo;    		// El retoño, mayormente parecido a su popo
 Individuo *hija;    		// La retoña, mayormente parecida a su momo
 int        posPadre, posMadre;	// Posiciones ocupadas por los padres en la poblacion
 Flt        errPadre, errMadre;	// Los errores de los padres
 Flt        errHijo,  errHija;	// Errores de los fios
 int        n;			// Un humilde indice
 Flt        d;			// Distancia entre individuos
 Flt        dMax;		// La maxima distancia

 // En cada generacion se intenta rellenar la poblacion entera...
 while(numIteraciones != numIndividuos && !errorAlcanzado())
 {
  // Escojo al azar al infeliz papaito
  posPadre = RND(numIndividuos);
  // No recorro toda la poblacion porque seria una enorme burrada, sino que escojo nNovias aspirantes a emparejarse con
  // ese casto varon. El infeliz se desposara con la que resulte ser mas complementaria, medida por la distancia...
  dMax = 0.0;
  for(n = 0; n < nNovias; n++)
  {
   int posibleNovia = posPadre;

   while(posibleNovia == posPadre)		// Me aseguro especialmente de que nunca coincidan
    posibleNovia = RND(numIndividuos);
   d = elProblema->distancia((*conjIndividuos)[posPadre], (*conjIndividuos)[posibleNovia], conjVariables);
   if(d > dMax)
   {
    dMax     = d;
    posMadre = posibleNovia;
   }
  }
  // Obtengo los errores de padre y madre
  errPadre = (*conjIndividuos)[posPadre]->getError();
  errMadre = (*conjIndividuos)[posMadre]->getError();
  // Para facilitar las cosas, hare que el padre sea el de menor error de ambos
  if(errPadre > errMadre)		// SOLO hay que intercambiar sus indices y errores
  {
   intercambia(posPadre, posMadre);
   intercambia(errPadre, errMadre);
  }
  // Obtengo a ambos progenitores y sus errores respectivos
  padre = (*conjIndividuos)[posPadre];
  madre = (*conjIndividuos)[posMadre];

  // A veces, los hijos surgen como una mutacion de sus padres, pero mayormente son el fruto de un cruce
  //if(RND(100) < TPC_MUTACION)	// Mutatio habemus!!!
  if(dMax == 0.0)
  {                             // Me parecio ver una linda mutacion
   hayMutacion = VERDAD;	// Es cierto, es una linda mutacion!!!
   hijo = padre->mutacion(*conjVariables, *conjOperaciones);
   hija = madre->mutacion(*conjVariables, *conjOperaciones);
   numMutados += 2;
  }
  else
  {
   // Tras consumar el "ayuntamiento carnal con fembra placentera" viene el PARTO de ambos retoños
   hijo = padre->cruce(*madre);
   hija = madre->cruce(*padre);
   hayMutacion = FALSO;
  }
  // En este momento tenemos 4 individuos para 2 plazas: se quedan los mejores!
  // Evaluo los errores de los hijos
  hijo->setError(elProblema->fitness(hijo, conjVariables));
  errHijo = hijo->getError();
  //hija->setError(ptrFuncFitness(hija, conjVariables));
  hija->setError(elProblema->fitness(hija, conjVariables));
  errHija = hija->getError();
  // Hago que el hijo sea mejor que la hija.
  // No es machismo, sino un convenio (y/o una arcaica forma de vivir?)
  if(errHijo > errHija)
  {
   intercambia(hijo, hija);		// Intercambia punteros a los objetos
   intercambia(errHijo, errHija);	// ademas de los correspondientes errores
  }

  // Enfrento a 2 generaciones: El padre es mejor que la madre, y el hijo mejor que la hija
  // NAZISMO: Se quedan los 2 individuos de menor error y se extermina a los restantes...
  if(errHija < errPadre)	// Si la hija es mejor que el padre, ambos hijos se salvan
  {				// ya que errHijo < errHija < errPadre < errMadre
   delete padre;                // Cria cuervos y te sacaran los ojos!
   delete madre;		// Mata a los padres.
   (*conjIndividuos)[posPadre] = hijo;		// El hijo asume el puesto del padre (EDIPO)
   (*conjIndividuos)[posMadre] = hija;		// Y la hija el de su difunta madre (ELECTRA)
   // Podria darse el caso de que el hijo sea mejor que el mejor individuo...
   if(errHijo < (*conjIndividuos)[posMejor]->getError())
   {
    posMejor = posPadre;
    if(hayMutacion)
     cout << "!";
   }
   // Se han creado 2 nuevos individuos: ambos hijos
   numNuevos += 2;
   // Para llevar la cuenta de las mutaciones
   if(hayMutacion)
    numMutadosOK += 2;
  }
  else // Queda por tener en cuenta que a igualdad de errores, siempre debe ser preferible el individuo menor...
  {
   delete hija;                 // La hija es peor que el padre y hay que matarla
   if(errHijo < errMadre)	// Si el hijo es mejor que la madre: hay que matarla
   {				// errHijo < min(errHija, errMadre)
    delete madre;		// Se mata a la madre
    // En este punto, hay que comprobar si el hijo es mejor que el padre:
    if(errHijo < errPadre)	// El hijo es el mejor
    {
     (*conjIndividuos)[posPadre] = hijo;	// El hijo se convierte en viril padre
     (*conjIndividuos)[posMadre] = padre;	// y el padre se afemina y hace madre
     // Aqui queda comprobar si el hijo es mejor que el mejor individuo...
     if(errHijo < (*conjIndividuos)[posMejor]->getError())
     {
      posMejor = posPadre;
      if(hayMutacion)
       cout << "!";
     }
    }
    else					// El padre sigue siendo el mejor (de esa familia)
     (*conjIndividuos)[posMadre] = hijo;	// Y el hijo pasa a ser la madre
    // En todo caso, se ha creado 1 nuevo individuo: el hijo
    numNuevos++;
    // Y por si es mutado
    if(hayMutacion)
     numMutadosOK++;
   }
   else			// El vastago es tan rematadamente malo que no merece vivir: apologia abortiva
    delete hijo;        // ya que incluso era peor que su madre (errHijo > errMadre)
  }
  numIteraciones++;
 }
 numGeneraciones++;
 //cout << " [Nuevos: " << numNuevos << " (" << 100.*numNuevos/numIndividuos << "%)] " << endl;
 //cout << "#Mut: " << numMutadosOK << "/" << numMutados << endl;
 return numNuevos;
}

// Refinamiento de una poblacion: Se recorren todos sus miembros y para cada
// uno de ellos se comprueba si alguno de los subarboles inmediatamente
// colgantes del nodo raiz es mejor que el arbol completo. Para aquellos
// en los que esto se cumpla, ese subarbol queda como arbol principal,
// podando el resto del arbol. Tambien se recalculan los errores de cada
// individuo refinado, y el error medio de la poblacion; tambien puede
// cambiar el mejor individuo!!!
void Poblacion::refinaPoblacion()
{
/*
 errorPoblacion = .0;
 for(int n = 0; n < numIndividuos; n++)
 {
  // Si se refina, hay que ver si el mejor error se ha superado
  if((*conjIndividuos)[n]->refinaIndividuo(ptrFuncFitness, conjVariables))
   if((*conjIndividuos)[n]->getError()<(*conjIndividuos)[posMejor]->getError())
    posMejor = n;
  // En todo caso, calculamos el error medio de la poblacion
  errorPoblacion += (*conjIndividuos)[n]->getError();
 }
 errorPoblacion /= numIndividuos;
*/
}

// Algoritmo de Karl Wahall para la limpieza etnica y posterior recuperacion de la diversidad genetica.
// Se salva un porcentaje de la poblacion actual, la elite de los mejores, y se extermina sistematicamente
// al resto; despues se sustituye a los caidos por una nueva poblacion aleatoria ¿y enriquecida?, como se
// hace con la mano de obra tercermundista...
void Poblacion::nemesis(int nElite)
{
 int n;

 // En primer lugar ordeno la poblacion actual, para ver quienes son la elite que sobrevivira al genocidio
 conjIndividuos->ordena();

 // Ahora someto a los peores a un proceso de holocausto, o nemesis, y asi ahorro memoria para luego
 for(n = nElite; n < numIndividuos; n++)
 {
  delete (*conjIndividuos)[n];
  (*conjIndividuos)[n] = new Individuo(*conjVariables, *conjOperaciones);
  (*conjIndividuos)[n]->setError(elProblema->fitness((*conjIndividuos)[n], conjVariables));
 }

/*
 // Tengo 2 opciones, rellenar todas las bajas con individuos al azar o bien crear una nueva poblacion enriquecida

 // Creo un conjunto inicial de cardinal numIndividuos*factorEnriq; calculo el error de cada miembro y los ordeno
 // segun este, crecientemente. Me quedo con los numIndividuos - nElite primeros!!!
 int tam = numIndividuos * factorEnriq;
 Conjunto<ptrIndividuo> *cIniInd = new Conjunto<ptrIndividuo>(tam);

 // Y los creo de uno en uno con todos sus niveles y calculo su error
 for(n = 0; n < tam; n++)
 {
  (*cIniInd)[n] = new Individuo(*conjVariables, *conjOperaciones);
  (*cIniInd)[n]->setError(elProblema->fitness((*cIniInd)[n], conjVariables));
 }
 // Ahora habria que ordenar esa enorme poblacion enriquecida crecientemente segun el error individual
 cIniInd->conectaFuncionOrdenacion(fExtOrdenInd);
 cIniInd->ordena();
 // Tras la ordenacion me quedo con los primeros, que son los mejores como es logico!!!
 // Seguramente son peores que la elite actual, pero insuflan nueva diversidad genetica
 for(n = nElite; n < numIndividuos; n++)
  (*conjIndividuos)[n] = (*cIniInd)[n - nElite];
 // Y extermino a los desafortunados que no tienen cabida en este nuevo n-simo reich (hdp!!!)
 for(n = numIndividuos; n < tam; n++)
  delete (*cIniInd)[n];
 delete cIniInd;
*/
 // Para acabar, actualizo los parametros de la poblacion
 actualizaParametros();
 posMejor = 0;			// El mejor, lider, guru y timonel ocupa la primera e indiscutible posicion
}

// Tras acabar un entrenamiento, escribe los 2 ficheros de salida:
// expres019.dat con datos sobre el entrenamiento y el mejor...
// salida019.dat con datos de la funcion aproximada y de la aproximacion propuesta
// El primer parametro es el nombre de la funcion externa de escritura creada por el usuario
// El segundo (opcional) es el numero de segundos que ha durado el entrenamiento
void Poblacion::escribeResultados(void (*ptrFuncEscritura)(ptrIndividuo, ptrConjNodos, fstream &), int t)
{
 // Al final, creo un fichero con la expresion (*.EXP) que contiene la expresion y algun otro dato mas
 // y otro con su evaluacion en el intervalo dado, con la primera columna con los valores de X,
 // la segunda con los de f(X) y la tercera con los ARB(X). Sera utilizado para crear graficas...
 char *elNombre = generaNombre("expre000.dat");	// Se le da un nombre al fichero fruto de la evolucion
 cout << "Fichero de salida:        " << elNombre         << endl;
 fstream fExp(elNombre, ios::out);		// Fichero de expresion
 fExp << "Fichero de salida:        " << elNombre         << endl;
 fExp << "Problema a resolver:      " << elProblema->getNombre() << endl;
#if defined(__VMS)
 fExp << "Maquina: HORRU VMS/VAX"                         << endl;
#else
 fExp << "Maquina: PENTIUM 100"                           << endl;
#endif
 fExp << __FILE__ << "  " << __DATE__ << "  " << __TIME__ << endl;
 fExp << "Poblacion:                " << numIndividuos    << endl;
 fExp << "Semilla GNA:              " << semilla          << endl;
 fExp << "Factor enriquecimiento:   " << factorEnriq      << endl;
 fExp << "Metodo de generacion:     " << metodoGeneracion << endl;
 fExp << "Generaciones:             " << numGeneraciones  << endl;
 if(t)
  fExp << "Tiempo de entrenamiento:  " << t << " segundos" << endl;
 fExp << "Posicion del mejor:       " << posMejor << endl;
 fExp << "Error del mejor:          " << (*conjIndividuos)[posMejor]->getError() << endl;
 fExp << "Numero de nodos:          " << (*conjIndividuos)[posMejor]-> getNumNodos() << endl;
 fExp << "Numero de intrones:       " << (*conjIndividuos)[posMejor]-> getNumIntrones() << endl;
 fExp << "Error medio de poblacion: " << errorMedioPoblacion << endl;
 fExp << "Numero medio de nodos:    " << numMedioNodos << endl;
 fExp << "Numero medio de intrones: " << numMedioIntrones << endl;
 fExp << "Expresion del mejor:" << endl;
 fExp << *((*conjIndividuos)[posMejor]->getArbol()) << endl;

 fExp.close();

 fstream fSal(generaNombre("salid000.dat"), ios::out);         // Fichero de datos para graficas
 // Se llama a la funcion externa dada por el usuario
 ptrFuncEscritura(getMejorIndividuo(), conjVariables, fSal);
 fSal.close();
}
