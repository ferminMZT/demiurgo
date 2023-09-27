// Problema 12
// Lentes_______________________________________________________________________
// Tenemos 24 ejemplos con 4 atributos sobre pacientes del oftalmologo, y como
// clasificacion se produce 1 de 3 valores.
class Lentes : public problema
{
 private:
  char     *nombre;		// Nombre del problema a resolver
  bateria  *batLentes;		// Bateria de ejemplos a clasificar
  int       numEjemplos;      	// Numero de ejemplos de la bateria
  int       numAtributos;	// Numero de atributos de cada ejemplo
  variable *var;
  flt       errTodasMuestras;
  int       ne, na;
  flt       valEvaluado, valClase;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  Lentes()
  {
   nombre       = "Lentes: Debo usarlas duras, blandas o no usarlas?";
   batLentes    = new bateria("lenses.dat");
   numEjemplos  = batLentes->getNumEjemplos();
   numAtributos = batLentes->getNumAtributos();
  }
 ~Lentes()
  {
   delete batLentes;
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