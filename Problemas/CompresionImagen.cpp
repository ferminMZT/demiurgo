#include "CompresionImagen.h"
#include <math.h>
#include <fstream>

CompresionImagen::CompresionImagen()
{
    nombre = "CompresionImagen: El sueño pitagorico de reducir la belleza a una formula";
    xTam   = 25;
    yTam   = 25;
    xTamInv = 1. / xTam;
    yTamInv = 1. / yTam;
    invNumPixels = 255./((Flt)xTam * (Flt)yTam);
    // Asigno memoria para la imagen en 2 pasos
    pixels = new Byte*[yTam];	// Primero las yTam lineas
    if(!pixels)			// Casca por no poder asignar memoria para las lineas
    {
        DEBUG("Error!");
        std::cout << "No asigna memoria a las lineas de la imagen" << std::endl;
        exit(-1);
    }
    for(y = 0; y < yTam; y++)	// Despues cada linea columna a columna
    {
        pixels[y] = new Byte[xTam];
        if(!pixels[y])		// Casca por no poder asignar memoria a una linea entera
        {
            DEBUG("Error!");
            std::cout << "No asigna memoria a la linea " << y << std::endl;
            exit(-1);
        }
    }
    // Ya hay memoria asignada y todo va bien!!!
    // Vamos a cargar el fichero de imagen
    std::fstream fImg("imagenFermin50.raw", std::ios::in);
    Byte unByte;
    for(y = 0; y < yTam; y++)	// Linea a linea
        for(x = 0; x < xTam; x++)	// Columna a columna
        {
            fImg >> unByte;
            pixels[y][x] = unByte;
        }
    fImg.close();
}

CompresionImagen::~CompresionImagen()
{
    for(y = 0; y < yTam; y++)
        delete [] pixels[y];
    delete [] pixels;
    x = y = xTam = yTam = 0;
    pilaEvaluacion = 0;
}

Flt CompresionImagen::fitness(ptrIndividuo pInd, ptrConjNodos pConjVariables)
{
    // Se construye la pila piposeco a partir del individuo dado, sobre la cual se evaluara todo
    pilaEvaluacion = new Piposeco(pInd->getArbol(), pInd->getNumNodos());
    errTotalImagen = 0.;
    // Conecto variables
    Variable *X = (Variable *) (*pConjVariables)[0];
    Variable *Y = (Variable *) (*pConjVariables)[1];
    // Se procesan todos los pixels
    for(y = 0; y < yTam; y++)	// Linea a linea
        for(x = 0; x < xTam; x++)	// Columna a columna
          {
              (*X) = x * xTamInv;
              (*Y) = y * yTamInv;
              valEvaluado = pilaEvaluacion->evalua();
              // Mi primer intento de usar un fitness aleatorio!!!
              //if(COC())
              // Esto va medio bien pero es extremadamente lento!!!
              // Me limito a devolver la diferencia entre la edad obtenida y la real.
              // errTotalImagen += fabs(valEvaluado - pixels[y][x] * 0.00392156862745098);
              errTotalImagen += fabs(valEvaluado - pixels[y][x] * 0.00392156862745098);
              //else
              // Esto va a primar que se acierte plenamente en un punto (con un error de 0.01)
              //{
              // if(fabs(valEvaluado - pixels[y][x] * 0.00392156862745098) < .01)
              //  errTotalImagen -= 10;
              // else
              //  errTotalImagen += 5;
              //}
        }
    delete pilaEvaluacion;
    return errTotalImagen * invNumPixels;
}