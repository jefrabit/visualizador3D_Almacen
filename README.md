# Visualizador 3D de Almacén con KD-Tree

## Descripción del Proyecto

Aplicación gráfica en 3D desarrollada en C++ utilizando la librería **raylib** que permite visualizar productos dentro de un almacén y encontrar el producto más cercano a la posición de un trabajador utilizando un **KD-Tree** (K-Dimensional Tree) en 3 dimensiones.

## Objetivo

Demostrar la implementación y uso práctico de estructuras de datos avanzadas (KD-Tree) para la búsqueda eficiente del vecino más cercano en un espacio tridimensional, aplicando conceptos del curso de Estructuras de Datos Avanzadas.

## Tecnologías Usadas

- **Lenguaje:** C++ (estándar C++11)
- **Librería gráfica:** raylib
- **Plataforma:** Linux

## ¿Qué es un KD-Tree?

El **KD-Tree** (K-Dimensional Tree) es una estructura de datos de tipo árbol binario que subddivide el espacio k-dimensional recursivamente. Es extremadamente útil para realizar búsquedas de vecino más cercano en espacios multidimensionales.

### ¿Cómo funciona en 3D?

1. **Inserción alternada por ejes:**
   - Nivel 0: Compara coordenadas **X** (izquierda/derecha)
   - Nivel 1: Compara coordenadas **Y** (arriba/abajo)
   - Nivel 2: Compara coordenadas **Z** (profundidad)
   - Nivel 3: Vuelve a **X** (ciclo)

2. **Búsqueda del vecino más cercano:**
   - Se recorre el árbol recursivamente
   - Se compara la distancia actual con la mejor encontrada
   - Se decide qué rama explorar primero basada en el eje actual
   - Solo se explora la otra rama si puede contener un punto más cercano (optimización del "hiperplano")

### Complejidad Algorítmica

- **Inserción:** O(log n) promedio
- **Búsqueda del vecino más cercano:** O(log n) promedio
- **Peor caso:** O(n) cuando el árbol está desbalanceado

### Significado de las Coordenadas

En el contexto del almacén:
- **X**: Eje horizontal (pasillo izquierda/derecha)
- **Y**: Eje vertical (altura/nivel)
- **Z**: Eje de profundidad (pasillo adelante/atrás)

## Visualización

El programa muestra:

1. **Grid del almacén:** Superficie de trabajo con divisiones por zonas
2. **Paredes perimetrales:** Límites del almacén
3. **Zonas organizadas:** 
   - Zona de alimentos (X: 0-4, Z: 0-4)
   - Zona de alimentos secundarios (X: 4-8, Z: 0-4)
   - Zona de limpieza (X: 8-12, Z: 0-4)
   - Zona de bebidas y otros (X: 0-4, Z: 4-8)
4. **Productos:** Cubos de color azul (20 productos)
5. **Trabajador:** Cilindro + esfera de color verde
6. **Producto más cercano:** Cubo de color rojo resaltado con borde amarillo
7. **Línea guía:** Líneaamarilla que conecta el trabajador con el producto más cercano
8. **Paneles informativos:** Título, controles, posición, distancia y datos del producto

### Productos Incluidos (20 productos)

| # | Producto    | Posición (X, Y, Z) | Zona         |
|---|-----------|-------------------|--------------|
| 1 | Arroz      | (1, 1, 1)      | Alimentos   |
| 2 | Azúcar    | (2, 1, 2)      | Alimentos   |
| 3 | Leche     | (3, 1, 1)      | Alimentos   |
| 4 | Fideos    | (1, 1, 3)      | Alimentos   |
| 5 | Aceite    | (2, 1, 4)      | Alimentos   |
| 6 | Galletas  | (5, 1, 1)      | Alimentos 2 |
| 7 | Café      | (6, 1, 2)      | Alimentos 2 |
| 8 | Harina    | (7, 1, 3)      | Alimentos 2 |
| 9 | Sal       | (5, 1, 4)      | Alimentos 2 |
|10 | Cereal    | (6, 1, 4)      | Alimentos 2 |
|11 | Detergente | (9, 1, 1)      | Limpieza   |
|12 | Jabón     | (10, 1, 2)     | Limpieza   |
|13 | Shampoo   | (11, 1, 1)     | Limpieza   |
|14 | Papel     | (9, 1, 3)       | Limpieza   |
|15 | Conservas | (10, 1, 4)     | Limpieza   |
|16 | Agua      | (1, 1, 5)      | Bebidas    |
|17 | Yogurt    | (2, 1, 6)      | Bebidas    |
|18 | Atún      | (3, 1, 5)       | Bebidas    |
|19 | Pan       | (1, 1, 7)      | Bebidas    |
|20 | Huevos    | (2, 1, 8)      | Bebidas    |

