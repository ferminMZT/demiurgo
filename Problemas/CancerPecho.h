// Problema 7
// CancerPecho__________________________________________________________________
// Tenemos 569 casos de cancer de pecho (de Wisconsin) lirios para clasificar en 2 clases distintas
// usando 30 atributos
class CancerPecho : public problema
{
 public:
  CancerPecho()
  {
   nombre       = "El cancer de pecho de Wisconsin";
   batcancer     = new bateria("cancerPecho.dat");
   numEjemplos  = batcancer->getNumEjemplos();
   numAtributos = batcancer->getNumAtributos();
  }
 ~CancerPecho()
  {
   delete batcancer;
   numEjemplos = numAtributos = 0;
   pilaEvaluacion = 0;
  }
  char *getNombre()
  { return nombre; }
  flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
  flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
  { return 0.0; }
  flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest)
  { return 0.0; }

 private:
  char     *nombre;		// Nombre del problema a resolver
  bateria  *batcancer;		// Bateria de ejemplos a clasificar
  int       numEjemplos;      	// Numero de ejemplos de la bateria
  int       numAtributos;	// Numero de atributos de cada ejemplo
  variable *var;
  flt       errTodasMuestras;
  int       ne, na;
  flt       valEvaluado, valClase;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion

};

#endif