#ifndef KDTREE_H
#define KDTREE_H

#include <string>
#include "Producto.h"

// Estructura para un nodo del KD-Tree
struct NodoKD {
    Producto producto;     // Producto almacenado en este nodo
    NodoKD* izquierda;    // Puntero al subarbol izquierdo
    NodoKD* derecha;       // Puntero al subarbol derecho

    NodoKD(const Producto& prod) : producto(prod), izquierda(nullptr), derecha(nullptr) {}
};

// Clase KD-Tree para buscar productos cercanos en 3D
class KDTree {
private:
    NodoKD* raiz;          // Raiz del arbol

    // Metodos privados(recursivos)
    NodoKD* insertarRecursivo(NodoKD* nodo, const Producto& prod, int profundidad);
    void buscarMasCercanoRecursivo(NodoKD* nodo, const Vector3& punto, 
                                   Producto*& mejorProducto, float& mejorDistancia, 
                                   int profundidad);
    void liberarMemoria(NodoKD* nodo);

public:
    // Constructor y destructor
    KDTree();
    ~KDTree();

    // Metodos publicos
    void insertar(const Producto& prod);
    Producto* buscarMasCercano(const Vector3& punto);

    // Funcion auxiliar: distancia cuadrada entre dos puntos 3D
    static float distanciaCuadrada(Vector3 a, Vector3 b);
};

#endif // KDTREE_H