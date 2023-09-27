// Problema 11
// CompresionImagen_____________________________________________________________
// Tenemos una imagen de 25*25 y la queremos codificar en una expresion.
// Es una regresion simbolica un tanto extraña, pero puede ser espectacular

class CompresionImagen : public problema
{
 private:
  char     *nombre;		// Nombre del problema a resolver
  int       x, y;               // Indices de la imagen
  int       xTam, yTam;		// Tamaño de la imagen;
  flt       xTamInv, yTamInv;	// Inversas
  variable *X, *Y;		// Variables para cargar individuos
  byte    **pixels;		// Los atributos, datos de entrada (que se usan para clasificar...)
  flt       errTotalImagen;	// Error acumulado al descomprimir toda la imagen
  flt       valEvaluado;	// Valor tras evaluar un pixel
  flt       invNumPixels;	// 1/numero total de pixels
  piposeco *pilaEvaluacion;	// Pila piposeco para la evaluacion
 public:
  CompresionImagen();
 ~CompresionImagen();
  char *getNombre()
  { return nombre; }
  flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables);
  flt distancia(ptrIndividuo pInd1, ptrIndividuo pInd2, ptrConjNodos pConjVariables)
  { return 0.0; }
  flt testeaProblema(ptrIndividuo pInd, ptrConjNodos pConjVariables, char *nomFichTest)
  { return 0.0; }
};