## Controles del Programa

| Tecla | Acción                              |
|------|-------------------------------------|
| W    | Mover trabajador hacia adelante (Z-)  |
| S    | Mover trabajador hacia atrás (Z+)       |
| A    | Mover trabajador a la izquierda (X-)   |
| D    | Mover trabajador a la derecha (X+)    |
| Q    | Subir trabajador (Y+)               |
| E    | Bajar trabajador (Y-)                |
| T    | Reiniciar posición del trabajador       |
| R    | Reiniciar cámara                    |
| ESC  | Cerrar programa                  |
| Mouse| Rotar cámara orbital              |

## Cómo Compilar

1. Asegúrate de tener instalado raylib:

   ```bash
   # En Debian/Ubuntu
   sudo apt-get install raylib libraylib-dev

   # Instalación desde código fuente
   cd /tmp/raylib/src
   make
   sudo make install
   sudo ldconfig
   ```

2. Compila el proyecto:

   ```bash
   make
   ```

   Esto generará el ejecutable `almacen3d`.

## Cómo Ejecutar

```bash
./almacen3d
```

## Relación con el Curso de Estructuras de Datos Avanzadas

Este proyecto aplica los siguientes conceptos del curso:

1. **KD-Tree:** Estructura de datos multidimensional para búsqueda espacial
2. **Recursividad:** Implementación de algoritmos recursivos para insertar y buscar
3. **Árboles binarios:** Estructura fundamental del KD-Tree
4. **Complejidad algorítmica:** Análisis O(log n) vs O(n) para búsquedas
5. **Optimización espacial:** Reducción del espacio de búsqueda con "planos de corte"

### ¿Por qué usar KD-Tree en lugar de búsqueda lineal?

Si tuviéramos 10,000 productos y quisiéramos encontrar el más cercano a un punto:
- **Búsqueda lineal**: Comparar cada producto = 10,000 operaciones
- **KD-Tree**: Promedio de ~log2(10,000) = ~14 operaciones

La diferencia es significativa para almacenes grandes.

## Estructura de Archivos

```
visualizador3D_Almacen/
├── main.cpp       # Programa principal con visualización raylib
├── Producto.h     # Estructura de datos del producto
├── KDTree.h      # Definición de la clase KD-Tree
├── KDTree.cpp    # Implementación del KD-Tree
├── Makefile      # Compilación para Linux
└── README.md    # Este archivo
```

## Explicación del Código para la Exposición

### Puntos clave para explicar:

1. **Producto.h:** Estructura simple que combina nombre con posición 3D (Vector3 de raylib)

2. **KDTree.h:** 
   - Define la estructura NodoKD con punteros izquierda/derecha
   - La profundidad determina el eje de comparación (profundidad % 3)

3. **KDTree.cpp:**
   - Las funciones usan distancia cuadrada (no sqrt) por eficiencia
   - La búsqueda revisa primero la rama más prometedora
   - Solo explora la otra rama si puede tener algo más cerca

4. **main.cpp:**
   - Cada frame recalcula el producto más cercano usando el KD-Tree
   - Esto demuestra la eficiencia de O(log n) vs una búsqueda lineal O(n)

### ¿Cómo demostrar que el KD-Tree funciona?

1. Agregar prints de debug en la búsqueda para ver cuántos nodos visita
2. Mover el trabajador muy cerca de un producto y observar el cambio
3. Comparar el tiempo con vs sin KD-Tree (para muchos productos)

## Autores

Proyecto desarrollado para el curso de Estructuras de Datos Avanzadas.

## Licencia

Para fines educativos.