/* 
  TCrono.h
  Cabecera con la definicion de la clase TCrono, usada para realizar medidas
  de tiempos de ejecucion de mis programas
*/

#if !defined(TCRONO_H)	// Evita inclusiones multiples
#define TCRONO_H


class TCrono
{
	 
 public:

	// Constructor (unico) que pone todo a 0
	TCrono()			
	{
		t_inic = t_acum = 0;
		funcionando = false;     	// De momento esta parado
	}

	// Inicia o continua el cronometrado
	float arranca()		
	{
		if(!funcionando)		// Si estaba parado,
		{
			funcionando = true;			// lo pongo a andar y
			//t_inic = GetTickCount();	// tomo el instante de arranque
		}
   
		return t_acum*.001F;   	// Devuelvo tiempo acumulado en segundos
	}

	// Lo detiene momentaneamente	
	float detente()		
	{
		if(funcionando)  		// Si esta cronometrando,
		{						// lo detengo y recojo el tiempo acumulado
			funcionando = false;	   
			//t_acum += GetTickCount() - t_inic;
		}
		// En todo caso, funcionando o no, devuelvo el ultimo tiempo acumulado
		return t_acum*.001F;
	}

	// Lo finaliza e instaura a cero
	float resetea()		
	{
		float t_final = t_acum*.001F;

		if(funcionando)
		{
			//t_final += (GetTickCount() - t_inic)*.001F;
			funcionando = false;
		}
		t_inic = t_acum = 0;
		return t_final;
	}


 private:

	// Tiempo inicial al arrancar el crono
	unsigned int t_inic;			
	// Tiempo acumulado entre 2 lapsos sucesivos
	unsigned int t_acum;			
	// Estado del crono (false parado, true midiendo)
	bool funcionando;		

};

#endif // TCRONO_H