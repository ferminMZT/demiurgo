/**
  Piposeco.cpp
  Implementacion de la clase Piposeco, de la pila polaca semi constante
*/

#include <math.h>

#include "Piposeco.h"

Piposeco::~Piposeco()			// Destructor
{
 // Los espacios intermedios para calculos de operaciones deben borrarse
 // asi como los espacios de operaciones simplificados a constantes (param2 == 1)
 for(int n = 0; n < numEntradas; n++)
  if(entradas[n].tipoNodo != VARIABLE)
  {
   // Si es una constante legitima, no hace falta borrarla
   if(entradas[n].tipoNodo == CONSTANTE && !entradas[n].param2)
    continue;
   if(entradas[n].valorNodo)	// Por si borro algo al simplificar...
    delete entradas[n].valorNodo;
  }
 // Borra todo
 if(entradas)
 {
	delete [] entradas;
	entradas = 0;
 }
 numEntradas = 0;
}

// Una cosa algo tonta pero que puede reportar un considerable incremento
// en la velocidad de ejecucion es la simplificacion de entradas constantes
// dentro de la pila. Por ejemplo:
//
// 	Indice:	tipoNodo: valorNodo param2 param3
//	i+1       k            3      _      _
//	i         k            2      _      _
//      i-1       +           ...    i+1     _
//
// Se podria simplificar a algo como:
//
// 	Indice:	tipoNodo: valorNodo param2 param3
//	i+1       k            3      _      _
//	i         k            2      _      _
//      i-1       k         5 = 2+3   _      _
//
// Y se reduciria notablemente el tiempo de ejecucion!!!
// De momento esto parece destruir los objetos perfectamente, pero podria exprimirse aun
// mas la gestion de memoria, pero de cara al tiempo de ejecucion creo que no interesa...
void Piposeco::simplifica()
{
	int n = numEntradas - 1;	// Indice de la pila
	
	while(n >= 0)			// Hasta recorrer totalmente la pila
	{
		// Cuando se topa con una operacion, debo comprobar si sus n parametros
		// son absolutamente todos constantes y en tal caso simplificar
		// Ademas como empieza por arriba, solo me preocupo de aquellas cosas con aridad no nula,
		// que es seguro que son operaciones (simplificables o no) y nunca constantes...
		if(entradas[n].aridad)
		{
			switch(entradas[n].tipoNodo)
			{
				// Unarias: Si el parametro es CONSTANTE, pre-calculo la operacion y
				// almaceno este resultado dentro de la propia operacion, la cual pasa
				// a convertirse en una constante
				case NEGATIVO:
					if(entradas[n+1].tipoNodo == CONSTANTE)
					{
						// Pre-calculo
						*(entradas[n].valorNodo) = - *(entradas[n+1].valorNodo);
						// Y convierte esta entrada en una constante
						entradas[n].tipoNodo = CONSTANTE;
						// Y aviso de que no es una constante legitima (para el destructor...)
						// Como? Pues simplemente poniendo el segundo parametro como 1 (cosa imposible)
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;

				case SENO:
					if(entradas[n+1].tipoNodo == CONSTANTE)
					{
						// Pre-calculo
						*(entradas[n].valorNodo) = (Flt) sin(*(entradas[n+1].valorNodo));
						// Y convierte esta entrada en una constante
						entradas[n].tipoNodo = CONSTANTE;
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;

				case COSENO:
					if(entradas[n+1].tipoNodo == CONSTANTE)
					{
						// Pre-calculo
						*(entradas[n].valorNodo) = (Flt) cos(*(entradas[n+1].valorNodo));
						// Y convierte esta entrada en una constante
						entradas[n].tipoNodo = CONSTANTE;
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;

				case CUADRADO:
					if(entradas[n+1].tipoNodo == CONSTANTE)
					{
						// Pre-calculo
						*(entradas[n].valorNodo) = *(entradas[n+1].valorNodo) * *(entradas[n+1].valorNodo);
						// Y convierte esta entrada en una constante
						entradas[n].tipoNodo = CONSTANTE;
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;

				// Binarias: Vere si ambos parametros son constantes...
				case SUMA:
					if(entradas[n+1].tipoNodo == CONSTANTE && entradas[entradas[n].param2].tipoNodo == CONSTANTE)
					{
						// Precalcula
						*(entradas[n].valorNodo) = *(entradas[n+1].valorNodo) + *(entradas[entradas[n].param2].valorNodo);
						// Y cambia el tipo de esta entrada
						entradas[n].tipoNodo = CONSTANTE;
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;
				
				case RESTA:
					if(entradas[n+1].tipoNodo == CONSTANTE && entradas[entradas[n].param2].tipoNodo == CONSTANTE)
					{
						*(entradas[n].valorNodo) = *(entradas[n+1].valorNodo) - *(entradas[entradas[n].param2].valorNodo);
						entradas[n].tipoNodo = CONSTANTE;
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;
				
				case PRODUCTO:
					if(entradas[n+1].tipoNodo == CONSTANTE && entradas[entradas[n].param2].tipoNodo == CONSTANTE)
					{
						*(entradas[n].valorNodo) = *(entradas[n+1].valorNodo) * *(entradas[entradas[n].param2].valorNodo);
						entradas[n].tipoNodo = CONSTANTE;
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;
				
				case DIVISION:		// Ojo, que esta protegida
					if(entradas[n+1].tipoNodo == CONSTANTE && entradas[entradas[n].param2].tipoNodo == CONSTANTE)
					{
						*(entradas[n].valorNodo) = (Flt) ((esCero(*(entradas[entradas[n].param2].valorNodo))) ? .0 : *(entradas[n+1].valorNodo) / *(entradas[entradas[n].param2].valorNodo));
						entradas[n].tipoNodo = CONSTANTE;
						entradas[n].param2 = 1;
						entradas[n].aridad = 0;
					}
					break;
				
				// Ternarias: El caso del condicional es algo mas complicado
				case CONDICIONAL:
					// Solo si la condicion IF(...) es constante podremos seguir
					if(entradas[n+1].tipoNodo == CONSTANTE)
					{
						// Si la condicion se cumple y la parte THEN es constante
						if(*(entradas[n+1].valorNodo) > 0)
						{
							if(entradas[entradas[n].param2].tipoNodo == CONSTANTE)
							{
								*(entradas[n].valorNodo) = *(entradas[entradas[n].param2].valorNodo);
								entradas[n].tipoNodo = CONSTANTE;
								entradas[n].param2 = 1;
								entradas[n].aridad = 0;
							}
						}
						else	// Le toca a la parte else
						{
							if(entradas[entradas[n].param3].tipoNodo == CONSTANTE)
							{
								*(entradas[n].valorNodo) = *(entradas[entradas[n].param3].valorNodo);
								entradas[n].tipoNodo = CONSTANTE;
								entradas[n].param2 = 1;
								entradas[n].aridad = 0;
							}
						}
					}
					break;
				
				// El operador ...; var = ... es algo complicado
				case ASIGNACIONCOMA:
					{
						// cout << "Simplificando..." << endl;;

					}
					break;


			} // switch(entradas[n].tipoNodo)
		} // if(entradas[n].aridad)
		n--;
	} // while(n >= 0)
}

// Un programador profesional esta siempre a la altura de las circunstancias:
// Aqui se necesita ABSOLUTAMENTE toda la VELOCIDAD que pueda conseguir...
Flt Piposeco::evalua()
{
	Flt *param1, *param2, *param3;

	// Empiezo a recorrer la pila por la parte de arriba hasta topar con una operacion
	// es decir algo que no es ni CONSTANTE ni VARIABLE y voy evaluandolo...
	// Empiezo en numEntradas - 2 ya que en numEntradas - 1 hay algo que es cte o variable
	// y eso ahorra algo menos de un .5%
	for(int n = numEntradas - 2; n >= 0; n--)
	{
		// Solo tiene en cuenta nodos con aridad (operaciones)
		if(entradas[n].aridad)
		{
			// De momento sabemos que al menos se va a usar el primer parametro
			param1 = entradas[n+1].valorNodo;

			// Reordeno la secuencia de ifs para que las operaciones binarias, que son las mas
			// numerosas, sean evaluadas antes, y solo tengan que sufrir una evaluacion de if
			// Esta tonteria eleva la velocidad casi en un 5% !!!
			if(entradas[n].aridad == 2)		// O bien ~entradas[n].aridad & 1 que tarda lo mismo
			{
				param2 = entradas[entradas[n].param2].valorNodo;

				switch(entradas[n].tipoNodo)
				{
					case SUMA:
						*(entradas[n].valorNodo) = *param1 + *param2;
						goto SIG;

					case RESTA:
						*(entradas[n].valorNodo) = *param1 - *param2;
						goto SIG;

					case PRODUCTO:
						*(entradas[n].valorNodo) = *param1 * *param2;
						goto SIG;

					case DIVISION:
						*(entradas[n].valorNodo) = (Flt) ((esCero(*param2)) ? .0 : *param1 / *param2);
						goto SIG;
				}
			}

			// Las operaciones unarias son las segundas mas numerosas y pueden esperar 2 ifs...
			if(entradas[n].aridad == 1)
			{
				switch(entradas[n].tipoNodo)	// Ejecuta las operaciones unarias
				{
					 case NEGATIVO:
						 *(entradas[n].valorNodo) = - *(param1);
						 goto SIG;

					 case SENO:
						 *(entradas[n].valorNodo) = (Flt) sin(*(param1));
						 goto SIG;

					 case COSENO:
						 *(entradas[n].valorNodo) = (Flt) cos(*(param1));
						 goto SIG;

					 case CUADRADO:
						 *(entradas[n].valorNodo) = *(param1) * *(param1);
						 goto SIG;
				}
			}

			// Finalmente las operaciones ternarias, que son escasas
			param2 = entradas[entradas[n].param2].valorNodo;
			param3 = entradas[entradas[n].param3].valorNodo;
			
			switch(entradas[n].tipoNodo)	// Ejecuta las operaciones ternarias
			{
				case CONDICIONAL:
					*(entradas[n].valorNodo) = ( *(param1) > 0) ? (*(param2)) : (*(param3));
					goto SIG;

				case ASIGNACIONCOMA:
					// Esta es bastante problematica
					*param1 = *param2;
					// Tachan!!! Ya esta, que se evalue a lo que hay al principio!!!
					*(entradas[n].valorNodo) = *param3;
					goto SIG;

//				case ASIGNACION:
//					// Esta es bastante problematica
//					*param2 = *param3;
//					// Tachan!!! Ya esta, que se evalue a lo que hay al principio!!!
//					*(entradas[n].valorNodo) = *param1;
//					goto SIG;

			}
			
		}
SIG: ;
	}
	// Y aqui esta destilada la esencia de toda la sabiduria encerrada en el individuo...
	return *(entradas[0].valorNodo);
}

// Tan solo sirve para visualizar en pantalla la expresión simplificada de la pila.
// Imprime la expresión en in-orden, por lo cual funciona recursivamente...
void Piposeco::verSimplificado(int nPipo)
{
	// Voy a imprimir la pila en inorden!!!, comenzando por el primer operando; para ello
	// se recorrera la pila desde abajo (0) hasta arriba.
	
	// Casos basicos, una constante o una variable
	if(entradas[nPipo].aridad == 0)
	{
		if(entradas[nPipo].tipoNodo == CONSTANTE)
			cout << *(entradas[nPipo].valorNodo);
		else
			// Se supone que solo puede ser una variable, pero surge un problema,
			// ¿como acceder a su nombre? -> Al crear la pila en apilar meto el nombre
			// char -> int en su dmc parametro2
			cout << (char)entradas[nPipo].param2;
	}
	else
		// Caso recursivo, nos enfrentamos a una operacion
		if(entradas[nPipo].aridad)
		{
			// De momento sabemos que al menos se va a usar el primer parametro
			if(entradas[nPipo].aridad == 2)	// O bien ~entradas[n].aridad & 1 que tarda lo mismo
			{
				// Si no hay un terminal colgando del operando, se le pone entre ()
				int noHayTerminal = (entradas[nPipo + 1].aridad != 0);
				
				// Vemos el primer operando (a continuacion)
				if(noHayTerminal)
					cout << "(";

				verSimplificado(nPipo + 1);

				if(noHayTerminal)
					cout << ") ";

				// Vemos la operacion
				switch(entradas[nPipo].tipoNodo)
				{
					case SUMA:
						cout << "+";
						break;
					case RESTA:
						cout << "-";
						break;
					case PRODUCTO:
						cout << "*";
						break;
					case DIVISION:
						cout << "/";
						break;
				}

				// Y vemos el segundo operando (en la posicion param2)
				noHayTerminal = (entradas[entradas[nPipo].param2].aridad != 0);
				
				if(noHayTerminal)
					cout << " (";
				
				verSimplificado(entradas[nPipo].param2);
				
				if(noHayTerminal)
					cout << ") ";
			}
			else
				// Las operaciones unarias
				if(entradas[nPipo].aridad == 1)
				{
					// Imprime primero el operador
					switch(entradas[nPipo].tipoNodo)
					{
						case NEGATIVO:
							cout << " -";
							break;
						case SENO:
							cout << " sin";
							break;
						case COSENO:
							cout << " cos";
							break;
						case CUADRADO:
							cout << " pow";
							break;
					}

					// Y luego el operando
					cout << "(";

					verSimplificado(nPipo + 1);

					if(entradas[nPipo].tipoNodo == CUADRADO)
						cout << ", 2.";
					
					cout << ")";
				}
				// El IFGZTE, operador ternario (op1) ? (op2) : (op3)
				// // O el operador ;=             (op1) ; (op2) = (op3)
				// O el operador =,             (op1) = (op2) , (op3)
				else
				{
					if(entradas[nPipo].tipoNodo == CONDICIONAL)
					{
						cout << "(";
						verSimplificado(nPipo + 1);
						cout << " > 0) ? (";
						verSimplificado(entradas[nPipo].param2);
						cout << ") : (";
						verSimplificado(entradas[nPipo].param3);
						cout << ")";
					}
					else
						if(entradas[nPipo].tipoNodo == ASIGNACIONCOMA)
						{
//						// Aqui va el ;=
//						verSimplificado(nPipo + 1);
//						cout << ";\n";
//						verSimplificado(entradas[nPipo].param2);
//						cout << " = ";
//						verSimplificado(entradas[nPipo].param3);

							// Aqui va el =,
							cout << "(";
							verSimplificado(nPipo + 1);
							cout << " = (";
							verSimplificado(entradas[nPipo].param2);
							cout << ")), (";
							verSimplificado(entradas[nPipo].param3);
							cout << ")";
						}
				}
		}
}