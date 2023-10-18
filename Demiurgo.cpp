/*
  \file Demiurgo.cpp
  Aqui esta el main() que tira de todo.


  V9.0: 10/07/02
  Casi 4 años despues de haber sembrado el caos y la discordia, vuelve Demiurgo 2002!!!
  Los amigos han cambiado/muerto pero Loebia resiste incansable.

  Esto va por ti, fiel amiga!!!

  Una nueva superversion Demiurgica se esta preparando, con todas las maldades que he
  aprendido y aprehendido en estos ultimos fieros años, cuyas dentelladas no han
  conseguido (todavia) acabar conmigo...

  V8.0: Corre en PC, en Horru y en el Centauro!!!
  Incorpora el supra-enriquecimiento del profesor Karl Wahall. Record absoluto: 4 errores!!!!
  En el Centauro ha comenzado a funcionar tras sustituir los int en algun cast de ptr por long... esto es logico, ya
  que realmente se trata de una maquina de 64 bits!!!
  01/06/98: Entre los vapores etilicos del party de Karl Wahall, se me ha ocurrido que para tener un hijo realmente bueno,
  una pareja deberia tener la de dios de hijos (como en la real y perra vida...) Es un nuevo enfoque de generacion!!!
  Sus resultados son impresionantes con poblaciones de tamaños ridiculos y es muy raro que se produzcan estrangulamientos
  Otra etilica idea consiste en la coevolucion simultanea de varias poblaciones que se van intercambiando sus mejores
  individuos... Parece dar tambien muy buenos resultados con poblaciones escasas, habiendo llegado a un error de 5 con
  mi escaso pentium 100 casero en un brevisimo lapso de tiempo!!!
  02/06/98: Probare con 10 pequeñas poblaciones que coevolucionan simultaneamente. Los resultados todavia no son muy buenos
  03/06/98: Vamos por el esquema de reproduccion basado en la eleccion de parejas lo mas complementarias posibles...
  08/06/98: A por los intrones con 2 c**ones!!!
  14/06/98: Un mes sin "fumar" y se me ocurre que las pilas piposeco solo son necesarias en la evaluacion; de esta forma
  hay un importante ahorro de memoria que repercute en mayores poblaciones evolucionando...
  16/06/98: Los intrones parecen estar comportandose de puta madre
 *17/06/98: A las 9 he decapitado al DRAGON de las ESPIRALES!!!!!!!!!!!!!!!!!!!!
  18/06/98: Otra vez he resuelto el enigma. Me preparo para plantear el problema de la parabola!!!
            Introduzco la idea del fitness aLeatico (aleatorio-ad-letico)
  22/06/98: El problema de Gudermann
  Demiurgo.cpp: Ahora voy a tener en cuenta que cada individuo es un arbol GAP, con parte arborea y parte vectorial:
        +                0------------> .5
                               / \               ...
                              /   \              2------------> .7
                             /     \             ...
                            *       -            7------------> .45
                           / \     / \           ...
                          /   \   /   \          ...
                         Y    7  2     X      TAM_TABLA_GA-1 -> .1

                         Parte arborea		Parte vectorial (T)
 El arbol es:
    Y * T[7] + T[2] - X  =  .45*Y + .7 - X

 ULTIMA HORA:
  La evaluacion del fitness arboreo me cuesta un huevo temporalmente hablando.
        La recursividad es una puta mierda. Alguna forma de evaluacion arborea basada
        en una pila seria cojonuda y usando la forma en preorden (polaka) seria facil
        a la vez que rapida. Problema: Cada individuo debera crear esa pila al mismo
        tiempo que crea su estructura arborea... Esto puede ser lento en ese momento,
        pero cuando hay que evaluar fitness en miles de puntos AHORRA LA DE DIOS!!!
        Otro problema: Una estructura dinamica como la pila, se encoge y agranda,
        provocando muchos movimientos de memoria que son ineficientes e inutiles
        para mis fines. Ademas muchas partes son constantes ya precalculables en
        la creacion de arbol y pila, y no interesa menear la cosa mas de la cuenta.
        SOLUCION: INVENTO LA SIGUIENTE ESTRUCTURA DE DATOS: la pila PIPOSECO
        (PIla POlaca SEmi COnstante), que es una pila con todo en forma polaca (+ x 2)
        pero que no es dinamica ni estatica, sino semiconstante en el sentido de que
        SOLO CAMBIA LO QUE ES ABSOLUTAMENTE IMPRESCINDIBLE QUE CAMBIE PARA LA CORRECTA
        EVALUACION ARBOREA (los valores de variables y los resultados intermedios)...
        Ejemplo:
        Sea la expresion:	f(x) = 2*x + (x - 1/(x*x - 1))
        En forma arborea:
        +               Parte vectorial:
             / \              0 ---> 2.
                              /   \             1 ---> 1.
                             /     \               ...
                            *       -
                           / \     / \
                          2   x   x   /
                                     / \
                                    1   -
                                       / \
                                      *   1
                                     / \
                                    x   x
        Y esta seria su traduccion a estructura PIPOSECO, como un vector con n elementos,
        correspondiente cada uno de ellos con cada nodo (en este caso 13):

        INDICE	TIPO	VALOR	SIG1	SIG2	Podemos ver que:
    8 > 12        (k)   1			INDICE: Es un simple numero de posicion
    9 > 11        (v)   x			(corresponde al recorrido en preorden...)
        10        (v)   x			TIPO: Es el tipo del nodo
         9      * (o2)  ...     11*			(k)  constante
    6 >  8      - (o2)  ...     12                      (v)  variable
         7        (k)   1                               (o1) operacion unaria
    4 >  6      / (o2)  ...	8*                      (o2) idem binaria
         5        (v)   x                               (o3) idem ternaria
    0 >  4      - (o2)  ...	6*		VALOR: Esto es lo realmente curioso
    1 >  3        (v)   x				constante: no cambia
         2        (k)   2				variable: cambia en cada evaluacion
         1      * (o2)  ...	3*			...: Son los valores intermedios que
         0	+ (o2)  ...	4                            se van calculando y CAMBIAN!!!

    Los 2 ultimos campos son punteros (posiciones int del piposeco) que indican la posicion
    piposeco donde se almacenan los parametros de la operacion en curso:
      Si una operacion esta en la posicion [i], su primer parametro esta en la posicion
  [i+1], y su segundo parametro en [SIG1], y el tercero (si existiese) en [SIG2].
        Ojo: Estas cosas raras son para expresiones anidadas, pues sin anidamiento,
        tendriamos que [SIG1] == [i+1]

Lo realmente cojonudo de este invento, es que:
  - LA EVALUACION ES TOTALMENTE ITERATIVA
        - SOLO HAY QUE CONECTAR LAS VARIABLES AL CAMBIARLAS PARA EVALUAR EL FITNESS
        - LO CONSTANTE NO SE VUELVE A TOCAR NUNCA
        - SE PUEDEN HACER SIMPLIFICACIONES DE CONSTANTES AL CREAR EL PIPOSECO
Para evaluar se comienza por arriba (indices altos) y se va recorriendo cada operacion
y calculando sus resultados, y asi sucesivamente hasta abajo, para llegar en la posicion 0
con el nodo raiz con la ultima operacion...

Propongo ademas, que se pueda asignar a los objetos de tipo variable, una direccion de
memoria pegada a otra variable que se use en el calculo del fitness...

SI ESTO FUNCIONA, SERA LA HOSTIA!!!

Lun23Feb97: Ya esta programado y consigo una reduccion que oscila entre un
20% para pocas generaciones y un 25% con mas!!!

Por otra parte, las pilas PIPOSECO son bastante simplificables, con lo que
ello implica en tiempo de ejecucion!!!

Ademas por sugerencia de Fernando Cano, creo la clase bateria de ejemplos...

La nueva version, incluira ASM del coprocesador matematico para la evaluacion
de la pila PIPOSECO. Esto deberia elevar notablemente la velocidad de ejecucion
aunque restringiria al Demiurgo a operar solo sobre maquinas PC...

El ASM se desestima de momento...

La division no hace mas que dar problemas al clasificar las espirales, por lo que la destituyo.

Es importante poder almacenar la semilla, asi como usar ficheros de configuracion...
El operador nemesis se prueba salvando a un solo individuo de la matanza...
Salvo la semilla e introduzco la posibilidad de mas generaciones
*/

#include <iostream> // cout...
#include <fstream>  // fstream <<...
// #include <strstream>	// fstream <<...
#include <iomanip>  // setprecision
#include <stdlib.h> // qsort, rand, srand, rand...
#include <math.h>   // pow, cos, sin...
#include <time.h>   // time
#include <float.h>  // FLT_MAX
// #include <io.h>			// access

#include "TCrono.h" // Clase TCrono para cronometrar ejecuciones
#include "Utiles.h" // Mi cajon de sastre

#include "Operaciones/OperacionPuntoComaAsignacion.h"
#include "Operaciones/OperacionCondicional.h"
#include "Operaciones/OperacionCoseno.h"
#include "Operaciones/OperacionCuadrado.h"
#include "Operaciones/OperacionDivision.h"
#include "Operaciones/OperacionNegativo.h"
#include "Operaciones/OperacionProducto.h"
#include "Operaciones/OperacionResta.h"
#include "Operaciones/OperacionSeno.h"
#include "Operaciones/OperacionSuma.h"
#include "Variable.h"
#include "Problemas/Espiral.h"
#include "Problemas/KozaRegresionSimbolica1.h"
#include "Problemas/Sombrero.h"
#include "Problemas/SDI000.h"
#include "Problemas/Parabola1.h"
#include "Poblacion.h"
#include "Nodo.h"
#include "Conjunto.h"

