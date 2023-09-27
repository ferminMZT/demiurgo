// Problema 6
// Lirio________________________________________________________________________
// Tenemos 150 lirios para clasificar en 3 clases distintas usando 4 atributos
class Lirio : public problema
{
 private:
  char     *nombre;		// Nombre del problema a resolver
  bateria  *batLirio;		// Bateria de ejemplos a clasificar
  int       numEjemplos;      	// Numero de ejemplos de la bateria
  int       numAtributos;	// Numero de atributos de cada ejemplo
  variable *var;
  flt       errTodasMuestras;
  int       ne, na;
  flt       valEvaluado, valClase;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  Lirio()
  {
   nombre       = "Clasificacion de los lirios";
   batLirio     = new bateria("lirios.dat");
   numEjemplos  = batLirio->getNumEjemplos();
   numAtributos = batLirio->getNumAtributos();
  }
 ~Lirio()
  {
   delete batLirio;
   numEjemplos = numAtributos = 0;
   pilaEvaluacion = 0;
  }
  char *getNombre()
  { return nombre; }
  flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
  flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
  { return 0.0; }
  flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest);
};