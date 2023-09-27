// Problema 5
// Vino_________________________________________________________________________
// Tenemos 178 vinos para clasificar en 3 clases distintas usando 13 atributos
class Vino : public problema
{
 private:
  char     *nombre;		// Nombre del problema a resolver
  bateria  *batVino;		// Bateria de ejemplos a clasificar
  int       numEjemplos;	// Numero de ejemplos de la bateria
  int       numAtributos;	// Numero de atributos de cada ejemplo
  variable *var;
  flt       errTodasMuestras;
  int       ne, na;
  flt       valEvaluado, valClase;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  Vino()
  {
   nombre       = "La clasificacion enologica";
   batVino      = new bateria("vino.dat");
   numEjemplos  = batVino->getNumEjemplos();
   numAtributos = batVino->getNumAtributos();
  }
 ~Vino()
  {
   delete batVino;
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
};