// Algunas definiciones utiles

// Aqui van algunas funciones de uso general

void main1();         // Espiral
void main2();         // Espiral
void main3();         // Espiral
void ResuelveParabola1();
void main5();         // Rosenbrock
void main6();         // Gudermann
void main7();         // Vino
void ResuelveLirio(); // Lirio
void main9();         // CancerPecho
void main10();        // Monjes1
void ResuelveBupa();
void ResuelveAbalones();
void ResuelveCompresionImagen();
void ResuelveKozaRegresionSimbolica1();
void ResuelveSombrero();
void ResuelveLentes();
void ResuelveRaizCuadrada();
void resuelveSDI000();

Flt f(const Flt &x);
/*
Flt  fitness(ptrIndividuo pInd, ptrConjNodos pConj);
Flt  fitness2(ptrIndividuo pInd, ptrConjNodos pConj);
*/
void funcionEscrituraExterna(ptrIndividuo pInd, ptrConjNodos pConj, std::fstream &fichero);

void execution_info(void)
{
    std::cout << std::endl;

    std::cout << "\t  //" << std::endl;
    std::cout << "\t // Demiurgo" << std::endl;
    std::cout << "\t//" << std::endl;
    
    std::cout << "\n\t" << "Version " << __DATE__ << " " << __TIME__ << "\n" << std::endl;


}

int main(int argc, char *argv[])
{
    execution_info();

    ResuelveParabola1();

    // ResuelveRaizCuadrada();
    // ResuelveLirio();
    // main3();
    //  ResuelveKozaRegresionSimbolica1();

    // ResuelveSombrero();
    // resuelveSDI000();

    DEBUG("Acabado!");

    // Esto indica una ejecucion correcta.
    return 0;
}

// Resuelve Espiral
void main1()
{
  // Creo las operaciones basicas:
  OperacionSuma nSuma;
  OperacionResta nResta;
  OperacionProducto nProducto;
  // OperacionDivision    nDivision;	// Altamente problematica!!!
  OperacionNegativo nNegativo;
  OperacionSeno nSeno;
  OperacionCoseno nCoseno;
  OperacionCondicional nCondicional;
  // Las variables
  Variable X('X', 0.);
  Variable Y('Y', 0.);

  // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
  Conjunto<ptrNodo> cnjVar(2);
  Conjunto<ptrNodo> cnjOpc(7);
  cnjVar.agrega(&X);
  cnjVar.agrega(&Y);
  cnjOpc.agrega(&nSuma);
  cnjOpc.agrega(&nResta);
  cnjOpc.agrega(&nProducto);
  // cnjOpc.agrega(&nDivision);
  cnjOpc.agrega(&nNegativo);
  cnjOpc.agrega(&nCoseno);
  cnjOpc.agrega(&nSeno);
  cnjOpc.agrega(&nCondicional);

  time_t laSemilla = time(NULL);
  std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
  srand(laSemilla); // Comentando esta linea obtengo siempre lo mismo

  TCrono tiempo; // Para cronometrar
  tiempo.arranca();
  // Creo el problema
  Espiral spiralia;
  // Y las 3 poblaciones
  Poblacion A(&cnjVar, &cnjOpc);
  A.asignaProblema(&spiralia);
  A.asignaSemilla(laSemilla);
  A.creaPoblacionInicial();
  Poblacion B(&cnjVar, &cnjOpc);
  B.asignaProblema(&spiralia);
  B.asignaSemilla(laSemilla);
  B.creaPoblacionInicial();
  Poblacion C(&cnjVar, &cnjOpc);
  C.asignaProblema(&spiralia);
  C.asignaSemilla(laSemilla);
  C.creaPoblacionInicial();
  // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
  int nGener = 0; // Numero de generaciones
  while (nGener < NUM_MAX_GENER)
  {
    std::cout << "#G" << nGener << std::endl;
    int nA = A.generacionAI();
    Flt emA = A.getMejorError();
    A.actualizaParametros();
    Flt etA = A.getErrorMedioPoblacion();
    std::cout << "\tA\tI:" << nA << " E:" << emA << "/" << etA << std::endl;
    int nB = B.generacionB();
    Flt emB = B.getMejorError();
    B.actualizaParametros();
    Flt etB = B.getErrorMedioPoblacion();
    std::cout << "\tB\tI:" << nB << " E:" << emB << "/" << etB << std::endl;
    int nC = C.generacionA();
    Flt emC = C.getMejorError();
    C.actualizaParametros();
    Flt etC = C.getErrorMedioPoblacion();
    std::cout << "\tC\tI:" << nC << " E:" << emC << "/" << etC << std::endl;
    nGener++;
    if (nGener == NUM_MAX_GENER - 1)
    {
      int nGenAdicionales = 0;

      std::cout << "Generaciones adicionales ??? ";
      std::cin >> nGenAdicionales;
      nGener -= nGenAdicionales;
    }
  }
  tiempo.detente();
  std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
}

