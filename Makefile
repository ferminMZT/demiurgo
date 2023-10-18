# Definición de variables
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Lista de archivos fuente .cpp
SOURCES = \
	./Problemas/KozaRegresionSimbolica1.cpp \
	./Problemas/Gudermann.cpp \
	./Problemas/Sombrero.cpp \
	./Problemas/KozaRegresionSimbolica1.h \
	./Problemas/Lentes.h \
	./Problemas/Monjes1.h \
	./Problemas/Abalones.h \
	./Problemas/Carro.cpp \
	./Problemas/Vino.h \
	./Problemas/Espiral.cpp \
	./Problemas/CompresionImagen.cpp \
	./Problemas/CancerPecho.cpp \
	./Problemas/Abalones.cpp \
	./Problemas/RaizCuadrada.cpp \
	./Problemas/Vino.cpp \
	./Problemas/Lentes.cpp \
	./Problemas/RaizCuadrada.h \
	./Problemas/Espiral.h \
	./Problemas/SDI000.h \
	./Problemas/Rosenbrock.h \
	./Problemas/SDI000.cpp \
	./Problemas/CompresionImagen.h \
	./Problemas/Monjes1.cpp \
	./Problemas/Bupa.h \
	./Problemas/Lirio.cpp \
	./Problemas/Lirio.h \
	./Problemas/CancerPecho.h \
	./Problemas/Carro.h \
	./Problemas/Rosenbrock.cpp \
	./Problemas/Problema.h \
	./Problemas/Sombrero.h \
	./Problemas/Gudermann.h \
	./Problemas/Parabola1.cpp \
	./Problemas/Parabola1.h \
	./Problemas/Bupa.cpp \
	./TCrono.h \
	./EntradaPila.h \
	./Individuo.cpp \
	./Conjunto.cpp \
	./Bateria.h \
	./Piposeco.cpp \
	./Poblacion.cpp \
	./Bateria.cpp \
	./Constante.h \
	./Piposeco.h \
	./Utiles.cpp \
	./Individuo.h \
	./Variable.h \
	./Poblacion.h \
	./Arbol.h \
	./Utiles.h \
	./Conjunto.h \
	./Arbol.cpp \
	./Operaciones/OperacionProducto.h \
	./Operaciones/Operacion.h \
	./Operaciones/OperacionCuadrado.h \
	./Operaciones/OperacionSeno.h \
	./Operaciones/OperacionNegativo.h \
	./Operaciones/_OperacionAsignacion.h \
	./Operaciones/OperacionResta.h \
	./Operaciones/OperacionPuntoComaAsignacion.h \
	./Operaciones/OperacionDivision.h \
	./Operaciones/OperacionSuma.h \
	./Operaciones/OperacionCondicional.h \
	./Operaciones/OperacionCoseno.h \
	./Nodo.h

# Generar nombres de archivos objeto a partir de los archivos fuente
OBJECTS = $(SOURCES:.cpp=.o)

# Regla de construcción de archivos objeto
%.o: %.cpp \
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Archivo con main(), que no puede estar entre los SOURCES.
MAIN = ./Demiurgo.cpp

# Nombre del ejecutable
EXECUTABLE = demiurgo.out

# Regla de construcción del ejecutable
$(EXECUTABLE): $(OBJECTS) $(MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Limpieza de archivos objeto y ejecutable
clean:
	rm -f $(EXECUTABLE)

# Regla por defecto
all: $(EXECUTABLE)

.PHONY: clean all

