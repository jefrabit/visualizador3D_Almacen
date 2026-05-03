# Makefile para el Visualizador 3D de Almacen con KD-Tree
# Compilador: g++
# Libreria: raylib

COMPILADOR = g++
FLAGS = -Wall -Wextra -pedantic -std=c++11
ENLACES = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Archivos fuente
FUENTES = main.cpp KDTree.cpp
OBJETOS = $(FUENTES:.cpp=.o)
EJECUTABLE = almacen3d

# Objetivo principal
all: $(EJECUTABLE)

# Compilar el ejecutable
$(EJECUTABLE): $(OBJETOS)
	$(COMPILADOR) $(FLAGS) $(OBJETOS) -o $(EJECUTABLE) $(ENLACES)

# Compilar archivos objeto
%.o: %.cpp
	$(COMPILADOR) $(FLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJETOS) $(EJECUTABLE)

# Ejecutar el programa
run: $(EJECUTABLE)
	./$(EJECUTABLE)

# Regenerar todo desde cero
rebuild: clean all

.PHONY: all clean run rebuild