// Resuelve Espiral
void main2()
{
  // int n;
  // Creo las operaciones basicas:
  OperacionSuma nSuma;
  OperacionResta nResta;
  OperacionProducto nProducto;
  // OperacionDivision    nDivision;	// Altamente problematica!!!
  OperacionNegativo nNegativo;
  OperacionSeno nSeno;
  OperacionCoseno nCoseno;
  OperacionCondicional nCondicional;
  // Las variables
  Variable X('X', 0.);
  Variable Y('Y', 0.);

  // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
  Conjunto<ptrNodo> cnjVar(2);
  Conjunto<ptrNodo> cnjOpc(7);
  cnjVar.agrega(&X);
  cnjVar.agrega(&Y);
  cnjOpc.agrega(&nSuma);
  cnjOpc.agrega(&nResta);
  cnjOpc.agrega(&nProducto);
  // cnjOpc.agrega(&nDivision);
  cnjOpc.agrega(&nNegativo);
  cnjOpc.agrega(&nCoseno);
  cnjOpc.agrega(&nSeno);
  cnjOpc.agrega(&nCondicional);

  time_t laSemilla = time(NULL);
  std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
  // srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

  TCrono tiempo; // Para cronometrar
  tiempo.arranca();
  // Creo el problema
  Espiral spiralia;
  // Creo una serie de poblaciones que co-evolucionaran en paralelo, ayudandose en caso de estrangulamientos y desastres
  // Utopica idea basada en la negacion de la realidad que contemplamos cada dia en los telediarios (ahi queda eso!!!)
  const int nP = 3;                                           // Numero de poblaciones
  // int nR = 5;                                                 // Numero de retoños de cada pareja
  int n;                                                      // Indice de poblaciones
  int nPM = 0;                                                // Posicion de la mejor poblacion
  int nIG[nP];                                                // Numero de individuos creados en la generacion
  int nIT[nP];                                                // Numero de individuos totales en cada poblacion
  int nGE[nP];                                                // Numero de generaciones esteriles
  Flt eTP[nP];                                                // Error medio total de la poblacion
  Flt eMI[nP];                                                // Error del mejor individuo
  ptrConjPoblaciones cnjPob = new Conjunto<ptrPoblacion>(nP); // Las naciones creadas!!!

  for (n = 0; n < nP; n++) // Bucle de inicializacion de las poblaciones
  {
    std::cout << "Creando poblacion " << n << std::endl;
    (*cnjPob)[n] = new Poblacion(&cnjVar, &cnjOpc);
    (*cnjPob)[n]->asignaProblema(&spiralia);
    (*cnjPob)[n]->asignaSemilla(laSemilla);
    (*cnjPob)[n]->creaPoblacionInicial();
    (*cnjPob)[n]->setMetodoGeneracion(GENERACION_D);
    nIG[n] = nIT[n] = TAM_POBLACION * TAM_POBLACION * FACT_ENRIQ / ELITE_ENRIQ;
    nGE[n] = 0;
    eMI[n] = (*cnjPob)[n]->getMejorError();
    eTP[n] = (*cnjPob)[n]->getErrorMedioPoblacion();
    std::cout << "Error mejor / Error medio: " << eMI[n] << "/" << eTP[n] << std::endl;
    // Ajusta la poblacion con el mejor individuo
    if (eMI[n] < eMI[nPM])
      nPM = n;
  }
  // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
  int nGener = 0; // Numero de generaciones
  while (nGener < NUM_MAX_GENER)
  {
    std::cout << "#G" << nGener << std::endl;
    for (n = 0; n < nP; n++)
    {
      std::cout << "\tP" << n;
      nIG[n] = (*cnjPob)[n]->generacionBI();
      // nIG[n] = (*cnjPob)[n]->generacionE(5);		// Leticia sugiere el 15% de la poblacion
      eMI[n] = (*cnjPob)[n]->getMejorError();
      (*cnjPob)[n]->actualizaParametros();
      eTP[n] = (*cnjPob)[n]->getErrorMedioPoblacion();
      std::cout << " #Ind: " << nIG[n] << " Mejor: " << eMI[n] << " Media: " << eTP[n] << std::endl;
      nIT[n] += nIG[n];
      // Si se crean nuevos individuos, la generacion no es esteril
      if (nIG[n])
        nGE[n] = 0;
      else
        nGE[n]++;
      // Ajusta la poblacion con el mejor individuo
      if (eMI[n] < eMI[nPM])
        nPM = n;
    }
    std::cout << "\tMejor: " << eMI[nPM] << " [" << nPM << "]" << std::endl;
    // Tratamiento de las generaciones esteriles
    for (n = 0; n < nP; n++) // Recorre todas las poblaciones
      if (nGE[n] == 3)       // Al producirse 3 generaciones esteriles, selecciono entre los otros mejores de las otras
      {                      // poblaciones a aquel mas COMPLEMENTARIO, es decir con mayor distancia
        int nIdoneo = n;     // Numero de la poblacion con el individuo idoneo mas complementario
        Flt dMax = 0.0;      // Distancia maxima encontrada hasta el momento
        Flt d;               // Distancia actual
        int m;               // Indice

        std::cout << "NEMESIS " << n << std::endl;
        // Aqui interesa obtener al mejor individuo de las otras poblaciones que tenga el mayor numero posible de
        // puntos DE LOS QUE NO SON CLASIFICADOS por el individuo que provoca la nemesis!!!
        // Ademas ese individuo NO deberia ser peor que el actual... Pero de momento SI puede serlo
        for (m = 0; m < nP; m++) // Recorre a los mejores individuos de las restantes poblaciones
          if (n != m)            // calculando las distancias...
          {
            d = spiralia.distancia((*cnjPob)[n]->getMejorIndividuo(), (*cnjPob)[m]->getMejorIndividuo(), &cnjVar);
            if (d > dMax)
            {
              dMax = d;
              nIdoneo = m;
            }
          }
        // En caso de que exista un idoneo apropiado (en distinta posicion del que teniamos y con una distancia NO nula...)
        // Hay un peligro: Llegar a poblaciones en las que solo hay pocos tipos de individuos casi iguales, que tienen el
        // mismo error, pero cubren distintos puntos del espacio y por lo tanto tienen distintas distancias (y no nulas);
        // entonces solo se intercambiarian y la convergencia global se detendria en esos individuos...
        // Solucion que propongo para eso: Los errores medios de las poblaciones deben ser distintos...
        if (nIdoneo != n && dMax != 0.0)
        // && eTP[n] != eTP[nIdoneo])
        {
          std::cout << "Mejor antes: " << n << std::endl;
          //(*cnjPob)[n]->getMejorIndividuo()->ver();
          std::cout << "Elegido otro mejor: " << nIdoneo << std::endl;
          (*cnjPob)[n]->asignaMejorIndividuo(*((*cnjPob)[nIdoneo]));
          std::cout << "Mejor despues: " << nIdoneo << std::endl;
          //(*cnjPob)[n]->getMejorIndividuo()->ver();
        }
        /*
        if(eMI[n] > eMI[nPM])
         (*cnjPob)[n]->asignaMejorIndividuo(*((*cnjPob)[nPM]));
        */
        // En el caso de que se escoja al mismo (sea por la misma posicion o porque es igual y la distancia es 0)
        else
        {
          std::cout << "Mutacion + Destruccion..." << std::endl;
          // Cambio al mejor de esta poblacion mutandolo
          ptrIndividuo mejorActual = (*cnjPob)[n]->getMejorIndividuo();
          // ptrIndividuo mejorMutado = mejorActual->mutacion(cnjVar, cnjOpc);
          // mejorMutado->asignaPila();
          // mejorMutado->setError(spiralia.fitness(mejorMutado, &cnjVar));
          // mejorMutado->ver();
          //(*((*cnjPob)[n]))[(*cnjPob)[n]->getPosMejor()] = mejorMutado;
          //  Lo coloco en la posicion 0 para que subsista a la nemesis
          (*((*cnjPob)[n]))[0] = new Individuo(*(mejorActual->mutacion(cnjVar, cnjOpc)));
          //(*((*cnjPob)[n]))[0]->asignaPila();
          (*((*cnjPob)[n]))[0]->setError(spiralia.fitness((*((*cnjPob)[n]))[0], &cnjVar));
          (*((*cnjPob)[n]))[0]->ver();
          //(*cnjPob)[n]->getMejorIndividuo()->ver();
          if ((*cnjPob)[n]->getPosMejor() == 0)
            delete mejorActual;
          // Y extermino sin piedad al resto de los pobladores de esa poblacion
          (*cnjPob)[n]->nemesis(1);
        }
        // Desactivo la posibilidad de nemesis
        nGE[n] = 0;
      }
    nGener++;
    if (nGener == 80)
      for (n = 0; n < nP; n++)
      {
        (*cnjPob)[n]->getMejorIndividuo()->ver();
        DEBUG("Alfa");
      }

    if (nGener == NUM_MAX_GENER - 1)
    {
      int nGenAdicionales = 0;

      std::cout << "Generaciones adicionales ??? ";
      std::cin >> nGenAdicionales;
      nGener -= nGenAdicionales;
    }
  }
  tiempo.detente();
  std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
  std::cout << "Resultado final:" << std::endl;
  int nIndTotales = 0;
  for (n = 0; n < nP; n++)
  {
    std::cout << "Poblacion " << n << std::endl;
    std::cout << " Mejor: " << *((*cnjPob)[n]->getMejorIndividuo()->getArbol()) << std::endl;
    std::cout << " Error mejor: " << (*cnjPob)[n]->getMejorError() << std::endl;
    (*cnjPob)[n]->actualizaParametros();
    std::cout << " Error medio: " << (*cnjPob)[n]->getErrorMedioPoblacion() << std::endl;
    (*cnjPob)[n]->escribeResultados(funcionEscrituraExterna, (int)tiempo.detente());
    std::cout << " Individuos creados: " << nIT[n] << std::endl;
    DEBUG("Siguiente");
    nIndTotales += nIT[n];
    delete (*cnjPob)[n];
  }
  std::cout << "Numero TOTAL de individuos creados: " << nIndTotales << std::endl;
  DEBUG("Finish!");
  // Destruccion de todo...
  delete cnjPob;
}

// Resuelve Espiral
void main3()
{
  // int n;
  // Creo las operaciones basicas:
  OperacionSuma nSuma;
  OperacionResta nResta;
  OperacionProducto nProducto;
  // OperacionDivision    nDivision;	// Altamente problematica!!!
  OperacionNegativo nNegativo;
  OperacionSeno nSeno;
  OperacionCoseno nCoseno;
  OperacionCondicional nCondicional;
  // Las variables
  Variable X('X', 0.);
  Variable Y('Y', 0.);

  // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
  Conjunto<ptrNodo> cnjVar(2);
  Conjunto<ptrNodo> cnjOpc(7);
  cnjVar.agrega(&X);
  cnjVar.agrega(&Y);
  cnjOpc.agrega(&nSuma);
  cnjOpc.agrega(&nResta);
  cnjOpc.agrega(&nProducto);
  // cnjOpc.agrega(&nDivision);
  cnjOpc.agrega(&nNegativo);
  cnjOpc.agrega(&nCoseno);
  cnjOpc.agrega(&nSeno);
  cnjOpc.agrega(&nCondicional);

  time_t laSemilla = time(NULL);
  std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
  // srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

  TCrono tiempo; // Para cronometrar
  tiempo.arranca();
  // Creo el problema
  Espiral spiralia;
  // Creo un pueblo!!!
  Poblacion pueblo(&cnjVar, &cnjOpc);
  pueblo.asignaProblema(&spiralia);
  pueblo.asignaSemilla(laSemilla);
  pueblo.creaPoblacionInicial();
  pueblo.setMetodoGeneracion(GENERACION_BI);
  std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

  // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
  int nGener = 0;                       // Numero de generaciones
  int nGeneracionesEsteriles = 0;       // Numero de generaciones seguidas sin nueva descendencia
  int nIndividuos;                      // Numero de individuos nuevos producidos en la generacion actual
  int nTotalIndividuos = 0;             // Numero total de individuos producidos por el proceso evolutivo
  Flt errorMejor, errorMedio, tpcError; // Errores del mejor, medio y tanto por ciento de error relativo
  Flt errorMejorAnt = .0;               // Mejor error anterior al actual

  while (nGener < NUM_MAX_GENER)
  {
    nIndividuos = pueblo.generacionBI();
    std::cout << nIndividuos << "\n";
    // Si se crean nuevos individuos, la generacion no es esteril
    if (nIndividuos)
    {
      nTotalIndividuos += nIndividuos;
      nGeneracionesEsteriles = 0;
    }
    else
      nGeneracionesEsteriles++;
    if (nGeneracionesEsteriles == 3)
    {
      std::cout << "NEMESIS!!!" << std::endl;
      pueblo.nemesis(1);
      nGeneracionesEsteriles = 0;
    }
    nGener++;

    if (nGener == NUM_MAX_GENER - 1)
    {
      int nGenAdicionales = 0;

      std::cout << "Generaciones adicionales ??? ";
      std::cin >> nGenAdicionales;
      nGener -= nGenAdicionales;
    }

    errorMejor = pueblo.getMejorError();
    pueblo.actualizaParametros();
    errorMedio = pueblo.getErrorMedioPoblacion();
    tpcError = 100.f * errorMejor / errorMedio;
    std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
    std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
    std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
    std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
    if (errorMejorAnt != errorMejor)
    {
      std::cout << "Mejora!!!" << std::endl;
      pueblo.getMejorIndividuo()->ver();
      errorMejorAnt = errorMejor;
    }
  }
  tiempo.detente();
  std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
  std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
  pueblo.getMejorIndividuo()->ver();
  std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
  std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
  // Escribe ficheros de salida
  pueblo.escribeResultados(funcionEscrituraExterna, (int)tiempo.detente());
}


