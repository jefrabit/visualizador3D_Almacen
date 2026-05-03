#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>
#include "raylib.h"

// Estructura que representa un producto en el almacen
struct Producto {
    std::string nombre;    // Nombre del producto
    Vector3 posicion;      // Posicion 3D en el almacen

    // Constructor basico
    Producto() : nombre(""), posicion({0, 0, 0}) {}

    // Constructor con parametros
    Producto(const std::string& nom, float x, float y, float z) 
        : nombre(nom), posicion({x, y, z}) {}
        
    // Constructor con Vector3
    Producto(const std::string& nom, Vector3 pos) 
        : nombre(nom), posicion(pos) {}
};

#endif // PRODUCTO_H