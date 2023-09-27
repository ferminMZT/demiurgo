// Problema 2
// Parabola1____________________________________________________________________
// sumatorio(i=1,3) (x(i)*x(i)  con x(i) en [-5.12, +5.12]
// Esto lo entiendo como x*x + y*y + z*z en ese intervalo
class Parabola1 : public problema
{
 private:
  char     *nombre;		// Nombre del problema a resolver
  int       nPtos; 		// Numero de puntos para x, y, z en [-5.12, +5.12]
  // Por otra parte, pongo aqui unas variables auxiliares para el calculo del
  // fitness para no tener que crearlas y ganar una poca velocidad mas... ?
  variable *X, *Y, *Z;
  flt       x, y, z;
  flt       errTodasMuestras;
  flt       a, b;
  flt       inc;
  int       n;
  flt       valEvaluado;
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  Parabola1()
  {
   nombre = "La hiperparabola x^2 + y^2 + z^2 con x, y, z en [-5.12, +5.12]";
   nPtos = 10;
   a     = -5.12;
   b     = +5.12;
   inc   = (b - a) / nPtos;
  }
 ~Parabola1()
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