// Resuelve Parabola1
void ResuelveParabola1()
{
    // Creo las operaciones basicas:
    OperacionSuma        nSuma;
    OperacionResta       nResta;
    OperacionProducto    nProducto;
    OperacionDivision    nDivision;	// Altamente problematica!!!
    OperacionNegativo    nNegativo;
    OperacionSeno        nSeno;
    OperacionCoseno      nCoseno;
    OperacionCondicional nCondicional;
    // Las variables
    Variable X('X', 0.);
    Variable Y('Y', 0.);
    Variable Z('Z', 0.);
    Variable L('L', 0.);			// Esta variable sobra (como muchas de las operaciones)

    // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
    Conjunto<ptrNodo> cnjVar(3);
    Conjunto<ptrNodo> cnjOpc(2);
    cnjVar.agrega(&X);
    cnjVar.agrega(&Y);
    cnjVar.agrega(&Z);
    //cnjVar.agrega(&L);
    cnjOpc.agrega(&nSuma);
    //cnjOpc.agrega(&nResta);
    cnjOpc.agrega(&nProducto);
    //cnjOpc.agrega(&nDivision);
    //cnjOpc.agrega(&nNegativo);
    //cnjOpc.agrega(&nCoseno);
    //cnjOpc.agrega(&nSeno);
    //cnjOpc.agrega(&nCondicional);

    time_t laSemilla = time(NULL);
    std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
    // srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

    TCrono tiempo;			// Para cronometrar
    tiempo.arranca();
    // Creo el problema
    Parabola1 parabolia;
    // Creo un pueblo!!!
    Poblacion pueblo(&cnjVar, &cnjOpc);
    pueblo.asignaProblema(&parabolia);
    pueblo.asignaSemilla(laSemilla);
    pueblo.creaPoblacionInicial();
    pueblo.setMetodoGeneracion(GENERACION_BI);
    std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

    // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
    int nGener = 0;			// Numero de generaciones
    int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
    int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
    int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
    Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
    Flt errorMejorAnt = .0;		// Mejor error anterior al actual

    while(nGener < NUM_MAX_GENER)
    {
      nIndividuos = pueblo.generacionBI();
      std::cout << nIndividuos << "\n";
      // Si se crean nuevos individuos, la generacion no es esteril
      if(nIndividuos)
      {
      nTotalIndividuos += nIndividuos;
      nGeneracionesEsteriles = 0;
      }
      else
      nGeneracionesEsteriles++;
      if(nGeneracionesEsteriles == 3)
      {
      std::cout << "NEMESIS!!!" << std::endl;
      pueblo.nemesis(1);
      nGeneracionesEsteriles = 0;
      }
      nGener++;

      if(nGener == NUM_MAX_GENER - 1)
      {
      int nGenAdicionales = 0;

      std::cout << "Generaciones adicionales ??? ";
      std::cin  >> nGenAdicionales;
      nGener -= nGenAdicionales;
      }

      errorMejor = pueblo.getMejorError();
      pueblo.actualizaParametros();
      errorMedio = pueblo.getErrorMedioPoblacion();
      tpcError   = 100.*errorMejor/errorMedio;
      std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
      std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
      std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
      std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
      if(errorMejorAnt != errorMejor)
      {
      std::cout << "Mejora!!!" << std::endl;
      pueblo.getMejorIndividuo()->ver();
      errorMejorAnt = errorMejor;
      }
    }
    tiempo.detente();
    std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
    std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
    pueblo.getMejorIndividuo()->ver();
    std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
    std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
    // Escribe ficheros de salida
    pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}


/*
// Resuelve Rosenbrock
void main5()
{
 // int n;
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;
 // Las variables
 Variable X('X', 0.);
 Variable Y('Y', 0.);

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(2);
 Conjunto<ptrNodo> cnjOpc(5);
 cnjVar.agrega(&X);
 cnjVar.agrega(&Y);
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 //cnjOpc.agrega(&nDivision);
 cnjOpc.agrega(&nNegativo);
 cnjOpc.agrega(&nCuadrado);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 //cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Rosenbrock rosita;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&rosita);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_BI);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER)
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   std::cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
// Resuelve Gudermann
void main6()
{
 // int n;
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;
 // Las variables
 Variable X('X', 0.);

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(1);
 Conjunto<ptrNodo> cnjOpc(8);
 cnjVar.agrega(&X);
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 cnjOpc.agrega(&nDivision);
 cnjOpc.agrega(&nNegativo);
 cnjOpc.agrega(&nCuadrado);
 cnjOpc.agrega(&nCoseno);
 cnjOpc.agrega(&nSeno);
 // cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 //srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Gudermann guder;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&guder);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_BI);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER)
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   std::cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
// Resuelve Vino
void main7()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(13);
 Conjunto<ptrNodo> cnjOpc(8);

 // Las 13 variables (que mala suerte para ellas!!!)
 Variable A('A', 0.); cnjVar.agrega(&A);
 Variable B('B', 0.); cnjVar.agrega(&B);
 Variable C('C', 0.); cnjVar.agrega(&C);
 Variable D('D', 0.); cnjVar.agrega(&D);
 Variable E('E', 0.); cnjVar.agrega(&E);
 Variable F('F', 0.); cnjVar.agrega(&F);
 Variable G('G', 0.); cnjVar.agrega(&G);
 Variable H('H', 0.); cnjVar.agrega(&H);
 Variable I('I', 0.); cnjVar.agrega(&I);
 Variable J('J', 0.); cnjVar.agrega(&J);
 Variable K('K', 0.); cnjVar.agrega(&K);
 Variable L('L', 0.); cnjVar.agrega(&L);
 Variable M('M', 0.); cnjVar.agrega(&M);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 cnjOpc.agrega(&nDivision);
 cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCuadrado);
 cnjOpc.agrega(&nCoseno);
 cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Vino tinto;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&tinto);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_B);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER)
 {
  nIndividuos = pueblo.generacionB();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
void ResuelveLirio()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(4);
 Conjunto<ptrNodo> cnjOpc(5);

 // Las 4 variables correspondientes a los 4 atributos
 Variable A('A', 0.); cnjVar.agrega(&A);
 Variable B('B', 0.); cnjVar.agrega(&B);
 Variable C('C', 0.); cnjVar.agrega(&C);
 Variable D('D', 0.); cnjVar.agrega(&D);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 //cnjOpc.agrega(&nDivision);
 cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCuadrado);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Lirio lila;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&lila);
 pueblo.asignaSemilla(laSemilla);
 pueblo.setErrorAceptable(1.0);
 pueblo.creaPoblacionInicial(1000);
 pueblo.setMetodoGeneracion(GENERACION_BI);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < 30 && !pueblo.errorAlcanzado())
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == 30 - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
 pueblo.getMejorIndividuo()->ver();
   //
  //
 //
 lila.testeaProblema(pueblo.getMejorIndividuo(), &cnjVar, "lirios.dat");
   //
  //
 //
 std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());

 // Y finalmente evaluamos un fichero de test sobre el mejor individio producido
 Flt errorTest = lila.testeaProblema(pueblo.getMejorIndividuo(), &cnjVar, "lirios.dat");
 std::cout << "El error al usar el fichero de test es del " << errorTest << "%" << std::endl;
}
*/

