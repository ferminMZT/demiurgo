/* 
  Bateria.h

  Clase creada para contener todos y cada uno de los ejemplos que se van a manejar,
  estando cada uno de ellos compuesto por un monton de caracteristicas (features)
  o atributos y unos resultados finales. Esto se usara fundamentalmente en la
  funcion de fitness...
*/

#ifndef BATERIA_H
#define BATERIA_H

#include "Utiles.h"

class Bateria
{

 public:

	// Constructor a partir de un fichero
	Bateria(char *nomFich);
	
	// Destructor: su uso es fundamental!
   ~Bateria();

    // Para obtener el numero de ejemplos
    int getNumEjemplos()   		
	{ 
		return numEjemplos; 
	}

	// Para obtener el numero de features o entradas
	int getNumAtributos()   		
	{ 
		return numAtributos; 
	}

	// Para obtener el numero de salidas o clases
	int getNumClases()   			
	{ 
		return numClases; 
	}

	// Para obtener una feature o valor de entrada
	Flt getAtributo(int nEje, int nAtr)	
	{ 
		return atributos[nEje][nAtr]; 
	}

	// Para obtener un valor real de salida, una clase
	Flt getClase(int nEje, int nCla = 0)	
	{ 
		return clases[nEje][nCla]; 
	}


 private:
	  
	// Numero de ejemplos a los que se enfrenta cada individuo
	int numEjemplos;

	// Numero de features de entrada para cada ejemplo
    int numAtributos;

	// Numero de salidas producidas, es decir de clases (para un clasificador)
    int numClases;

	// Los atributos, datos de entrada (que se usan para clasificar...)
    Flt **atributos;

	// Resultados de salida REALES de la clasificacion
    Flt **clases;			

};

#endif // BATERIA_H