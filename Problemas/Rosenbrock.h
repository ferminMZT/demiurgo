// Problema 3
// Rosenbrock___________________________________________________________________
// La funcion en forma de silla de montar (saddle) de ese sujeto:
// 100. * (x^2 - y^2) + (1. - x)^2              x, y en [-2.048, +2.048]
class Rosenbrock : public problema
{
 private:
  char *nombre;			// Nombre del problema a resolver
  int nPtos;			// Numero de puntos para x, y en [-2.048, +2.048]
  // Por otra parte, pongo aqui unas variables auxiliares para el calculo del
  // fitness para no tener que crearlas y ganar una poca velocidad mas... ?
  variable *X, *Y;
  flt       x, y;
  flt       errTodasMuestras;
  flt       a, b;
  flt       inc;
  int       n;
  flt       valEvaluado;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  Rosenbrock()
  {
   nombre = "Silla de montar de Rosenbrock: 100. * (x^2 - y^2) + (1. - x)^2 con x, y en [-2.048, +2.048]";
   nPtos = 50;
   a     = -2.048;
   b     = +2.048;
   inc   = (b - a) / nPtos;
  }
 ~Rosenbrock()
  {
   nombre = 0;
   nPtos          = 0;
   pilaEvaluacion = 0;
  }
  char *getNombre()
  { return nombre; }
  flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
  flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
  { return .0; }
  flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest)
  { return .0; }
};