/*
// Resuelve CancerPecho
void main9()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(30);
 Conjunto<ptrNodo> cnjOpc(6);

 // Las 30 variables correspondientes a los 30 atributos
 Variable V01('A', 0.); cnjVar.agrega(&V01);
 Variable V02('B', 0.); cnjVar.agrega(&V02);
 Variable V03('C', 0.); cnjVar.agrega(&V03);
 Variable V04('D', 0.); cnjVar.agrega(&V04);
 Variable V05('E', 0.); cnjVar.agrega(&V05);
 Variable V06('F', 0.); cnjVar.agrega(&V06);
 Variable V07('G', 0.); cnjVar.agrega(&V07);
 Variable V08('H', 0.); cnjVar.agrega(&V08);
 Variable V09('I', 0.); cnjVar.agrega(&V09);
 Variable V10('J', 0.); cnjVar.agrega(&V10);
 Variable V11('K', 0.); cnjVar.agrega(&V11);
 Variable V12('L', 0.); cnjVar.agrega(&V12);
 Variable V13('M', 0.); cnjVar.agrega(&V13);
 Variable V14('N', 0.); cnjVar.agrega(&V14);
 Variable V15('O', 0.); cnjVar.agrega(&V15);
 Variable V16('P', 0.); cnjVar.agrega(&V16);
 Variable V17('Q', 0.); cnjVar.agrega(&V17);
 Variable V18('R', 0.); cnjVar.agrega(&V18);
 Variable V19('S', 0.); cnjVar.agrega(&V19);
 Variable V20('T', 0.); cnjVar.agrega(&V20);
 Variable V21('U', 0.); cnjVar.agrega(&V21);
 Variable V22('V', 0.); cnjVar.agrega(&V22);
 Variable V23('W', 0.); cnjVar.agrega(&V23);
 Variable V24('X', 0.); cnjVar.agrega(&V24);
 Variable V25('Y', 0.); cnjVar.agrega(&V25);
 Variable V26('Z', 0.); cnjVar.agrega(&V26);
 Variable V27('a', 0.); cnjVar.agrega(&V27);
 Variable V28('b', 0.); cnjVar.agrega(&V28);
 Variable V29('c', 0.); cnjVar.agrega(&V29);
 Variable V30('d', 0.); cnjVar.agrega(&V30);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 cnjOpc.agrega(&nDivision);
 cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCuadrado);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 CancerPecho ruben;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&ruben);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_BI);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER)
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
// Resuelve Monjes1
void main10()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(6);
 Conjunto<ptrNodo> cnjOpc(6);

 // Las 30 variables correspondientes a los 30 atributos
 Variable V01('A', 0.); cnjVar.agrega(&V01);
 Variable V02('B', 0.); cnjVar.agrega(&V02);
 Variable V03('C', 0.); cnjVar.agrega(&V03);
 Variable V04('D', 0.); cnjVar.agrega(&V04);
 Variable V05('E', 0.); cnjVar.agrega(&V05);
 Variable V06('F', 0.); cnjVar.agrega(&V06);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 cnjOpc.agrega(&nDivision);
 cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCuadrado);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Monjes1 wojtila;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&wojtila);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_BI);
 pueblo.setErrorAceptable(0.0);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER && !pueblo.errorAlcanzado())
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Completadas " << pueblo.getNumGeneraciones() << " generaciones." << std::endl;
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << "Error mejor/medio: " << pueblo.getMejorError() << "/" << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Mejor individuo: " << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
void ResuelveBupa()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionNegativo    nNegativo;
 //OperacionSeno        nSeno;
 //OperacionCoseno      nCoseno;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(6);
 Conjunto<ptrNodo> cnjOpc(5);

 // 6 variables correspondientes a 6 atributos
 Variable V01('A', 0.); cnjVar.agrega(&V01);
 Variable V02('B', 0.); cnjVar.agrega(&V02);
 Variable V03('C', 0.); cnjVar.agrega(&V03);
 Variable V04('D', 0.); cnjVar.agrega(&V04);
 Variable V05('E', 0.); cnjVar.agrega(&V05);
 Variable V06('F', 0.); cnjVar.agrega(&V06);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Bupa unBupa;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&unBupa);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_BI);
 pueblo.setErrorAceptable(0.0);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER && !pueblo.errorAlcanzado())
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Completadas " << pueblo.getNumGeneraciones() << " generaciones." << std::endl;
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << "Error mejor/medio: " << pueblo.getMejorError() << "/" << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Mejor individuo: " << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
void ResuelveAbalones()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionNegativo    nNegativo;
 //OperacionSeno        nSeno;
 //OperacionCoseno      nCoseno;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(8);
 Conjunto<ptrNodo> cnjOpc(5);

 // 8 variables correspondientes a 8 atributos
 Variable V01('A', 0.); cnjVar.agrega(&V01);
 Variable V02('B', 0.); cnjVar.agrega(&V02);
 Variable V03('C', 0.); cnjVar.agrega(&V03);
 Variable V04('D', 0.); cnjVar.agrega(&V04);
 Variable V05('E', 0.); cnjVar.agrega(&V05);
 Variable V06('F', 0.); cnjVar.agrega(&V06);
 Variable V07('G', 0.); cnjVar.agrega(&V07);
 Variable V08('H', 0.); cnjVar.agrega(&V08);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Abalones losAbalones;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&losAbalones);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_BI);
 pueblo.setErrorAceptable(0.0);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER && !pueblo.errorAlcanzado())
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Completadas " << pueblo.getNumGeneraciones() << " generaciones." << std::endl;
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << "Error mejor/medio: " << pueblo.getMejorError() << "/" << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Mejor individuo: " << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
void ResuelveCompresionImagen()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(2);
 Conjunto<ptrNodo> cnjOpc(7);

 // 8 variables correspondientes a 8 atributos
 Variable V01('X', 0.); cnjVar.agrega(&V01);
 Variable V02('Y', 0.); cnjVar.agrega(&V02);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 cnjOpc.agrega(&nNegativo);
 cnjOpc.agrega(&nCoseno);
 cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 CompresionImagen Fermin;
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&Fermin);
 pueblo.asignaSemilla(laSemilla);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_BI);
 pueblo.setErrorAceptable(0.0);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER && !pueblo.errorAlcanzado())
 {
  nIndividuos = pueblo.generacionBI();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
 }
 tiempo.detente();
 std::cout << "Completadas " << pueblo.getNumGeneraciones() << " generaciones." << std::endl;
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << "Error mejor/medio: " << pueblo.getMejorError() << "/" << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Mejor individuo: " << std::endl;
 pueblo.getMejorIndividuo()->ver();
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

// Resuelve el primer ejemplo de regresion simbolica de Koza
void ResuelveKozaRegresionSimbolica1()
{
  // Creo las operaciones basicas:
  OperacionSuma nSuma;
  OperacionResta nResta;
  OperacionProducto nProducto;
  OperacionDivision nDivision; // Altamente problematica!!!
  OperacionNegativo nNegativo;
  OperacionSeno nSeno;
  OperacionCoseno nCoseno;
  OperacionCuadrado nCuadrado;
  OperacionCondicional nCondicional;
  // Las variables
  Variable X('X', 0.);

  // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
  Conjunto<ptrNodo> cnjVar(1);
  Conjunto<ptrNodo> cnjOpc(9);
  cnjVar.agrega(&X);
  cnjOpc.agrega(&nSuma);
  cnjOpc.agrega(&nResta);
  cnjOpc.agrega(&nProducto);
  // cnjOpc.agrega(&nDivision);
  // cnjOpc.agrega(&nNegativo);
  // cnjOpc.agrega(&nCuadrado);
  // cnjOpc.agrega(&nCoseno);
  // cnjOpc.agrega(&nSeno);
  // cnjOpc.agrega(&nCondicional);

  int nGeneracionesEsteriles = 0; // Numero de generaciones seguidas sin nueva descendencia

  time_t laSemilla = time(NULL);
  std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
  srand(laSemilla); // Comentando esta linea obtengo siempre lo mismo

  TCrono tiempo; // Para cronometrar
  tiempo.arranca();
  // Creo el problema
  KozaRegresionSimbolica1 krs1;
  // Los ficheros de salida de datos sobre la evolucion
  char *nomFichErrorMejor = generaNombre("eMejo000.dat");
  std::fstream fErrorMejor(nomFichErrorMejor, std::ios::out); // Fichero de error del mejor individuo
  char *nomFichErrorMedio = generaNombre("eMedi000.dat");
  std::fstream fErrorMedio(nomFichErrorMedio, std::ios::out); // Fichero de error medio de la poblacion

  // Creo un pueblo!!!
  Poblacion pueblo(&cnjVar, &cnjOpc);
  pueblo.asignaProblema(&krs1);
  pueblo.asignaSemilla(laSemilla);
  pueblo.creaPoblacionInicial(1000);
  pueblo.setMetodoGeneracion(GENERACION_B);
  pueblo.setErrorAceptable((Flt)0.000000001);
  std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

  // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
  int nGener = 0;                       // Numero de generaciones
  int nIndividuos;                      // Numero de individuos nuevos producidos en la generacion actual
  int nTotalIndividuos = 0;             // Numero total de individuos producidos por el proceso evolutivo
  Flt errorMejor, errorMedio, tpcError; // Errores del mejor, medio y tanto por ciento de error relativo
  Flt errorMejorAnt = .0;               // Mejor error anterior al actual

  while (nGener < 30 && !pueblo.errorAlcanzado())
  {
    nIndividuos = pueblo.generacionB();
    std::cout << nIndividuos << "\n";
    // Si se crean nuevos individuos, la generacion no es esteril
    if (nIndividuos)
    {
      nTotalIndividuos += nIndividuos;
      nGeneracionesEsteriles = 0;
    }
    else
      nGeneracionesEsteriles++;
    if (nGeneracionesEsteriles == 3)
    {
      std::cout << "NEMESIS!!!" << std::endl;
      pueblo.nemesis(1);
      nGeneracionesEsteriles = 0;
    }
    nGener++;
    if (nGener == 29)
    {
      int nGenAdicionales = 0;

      std::cout << "Generaciones adicionales ??? ";
      std::cin >> nGenAdicionales;
      nGener -= nGenAdicionales;
    }

    errorMejor = pueblo.getMejorError();
    pueblo.actualizaParametros();
    errorMedio = pueblo.getErrorMedioPoblacion();
    tpcError = (Flt)100. * errorMejor / errorMedio;
    std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
    std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
    std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
    std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
    if (errorMejorAnt != errorMejor)
    {
      std::cout << "Mejora!!!" << std::endl;
      pueblo.getMejorIndividuo()->ver();
      errorMejorAnt = errorMejor;
    }
    // Salva los errores de la generacion
    fErrorMejor << errorMejor << std::endl;
    fErrorMedio << errorMedio << std::endl;
    /*
    // Cada 5 generaciones, imprimo el mejor y su simplificacion
    if(nGener % 5 == 0)
    {
     std::cout << "En la generacion numero " << nGener << ", el mejor es:" << std::endl;
     pueblo.getMejorIndividuo()->ver();
     std::cout << "Y su expresion simplificada es:" << std::endl;
     pueblo.getMejorIndividuo()->verSimplificado();
     //DEBUG("Pulsa!!!");
    }
    */
  }
  tiempo.detente();
  fErrorMedio.close();
  fErrorMejor.close();
  std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
  std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
  pueblo.getMejorIndividuo()->ver();
  pueblo.getMejorIndividuo()->verSimplificado();
  std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
  std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
  // Escribe ficheros de salida
  pueblo.escribeResultados(funcionEscrituraExterna, (int)tiempo.detente());

  // Y finalmente evaluamos un fichero de test sobre el mejor individio producido
  Flt errorTest = krs1.testeaProblema(pueblo.getMejorIndividuo(), &cnjVar, "error.dat");
  std::cout << "El error al usar el test ampliado es de " << errorTest << " (RMS)" << std::endl;
}

