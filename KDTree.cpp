#include "KDTree.h"
#include <cmath>
#include <algorithm>

// Constructor: inicializa la raiz como nullptr
KDTree::KDTree() : raiz(nullptr) {}

// Destructor: libera toda la memoria del arbol
KDTree::~KDTree() {
    liberarMemoria(raiz);
}

// Calcula la distancia cuadrada entre dos puntos 3D
// Usamos distancia cuadrada para evitar usar sqrt (mas eficiente)
float KDTree::distanciaCuadrada(Vector3 a, Vector3 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

// Inserta un producto en el KD-Tree
void KDTree::insertar(const Producto& prod) {
    raiz = insertarRecursivo(raiz, prod, 0);
}

// Implementacion recursiva de insercion
// Alterna entre los 3 ejes segun la profundidad
NodoKD* KDTree::insertarRecursivo(NodoKD* nodo, const Producto& prod, int profundidad) {
    // Si el nodo es nullptr, crear uno nuevo
    if (nodo == nullptr) {
        return new NodoKD(prod);
    }

    // Determinar el eje actual (0=X, 1=Y, 2=Z)
    int eje = profundidad % 3;

    // Comparar segun el eje actual
    float valorProd, valorNodo;
    if (eje == 0) {
        valorProd = prod.posicion.x;
        valorNodo = nodo->producto.posicion.x;
    } else if (eje == 1) {
        valorProd = prod.posicion.y;
        valorNodo = nodo->producto.posicion.y;
    } else {
        valorProd = prod.posicion.z;
        valorNodo = nodo->producto.posicion.z;
    }

    // Insertar en la rama correspondiente
    if (valorProd < valorNodo) {
        nodo->izquierda = insertarRecursivo(nodo->izquierda, prod, profundidad + 1);
    } else {
        nodo->derecha = insertarRecursivo(nodo->derecha, prod, profundidad + 1);
    }

    return nodo;
}

// Busca el producto mas cercano a un punto dado
Producto* KDTree::buscarMasCercano(const Vector3& punto) {
    if (raiz == nullptr) {
        return nullptr;
    }

    Producto* mejor = &(raiz->producto);
    float mejorDist = distanciaCuadrada(punto, raiz->producto.posicion);

    buscarMasCercanoRecursivo(raiz, punto, mejor, mejorDist, 0);

    return mejor;
}

// Implementacion recursiva de busqueda del vecino mas cercano
void KDTree::buscarMasCercanoRecursivo(NodoKD* nodo, const Vector3& punto, 
                                        Producto*& mejorProducto, float& mejorDistancia,
                                        int profundidad) {
    if (nodo == nullptr) {
        return;
    }

    // Calcular distancia del punto actual al nodo
    float distActual = distanciaCuadrada(punto, nodo->producto.posicion);

    // Actualizar el mejor si es necesario
    if (distActual < mejorDistancia) {
        mejorDistancia = distActual;
        mejorProducto = &(nodo->producto);
    }

    // Determinar el eje actual
    int eje = profundidad % 3;
    float diff;
    if (eje == 0) {
        diff = punto.x - nodo->producto.posicion.x;
    } else if (eje == 1) {
        diff = punto.y - nodo->producto.posicion.y;
    } else {
        diff = punto.z - nodo->producto.posicion.z;
    }

    // Determinar cual rama revisar primero
    NodoKD* primeraRama = (diff < 0) ? nodo->izquierda : nodo->derecha;
    NodoKD* segundaRama = (diff < 0) ? nodo->derecha : nodo->izquierda;

    // Revisar la primera rama (la mas probable)
    buscarMasCercanoRecursivo(primeraRama, punto, mejorProducto, mejorDistancia, profundidad + 1);

    // Revisar la segunda rama solo si puede contener un punto mas cercano
    // Esto es una optimizacion: solo necesitamos revisar si la distancia al plano
    // es menor que la mejor distancia encontrada
    if (diff * diff < mejorDistancia) {
        buscarMasCercanoRecursivo(segundaRama, punto, mejorProducto, mejorDistancia, profundidad + 1);
    }
}

// Libera toda la memoria del arbol (postorden)
void KDTree::liberarMemoria(NodoKD* nodo) {
    if (nodo == nullptr) {
        return;
    }

    // Liberar subarboles primero
    liberarMemoria(nodo->izquierda);
    liberarMemoria(nodo->derecha);

    // Liberar el nodo actual
    delete nodo;
}