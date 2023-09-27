// Problema 8
// monjes1______________________________________________________________________
// Tenemos 569 casos de cancer de pecho (de Wisconsin) lirios para clasificar en 2 clases distintas
// usando 30 atributos
class Monjes1 : public problema
{
 private:
  char     *nombre;		// Nombre del problema a resolver
  bateria  *batMonjes1;		// Bateria de ejemplos a clasificar
  int       numEjemplos;      	// Numero de ejemplos de la bateria
  int       numAtributos;	// Numero de atributos de cada ejemplo
  variable *var;
  flt       errTodasMuestras;
  int       ne, na;
  flt       valEvaluado, valClase;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  Monjes1()
  {
   nombre       = "Los monjes (I)";
   batMonjes1   = new bateria("monjes3.dat");
   numEjemplos  = batMonjes1->getNumEjemplos();
   numAtributos = batMonjes1->getNumAtributos();
  }
 ~Monjes1()
  {
   delete batMonjes1;
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