// Resuelve el sombrerito!!!
void ResuelveSombrero()
{
  // Creo las operaciones basicas:
  OperacionSuma nSuma;
  OperacionResta nResta;
  OperacionProducto nProducto;
  OperacionDivision nDivision; // Altamente problematica!!!
  OperacionNegativo nNegativo;
  OperacionSeno nSeno;
  OperacionCoseno nCoseno;
  OperacionCuadrado nCuadrado;
  OperacionCondicional nCondicional;
  // Las variables
  Variable X('x', 0.);
  Variable Y('y', 0.);

  // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
  Conjunto<ptrNodo> cnjVar(2);
  Conjunto<ptrNodo> cnjOpc(9);
  cnjVar.agrega(&X);
  cnjVar.agrega(&Y);

  cnjOpc.agrega(&nSuma);
  cnjOpc.agrega(&nResta);
  cnjOpc.agrega(&nProducto);
  // cnjOpc.agrega(&nDivision);
  cnjOpc.agrega(&nNegativo);
  // cnjOpc.agrega(&nCuadrado);
  // cnjOpc.agrega(&nCoseno);
  // cnjOpc.agrega(&nSeno);
  cnjOpc.agrega(&nCondicional);

  int nGeneracionesEsteriles = 0; // Numero de generaciones seguidas sin nueva descendencia

  time_t laSemilla = time(NULL);
  std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
  srand(laSemilla); // Comentando esta linea obtengo siempre lo mismo

  TCrono tiempo; // Para cronometrar
  tiempo.arranca();
  // Creo el problema
  Sombrero somb;
  // Los ficheros de salida de datos sobre la evolucion
  char *nomFichErrorMejor = generaNombre("eMejo000.dat");
  std::fstream fErrorMejor(nomFichErrorMejor, std::ios::out); // Fichero de error del mejor individuo
  char *nomFichErrorMedio = generaNombre("eMedi000.dat");
  std::fstream fErrorMedio(nomFichErrorMedio, std::ios::out); // Fichero de error medio de la poblacion

  // Creo un pueblo!!!
  Poblacion pueblo(&cnjVar, &cnjOpc);
  pueblo.asignaProblema(&somb);
  pueblo.asignaSemilla(laSemilla);
  pueblo.creaPoblacionInicial(1000);
  pueblo.setMetodoGeneracion(GENERACION_BI);
  pueblo.setErrorAceptable((Flt)0.000000001);
  std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

  // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
  int nGener = 0;                       // Numero de generaciones
  int nIndividuos;                      // Numero de individuos nuevos producidos en la generacion actual
  int nTotalIndividuos = 0;             // Numero total de individuos producidos por el proceso evolutivo
  Flt errorMejor, errorMedio, tpcError; // Errores del mejor, medio y tanto por ciento de error relativo
  Flt errorMejorAnt = .0;               // Mejor error anterior al actual

  while (nGener < 30 && !pueblo.errorAlcanzado())
  {
    nIndividuos = pueblo.generacionB();
    std::cout << nIndividuos << "\n";
    // Si se crean nuevos individuos, la generacion no es esteril
    if (nIndividuos)
    {
      nTotalIndividuos += nIndividuos;
      nGeneracionesEsteriles = 0;
    }
    else
      nGeneracionesEsteriles++;
    if (nGeneracionesEsteriles == 3)
    {
      std::cout << "NEMESIS!!!" << std::endl;
      pueblo.nemesis(1);
      nGeneracionesEsteriles = 0;
    }
    nGener++;
    if (nGener == 29)
    {
      int nGenAdicionales = 0;

      std::cout << "Generaciones adicionales ??? ";
      std::cin >> nGenAdicionales;
      nGener -= nGenAdicionales;
    }

    errorMejor = pueblo.getMejorError();
    pueblo.actualizaParametros();
    errorMedio = pueblo.getErrorMedioPoblacion();
    tpcError = (Flt)100. * errorMejor / errorMedio;
    std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
    std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
    std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
    std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
    if (errorMejorAnt != errorMejor)
    {
      std::cout << "Mejora!!!" << std::endl;
      pueblo.getMejorIndividuo()->ver();
      errorMejorAnt = errorMejor;
    }
    // Salva los errores de la generacion
    fErrorMejor << errorMejor << std::endl;
    fErrorMedio << errorMedio << std::endl;
    /*
    // Cada 5 generaciones, imprimo el mejor y su simplificacion
    if(nGener % 5 == 0)
    {
     std::cout << "En la generacion numero " << nGener << ", el mejor es:" << std::endl;
     pueblo.getMejorIndividuo()->ver();
     std::cout << "Y su expresion simplificada es:" << std::endl;
     pueblo.getMejorIndividuo()->verSimplificado();
     //DEBUG("Pulsa!!!");
    }
    */
  }
  tiempo.detente();
  fErrorMedio.close();
  fErrorMejor.close();
  std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
  std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
  pueblo.getMejorIndividuo()->ver();
  pueblo.getMejorIndividuo()->verSimplificado();
  std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
  std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
  // Escribe ficheros de salida
  pueblo.escribeResultados(funcionEscrituraExterna, (int)tiempo.detente());

  // Y finalmente evaluamos un fichero de test sobre el mejor individio producido
  Flt errorTest = somb.testeaProblema(pueblo.getMejorIndividuo(), &cnjVar, 0);
  std::cout << "El error al usar el test ampliado es de " << errorTest << " (RMS)" << std::endl;
}

