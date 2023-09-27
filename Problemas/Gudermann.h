// Problema 4
// Gudermann____________________________________________________________________
// log10[1./cos(x) + tan(x)]		x en (-M_PI/2, +M_PI/2)
class Gudermann : public problema
{
 private:
  char *nombre;			// Nombre del problema a resolver
  int numPuntos;		// Numero de puntos a tratar
  flt *valX;			// Componente X
  flt *valG;                    // Valor de la Gudermanniana

  // Por otra parte, pongo aqui unas variables auxiliares para el calculo del
  // fitness para no tener que crearlas y ganar una poca velocidad mas... ?
  variable *X;
  flt       errTodasMuestras;
  int       n;
  flt       valEvaluado;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  Gudermann();
 ~Gudermann()
  {
   delete [] valX;
   delete [] valG;
   nombre 	  = 0;
   numPuntos      = 0;
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