/*
// Resuelve el problema de clasificacion de las lentes (Lenses)
void ResuelveLentes()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(4);
 Conjunto<ptrNodo> cnjOpc(4);

 // Las 4 variables correspondientes a los 4 atributos
 Variable A('A', 0.); cnjVar.agrega(&A);
 Variable B('B', 0.); cnjVar.agrega(&B);
 Variable C('C', 0.); cnjVar.agrega(&C);
 Variable D('D', 0.); cnjVar.agrega(&D);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 //cnjOpc.agrega(&nDivision);
 //cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCuadrado);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 Lentes efe;
  // Fichero de salida de datos sobre la evolucion: error medio y mejor
 char *nomFichError = generaNombre("error000.dat");
 fstream fError(nomFichError, ios::out);
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&efe);
 pueblo.asignaSemilla(laSemilla);
 pueblo.setErrorAceptable(0.0);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_B);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER && !pueblo.errorAlcanzado())
 {
  nIndividuos = pueblo.generacionB();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   std::cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
  // Salva los errores de la generacion
  fError << errorMedio << " " << errorMejor << std::endl;
 }
 tiempo.detente();
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << " Mejor: " << std::endl;
 // << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
 pueblo.getMejorIndividuo()->ver();
 DEBUG("Pulsa!");
 pueblo.getMejorIndividuo()->verSimplificado();
 std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

/*
// Resuelve la regresion simbolica de la raiz cuadrada de x en [0, 1]
void ResuelveRaizCuadrada()
{
 // Creo las operaciones basicas:
 OperacionSuma        nSuma;
 OperacionResta       nResta;
 OperacionProducto    nProducto;
 OperacionDivision    nDivision;	// Altamente problematica!!!
 OperacionNegativo    nNegativo;
 OperacionSeno        nSeno;
 OperacionCoseno      nCoseno;
 OperacionCuadrado    nCuadrado;
 OperacionCondicional nCondicional;

 // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
 Conjunto<ptrNodo> cnjVar(1);
 Conjunto<ptrNodo> cnjOpc(5);

 // Las 4 variables correspondientes a los 4 atributos
 Variable X('X', 0.); cnjVar.agrega(&X);

 // Y las pertinentes operaciones
 cnjOpc.agrega(&nSuma);
 cnjOpc.agrega(&nResta);
 cnjOpc.agrega(&nProducto);
 // cnjOpc.agrega(&nDivision);
 cnjOpc.agrega(&nNegativo);
 //cnjOpc.agrega(&nCuadrado);
 //cnjOpc.agrega(&nCoseno);
 //cnjOpc.agrega(&nSeno);
 cnjOpc.agrega(&nCondicional);

 time_t laSemilla = 0 * time(NULL);
 std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
 srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

 TCrono tiempo;			// Para cronometrar
 tiempo.arranca();
 // Creo el problema
 RaizCuadrada rQ;

 char nomFich[20];
 strcpy(nomFich, "error000.dat");
 // Fichero de salida de datos sobre la evolucion: error medio y mejor
 char *nomFichError = generaNombre(nomFich);
 fstream fError(nomFichError, ios::out);
 // Creo un pueblo!!!
 Poblacion pueblo(&cnjVar, &cnjOpc);
 pueblo.asignaProblema(&rQ);
 pueblo.asignaSemilla(laSemilla);
 pueblo.setErrorAceptable(0.0001);
 pueblo.creaPoblacionInicial();
 pueblo.setMetodoGeneracion(GENERACION_B);
 std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

 // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
 int nGener = 0;			// Numero de generaciones
 int nGeneracionesEsteriles = 0;	// Numero de generaciones seguidas sin nueva descendencia
 int nIndividuos;			// Numero de individuos nuevos producidos en la generacion actual
 int nTotalIndividuos = 0;		// Numero total de individuos producidos por el proceso evolutivo
 Flt errorMejor, errorMedio, tpcError;  // Errores del mejor, medio y tanto por ciento de error relativo
 Flt errorMejorAnt = .0;		// Mejor error anterior al actual

 while(nGener < NUM_MAX_GENER && !pueblo.errorAlcanzado())
 {
  nIndividuos = pueblo.generacionB();
  std::cout << nIndividuos << "\n";
  // Si se crean nuevos individuos, la generacion no es esteril
  if(nIndividuos)
  {
   nTotalIndividuos += nIndividuos;
   nGeneracionesEsteriles = 0;
  }
  else
   nGeneracionesEsteriles++;
  if(nGeneracionesEsteriles == 3)
  {
   std::cout << "NEMESIS!!!" << std::endl;
   pueblo.nemesis(1);
   nGeneracionesEsteriles = 0;
  }
  nGener++;

  if(nGener == NUM_MAX_GENER - 1)
  {
   int nGenAdicionales = 0;

   std::cout << "Generaciones adicionales ??? ";
   cin  >> nGenAdicionales;
   nGener -= nGenAdicionales;
  }

  errorMejor = pueblo.getMejorError();
  pueblo.actualizaParametros();
  errorMedio = pueblo.getErrorMedioPoblacion();
  tpcError   = 100.*errorMejor/errorMedio;
  std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
  std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
  std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
  std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
  if(errorMejorAnt != errorMejor)
  {
   std::cout << "Mejora!!!" << std::endl;
   pueblo.getMejorIndividuo()->ver();
   errorMejorAnt = errorMejor;
  }
  // Salva los errores de la generacion
  fError << errorMedio << " " << errorMejor << std::endl;
 }
 tiempo.detente();
 std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
 std::cout << " Mejor: " << std::endl;
 // << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
 pueblo.getMejorIndividuo()->ver();
 DEBUG("Pulsa!");
 pueblo.getMejorIndividuo()->verSimplificado();
 std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
 std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
 // Escribe ficheros de salida
 pueblo.escribeResultados(funcionEscrituraExterna, tiempo.detente());
}
*/

// La funcion a aproximar por medio de un arbol, en este caso (x + 1)^2 = x^2 + 2*x + 1
// Aqui meto mano yo para probar si una expresion geneticamente obtenida es correcta
Flt f(const Flt &x)
{
  // return x*x + 2.*x + 1.;		// (x+1)^2 Funciona de putisima madre y rapido OK!!!
  // return x*(x*(x + 3.) + 3.) + 1.;   	// (x+1)^3 OK!!!
  // return (x + 1.)*(x + 1.)*(x + 1.);  // Es lo mismo que lo de arriba
  // return log(x*x + 1.);               // Propuesta por Pepe y de puta madre!!! OK!!!
  // return x*x*x + 32.*x*x - 25.; 		// Propuesta por Santi
  // return x*x*x + x*x - 2.*x + 1.;	// Propuesta por Bernardez OK!!!
  // return absoluto(x);			// Propuesta por el doctor Bernardez Muerte -> No va bien
  // return pow(-1., x);			// Muy mala!!!
  return (Flt)sin(x * M_PI / 180.); // Acojonante OK!!!
                                    // return (x*x*x*x)/4. - (10.*x*x*x)/3. - 100.*x*x; // MUY MALA!!!
}

/*
// Esta es la funcion externa que debe ser creada (y con estos parametros) por el
// futurible usuario de este programa...
// Es obvio que esto es ineficiente, pero da gran flexibilidad a la entrada de datos
// Una manera muy obvia de acelerar el proceso, es precalcular los valores de todas las muestras en la
// primera llamada y tenerlas almacenadas en una matriz para uso y disfrute de las postreras llamadas...
Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 // Bateria con todos los ejemplos formados por la clase (salidas) y las features (entradas)
 static bateria *bat;
 static int esPrimeraLlamada = 1;	// La primera llamada se precalcula la tabla (el resto NO)
 static int nEj, nFe;			// Numero de ejemplos y features (entradas)
 Variable *var;				// Para conectar conjunto y valores...
 Flt valorReal;				// Lo que da en realidad
 Flt valorCalculado;			// Lo que aproxima el individuo
 Flt errParcial;			// Diferencia absoluta entre los anteriores
 Flt errTotal = .0;			// Y acumulando el anterior para todos los ejemplos

 if(esPrimeraLlamada)			// Hay que cargar el fichero o precalcular...
 {
  bat = new Bateria("masterFermin.btx");
  nEj = bat->getNumEjemplos();
  nFe = bat->getNumEntradas();
  esPrimeraLlamada = 0;			// Para que no vuelva a suceder
 }

 // OJO: Para liberar correctamente la memoria empleada para almacenar la bateria,
 // hago una llamada final fitness(0, 0)
 if(pInd == 0 && pConjVariables==0)
 {
  delete bat;
  return .0;
 }

 for(int ne = 0; ne < nEj; ne++)		// Bucle de ejemplos
 {
  valorReal = bat->getSalida(ne);		// Valor autentico dado de fuera
  // Cargo una a una todas las variables con el valor de las features de entrada
  for(int nf = 0; nf < nFe; nf++)		// Bucle de features
  {
   var = (Variable *)(*pConjVariables)[nf];
   (*var) = bat->getEntrada(ne, nf);
  }
  // Y tras cargar todas las variables (implicadas o no), evaluo al individuo
  valorCalculado = pInd->evalua();
  // Me quedo con el error absoluto sin cuadrado, ya que dispara las cifras hacia arriba!!!
  errParcial = valorReal - valorCalculado;
  errParcial = absoluto(errParcial);
  errTotal += errParcial;		// Acumula error total
 }
 return errTotal;
}
*/

/*
// Un fitness para la funcion seno
Flt fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 const numMuestras = 720;
 static Flt bateria[numMuestras];
 static int esprimera = VERDAD;
 Flt x = -360.;

 if(esprimera)
 {
  for(int n = 0; n < numMuestras; n++)
  {
   bateria[n] = f(x);
   x++;
  }
 }

 x = -360.;
 Variable *X = (Variable *) (*pConjVariables)[0];
 Flt errTodasMuestras = .0;
 for(int n = 0; n < numMuestras; n++)
 {
  (*X) = x;
  errTodasMuestras += fabs(pInd->evalua() - bateria[n]);
  x++;
 }
 return errTodasMuestras;
}
*/

/*
// En este caso manipulo una imagen de 50*50 bytes con mi preciosa cara
Flt fitness2(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
 Flt errUnaMuestra;			// Error absoluto al cuadrado en un solo punto del rango
 Flt errTodasMuestras = .0;		// Suma de errores puntuales en todo el rango manejado
 Flt valA;				// Valor tras evaluar el arbol
 Flt valX;				// Valor de la variable x
 Flt valY;				// Valor de la variable y
 // La tabla de precalculo de las muestras:
 const int numMuestras = 50;
 Flt incremento = 1./numMuestras;
 static Flt tabMuestras[numMuestras][numMuestras];
 static int esPrimeraLlamada = 1;	// La primera llamada se precalcula la tabla (el resto NO)
 int i, j;

 if(esPrimeraLlamada)			// Hay que precalcular!
 {
  fstream fCara("cabeza.raw", ios::in);
  byte valPixel;

  for(i = 0; i < numMuestras; i++)
   for(j = 0; j < numMuestras; j++)
   {
    fCara >> valPixel;
    tabMuestras[i][j] = Flt(valPixel)/255.;
   }
  esPrimeraLlamada = 0;			// Ya no volvera a precalcular nunca!
  fCara.close();
 }

 // Necesito conectar las variables del conjunto con una variable que reciba los datos del rango
 Variable *X = (Variable *) (*pConjVariables)[0];
 Variable *Y = (Variable *) (*pConjVariables)[1];

 for(i = 0, valX = incremento; i < numMuestras; i++, valX += incremento)
  for(j = 0, valY = incremento; j < numMuestras; j++, valY += incremento)
  {
   // Cargamos las variables del arbol con los valores correspondientes y lo evaluamos
   (*X) = valX;
   (*Y) = valY;
   valA = pInd->evalua();
   // Y nos quedamos con el error absoluto sin cuadrado, ya que dispara las cifras hacia arriba!!!
   errUnaMuestra = tabMuestras[i][j] - valA;
   errUnaMuestra = absoluto(errUnaMuestra);
   errUnaMuestra *= errUnaMuestra;		// Al cubo
   errUnaMuestra *= errUnaMuestra;
   // Y acumulo el error total
   errTodasMuestras += errUnaMuestra;
  }
 return errTodasMuestras;
}
*/

// Funcion externa de escritura de datos finales sobre el mejor individuo que debe ser creada (con estos
// parametros y en este orden) por el futuro usuario de este programa...
void funcionEscrituraExterna(ptrIndividuo pInd, ptrConjNodos pConjVariables, std::fstream &fichero)
{
  return;
  // Necesito conectar las variables del conjunto con una variable que reciba los datos del rango
  Variable *X = (Variable *)(*pConjVariables)[0];
  Flt valX; // Valor de la variable

  // Podemos tener un cierto rango de valores de entrada para las variables contenidas en el conjunto
  // de variables dado, eso debe manejarse por medio de los bucles oportunos que CARGUEN en las variables
  // (del conjunto dado) los valores pertinentes. Es responsabilidad del programador que haya coherencia
  // entre las variables (no vaya a ser que se haga referencia a una variable erronea...)
  // Por ejemplo, si manejo un rango de entrada con valores de x en [-360, +360]
  for (valX = -360.; valX < 360; valX++)
  {
    // Cargamos las variables del arbol con los valores correspondientes y lo evaluamos
    (*X) = valX;
    fichero << valX << "\t" << f(valX) << "\t" << pInd->evalua() << std::endl;
  }

  /*
   Variable *X = (Variable *) (*pConjVariables)[0];
   Variable *Y = (Variable *) (*pConjVariables)[1];
   int i, j;
   Flt valX, valY, valA;
   int numMuestras = 50;
   Flt incremento = 1./numMuestras;

   for(i = 0, valX = incremento; i < numMuestras; i++, valX += incremento)
    for(j = 0, valY = incremento; j < numMuestras; j++, valY += incremento)
    {
     // Cargamos las variables del arbol con los valores correspondientes y lo evaluamos
     (*X) = valX;
     (*Y) = valY;
     valA = pInd->evalua();
     if(valA < 0.)
      valA = 0.;
     if(valA > 1.)
      valA = 1.;
     fichero << (byte)(255.*valA);
    }
  */
}

void resuelveSDI000()
{
  // Creo las operaciones basicas:
  OperacionSuma nSuma;
  OperacionResta nResta;
  OperacionProducto nProducto;
  OperacionDivision nDivision; // Altamente problematica!!!
  OperacionNegativo nNegativo;
  OperacionSeno nSeno;
  OperacionCoseno nCoseno;
  OperacionCuadrado nCuadrado;
  OperacionCondicional nCondicional;
  OperacionAsignacionComa nAsignacion;
  // Las variables
  Variable X('X', 0.);
  Variable V('v', 0.);

  // Creo los conjuntos de variables y operaciones (las constantes se crean solas)
  Conjunto<ptrNodo> cnjVar(2);
  Conjunto<ptrNodo> cnjOpc(10 - 2);
  cnjVar.agrega(&X);
  cnjVar.agrega(&V);

  cnjOpc.agrega(&nSuma);
  cnjOpc.agrega(&nResta);
  cnjOpc.agrega(&nProducto);
  cnjOpc.agrega(&nDivision);
  cnjOpc.agrega(&nNegativo);
  // cnjOpc.agrega(&nCuadrado);
  // cnjOpc.agrega(&nCoseno);
  cnjOpc.agrega(&nSeno);
  cnjOpc.agrega(&nCondicional);
  cnjOpc.agrega(&nAsignacion);

  int nGeneracionesEsteriles = 0; // Numero de generaciones seguidas sin nueva descendencia

  time_t laSemilla = time(NULL);
  std::cout << "Semilla aleatoria empleada:      " << laSemilla << std::endl;
  // srand(laSemilla);		// Comentando esta linea obtengo siempre lo mismo

  TCrono tiempo; // Para cronometrar
  tiempo.arranca();
  // Creo el problema
  SDI000 krs1;
  // Los ficheros de salida de datos sobre la evolucion
  char *nomFichErrorMejor = generaNombre("eMejo000.dat");
  std::fstream fErrorMejor(nomFichErrorMejor, std::ios::out); // Fichero de error del mejor individuo
  char *nomFichErrorMedio = generaNombre("eMedi000.dat");
  std::fstream fErrorMedio(nomFichErrorMedio, std::ios::out); // Fichero de error medio de la poblacion

  // Creo un pueblo!!!
  Poblacion pueblo(&cnjVar, &cnjOpc);
  pueblo.asignaProblema(&krs1);
  pueblo.asignaSemilla(laSemilla);

  Flt errorTest = 0; // krs1.testeaProblema(0, &cnjVar, "Error.dat");
  // std::cout << "El error al usar el test ampliado es de " << errorTest << " (RMS)" << std::endl;

  // Creo una poblacion de 2000 individuos, a partir de poblaciones de 2000*10 individuos de los
  // que voy tomando 500 cada vez
  pueblo.creaPoblacionInicial(2000, 10, 500);
  pueblo.setMetodoGeneracion(GENERACION_BI);
  pueblo.setErrorAceptable(1.5f);
  std::cout << "Se parte de un error inicial de: " << pueblo.getErrorMedioPoblacion() << std::endl;

  // Vamos a hacer un bucle evolutivo con las generaciones pertinentes
  int nGener = 0;                       // Numero de generaciones
  int nIndividuos;                      // Numero de individuos nuevos producidos en la generacion actual
  int nTotalIndividuos = 0;             // Numero total de individuos producidos por el proceso evolutivo
  Flt errorMejor, errorMedio, tpcError; // Errores del mejor, medio y tanto por ciento de error relativo
  Flt errorMejorAnt = .0;               // Mejor error anterior al actual

  while (nGener < 100 && !pueblo.errorAlcanzado())
  {
    nIndividuos = pueblo.generacionB();
    std::cout << nIndividuos << "\n";
    // Si se crean nuevos individuos, la generacion no es esteril
    if (nIndividuos)
    {
      nTotalIndividuos += nIndividuos;
      nGeneracionesEsteriles = 0;
    }
    else
      nGeneracionesEsteriles++;
    if (nGeneracionesEsteriles == 3)
    {
      std::cout << "NEMESIS!!!" << std::endl;
      pueblo.nemesis(1);
      nGeneracionesEsteriles = 0;
    }
    nGener++;
    if (nGener == 29)
    {
      int nGenAdicionales = 0;

      std::cout << "Generaciones adicionales ??? ";
      std::cin >> nGenAdicionales;
      nGener -= nGenAdicionales;
    }

    errorMejor = pueblo.getMejorError();
    pueblo.actualizaParametros();
    errorMedio = pueblo.getErrorMedioPoblacion();
    tpcError = (Flt)100. * errorMejor / errorMedio;
    std::cout << "#G" << nGener << " P:" << pueblo.getPosMejor();
    std::cout << " E:" << errorMejor << "/" << errorMedio << " (" << tpcError << "%)";
    std::cout << " N:" << pueblo.getMejorIndividuo()->getNumNodos() << "/" << pueblo.getNumMedioNodos();
    std::cout << " I:" << pueblo.getMejorIndividuo()->getNumIntrones() << "/" << pueblo.getNumMedioIntrones() << std::endl;
    if (errorMejorAnt != errorMejor)
    {
      std::cout << "Mejora!!!" << std::endl;
      pueblo.getMejorIndividuo()->ver();
      errorMejorAnt = errorMejor;
    }
    // Salva los errores de la generacion
    fErrorMejor << errorMejor << std::endl;
    fErrorMedio << errorMedio << std::endl;
    /*
    // Cada 5 generaciones, imprimo el mejor y su simplificacion
    if(nGener % 5 == 0)
    {
     std::cout << "En la generacion numero " << nGener << ", el mejor es:" << std::endl;
     pueblo.getMejorIndividuo()->ver();
     std::cout << "Y su expresion simplificada es:" << std::endl;
     pueblo.getMejorIndividuo()->verSimplificado();
     //DEBUG("Pulsa!!!");
    }
    */
  }
  tiempo.detente();
  fErrorMedio.close();
  fErrorMejor.close();
  std::cout << "Tiempo de evolucion: " << tiempo.detente() << " segundos." << std::endl;
  std::cout << " Mejor: " << *(pueblo.getMejorIndividuo()->getArbol()) << " : " << pueblo.getMejorError() << std::endl;
  pueblo.getMejorIndividuo()->ver();
  pueblo.getMejorIndividuo()->verSimplificado();
  std::cout << pueblo.getErrorMedioPoblacion() << std::endl;
  std::cout << "Individuos creados: " << nTotalIndividuos << std::endl;
  // Escribe ficheros de salida
  pueblo.escribeResultados(funcionEscrituraExterna, (int)tiempo.detente());

  // Y finalmente evaluamos un fichero de test sobre el mejor individio producido
  // Flt errorTest = krs1.testeaProblema(pueblo.getMejorIndividuo(), &cnjVar, "Error.dat");
  std::cout << "El error al usar el test ampliado es de " << errorTest << " (RMS)" << std::endl;
}
