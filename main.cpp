#include "raylib.h"
#include "KDTree.h"
#include <vector>
#include <string>
#include <cmath>

// ================================================
// CONSTANTES
// ================================================

const int ANCHO_VENTANA = 1200;
const int ALTO_VENTANA = 800;
const float VELOCIDAD = 0.25f;
const float VELOCIDAD_GIRO = 3.0f;
const float ALMACEN_ANCHO = 20.0f;
const float ALMACEN_LARGO = 18.0f;
const float ALMACEN_ALTO = 6.0f;

const int NUM_FILAS = 4;
const int NUM_COLS = 4;
const int NUM_NIVELES = 4;
const int PRODUCTOS_POR_NIVEL = 3;  // 3 productos por nivel de repisa

// ================================================
// VARIABLES GLOBALES DE ESTADO
// ================================================

bool modoPrimeraPersona = false;
float anguloVision = 0.0f;  // Angulo de rotacion en primera persona

// ================================================
// DIBUJO DEL ESCENARIO
// ================================================

void dibujarPiso() {
    // Piso con cuadricula
    DrawCube({ALMACEN_ANCHO/2, -0.15f, ALMACEN_LARGO/2}, 
             ALMACEN_ANCHO, 0.3f, ALMACEN_LARGO, {180, 180, 180, 255});
    
    for (float x = 0; x <= ALMACEN_ANCHO; x += 2) {
        DrawLine3D({x, 0.02f, 0}, {x, 0.02f, ALMACEN_LARGO}, {200, 200, 100, 80});
    }
    for (float z = 0; z <= ALMACEN_LARGO; z += 2) {
        DrawLine3D({0, 0.02f, z}, {ALMACEN_ANCHO, 0.02f, z}, {200, 200, 100, 80});
    }
}

void dibujarParedes() {
    Color paredGris = {100, 100, 100, 255};
    
    DrawCube({-0.3f, 3.0f, ALMACEN_LARGO/2}, 0.6f, 6.0f, ALMACEN_LARGO, paredGris);
    DrawCube({ALMACEN_ANCHO + 0.3f, 3.0f, ALMACEN_LARGO/2}, 0.6f, 6.0f, ALMACEN_LARGO, paredGris);
    DrawCube({ALMACEN_ANCHO/2, 3.0f, -0.3f}, ALMACEN_ANCHO, 6.0f, 0.6f, paredGris);
    DrawCube({ALMACEN_ANCHO/2, 3.0f, ALMACEN_LARGO + 0.3f}, ALMACEN_ANCHO, 6.0f, 0.6f, paredGris);
}

void dibujarRepisa(float x, float z, int nivelMax) {
    Color posteColor = {45, 35, 25, 255};
    Color bandejaColor = {150, 110, 70, 255};
    
    float ancho = 2.2f;
    float profundo = 1.8f;
    float alturaNivel = 1.3f;
    float alturaTotal = nivelMax * alturaNivel;
    
    // 4 postes
    DrawCube({x - ancho/2 + 0.08f, alturaTotal/2, z - profundo/2 + 0.08f}, 0.12f, alturaTotal + 0.2f, 0.12f, posteColor);
    DrawCube({x + ancho/2 - 0.08f, alturaTotal/2, z - profundo/2 + 0.08f}, 0.12f, alturaTotal + 0.2f, 0.12f, posteColor);
    DrawCube({x - ancho/2 + 0.08f, alturaTotal/2, z + profundo/2 - 0.08f}, 0.12f, alturaTotal + 0.2f, 0.12f, posteColor);
    DrawCube({x + ancho/2 - 0.08f, alturaTotal/2, z + profundo/2 - 0.08f}, 0.12f, alturaTotal + 0.2f, 0.12f, posteColor);
    
    // Bandejas
    for (int nivel = 0; nivel < nivelMax; nivel++) {
        DrawCube({x, nivel * alturaNivel + 0.06f, z}, ancho - 0.15f, 0.08f, profundo - 0.15f, bandejaColor);
    }
    
    // Tope
    DrawCube({x, alturaTotal + 0.1f, z}, ancho, 0.08f, profundo, posteColor);
}

void dibujarEstanterias() {
    float posX[] = {3.5f, 7.5f, 12.5f, 17.0f};
    float posZ[] = {2.5f, 6.0f, 10.0f, 14.5f};
    
    for (int i = 0; i < NUM_FILAS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            dibujarRepisa(posX[i], posZ[j], NUM_NIVELES);
        }
    }
}

void dibujarProducto(Vector3 pos, bool esCercano) {
    float escala = esCercano ? 0.35f : 0.26f;
    Color color = esCercano ? Color{255, 50, 50, 255} : Color{55, 115, 215, 255};
    
    // Sombra
    DrawCube({pos.x, pos.y - escala/2 - 0.01f, pos.z}, escala * 1.1f, 0.02f, escala * 1.1f, {100, 100, 100, 80});
    
    // Producto
    DrawCube(pos, escala, escala, escala, color);
    DrawCubeWires(pos, escala, escala, escala, BLACK);
    
    if (esCercano) {
        DrawCubeWires(pos, escala + 0.06f, escala + 0.06f, escala + 0.06f, YELLOW);
        DrawSphere({pos.x, pos.y + escala/2 + 0.06f, pos.z}, 0.07f, YELLOW);
    }
}

void dibujarTrabajador(Vector3 pos) {
    // Sombra
    DrawCylinder(pos, 0.4f, 0.4f, 0.02f, 8, {70, 70, 70, 100});
    
    // Cuerpo
    DrawCylinder(pos, 0.28f, 0.32f, 0.9f, 16, {45, 170, 75, 255});
    DrawCylinder({pos.x, pos.y + 0.3f, pos.z}, 0.25f, 0.28f, 0.3f, 16, {35, 140, 60, 255});
    
    // Cabeza
    DrawSphere({pos.x, pos.y + 0.85f, pos.z}, 0.22f, {55, 190, 85, 255});
    
    // Casco
    DrawCylinder({pos.x, pos.y + 1.1f, pos.z}, 0.2f, 0.15f, 0.15f, 12, LIME);
    
    // Indicador
    DrawSphere({pos.x, pos.y + 1.3f, pos.z}, 0.06f, YELLOW);
}

void dibujarLinea(Vector3 desde, Vector3 hasta) {
    Vector3 fin = hasta;
    fin.y += 0.2f;
    DrawLine3D(desde, fin, ORANGE);
    DrawSphere(fin, 0.12f, ORANGE);
    DrawCylinder({desde.x, desde.y, desde.z}, 0.18f, 0.18f, 0.02f, 8, GREEN);
}

// ================================================
// PUNTERO/cROSSHAIR PARA PRIMERA PERSONA
// ================================================

void dibujarCrosshair() {
    int centroX = ANCHO_VENTANA / 2;
    int centroY = ALTO_VENTANA / 2;
    
    // Fondo transparente del crosshair
    Color crossColor = {255, 255, 255, 200};
    
    // Circulo exterior
    DrawCircleLines(centroX, centroY, 20, crossColor);
    
    // Lineas cruz
    DrawLine(centroX - 25, centroY, centroX - 8, centroY, crossColor);
    DrawLine(centroX + 8, centroY, centroX + 25, centroY, crossColor);
    DrawLine(centroX, centroY - 25, centroX, centroY - 8, crossColor);
    DrawLine(centroX, centroY + 8, centroX, centroY + 25, crossColor);
    
    // Punto central
    DrawCircle(centroX, centroY, 3, crossColor);
}

void dibujarPanelInfo(const std::string& nombreProd, Vector3 posTrab, Vector3 posProd, int total) {
    DrawRectangle(15, 15, 320, 220, {250, 250, 250, 240});
    DrawRectangleLines(15, 15, 320, 220, DARKGRAY);
    DrawRectangle(15, 15, 320, 5, {50, 100, 200, 255});
    
    int y = 35;
    DrawText("ALMACEN 3D - KD-Tree", 25, y, 22, BLACK);
    y += 28;
    DrawText("Estructuras de Datos Avanzadas", 25, y, 12, GRAY);
    y += 25;
    DrawLine(25, y, 325, y, LIGHTGRAY);
    y += 18;
    
    char posStr[100];
    sprintf(posStr, "Trabajador: (%.1f, %.1f, %.1f)", posTrab.x, posTrab.y, posTrab.z);
    DrawText(posStr, 25, y, 15, BLACK);
    y += 24;
    
    DrawText("PRODUCTO MAS CERCANO:", 25, y, 15, BLACK);
    y += 22;
    DrawText(nombreProd.c_str(), 25, y, 22, RED);
    y += 28;
    
    float dx = posTrab.x - posProd.x;
    float dy = posTrab.y - posProd.y;
    float dz = posTrab.z - posProd.z;
    float dist = sqrt(dx*dx + dy*dy + dz*dz);
    
    sprintf(posStr, "Distancia: %.2f m", dist);
    DrawText(posStr, 25, y, 14, DARKGRAY);
    sprintf(posStr, "Productos: %d", total);
    DrawText(posStr, 200, y, 14, DARKGRAY);
}

void dibujarControles() {
    int y = ALTO_VENTANA - 155;
    
    DrawRectangle(15, y, 280, 140, {250, 250, 250, 240});
    DrawRectangleLines(15, y, 280, 140, DARKGRAY);
    DrawRectangle(15, y, 280, 5, {50, 180, 80, 255});
    
    y += 20;
    DrawText("CONTROLES", 25, y, 18, BLACK);
    y += 28;
    DrawText("W / S     - Adelante / Atras", 25, y, 13, DARKGRAY);
    y += 20;
    DrawText("A / D     - Izquierda / Derecha", 25, y, 13, DARKGRAY);
    y += 20;
    DrawText("Q / E     - Subir / Bajar", 25, y, 13, DARKGRAY);
    y += 20;
    DrawText("Flechas   - Mirar alrededor", 25, y, 13, DARKGRAY);
    y += 20;
    DrawText("T         - Reiniciar posicion", 25, y, 13, DARKGRAY);
    DrawText("V         - Cambiar POV", 25, y, 13, RED);
}

void dibujarIndicadorPOV() {
    if (modoPrimeraPersona) {
        // Indicador de modo primera persona
        DrawRectangle(ANCHO_VENTANA - 180, 15, 165, 35, {50, 50, 50, 220});
        DrawRectangleLines(ANCHO_VENTANA - 180, 15, 165, 35, ORANGE);
        DrawText("MODO PRIMERA PERSONA", ANCHO_VENTANA - 170, 25, 16, ORANGE);
        
        // Instrucciones de giro
        DrawRectangle(ANCHO_VENTANA - 220, 55, 205, 40, {200, 200, 200, 200});
        DrawRectangleLines(ANCHO_VENTANA - 220, 55, 205, 40, DARKGRAY);
        DrawText("Flechas izq/der para mirar", ANCHO_VENTANA - 210, 65, 13, DARKGRAY);
        DrawText("WASD para caminar", ANCHO_VENTANA - 210, 82, 12, GRAY);
    } else {
        // Indicador de tercera persona
        DrawRectangle(ANCHO_VENTANA - 200, 15, 185, 35, {50, 50, 50, 220});
        DrawRectangleLines(ANCHO_VENTANA - 200, 15, 185, 35, GREEN);
        DrawText("MODO TERCERA PERSONA", ANCHO_VENTANA - 190, 25, 16, GREEN);
    }
    
    // Crosshair en primera persona
    if (modoPrimeraPersona) {
        dibujarCrosshair();
    }
    
    // KD-Tree indicator
    DrawRectangle(ANCHO_VENTANA - 260, ALTO_VENTANA - 40, 245, 30, {50, 50, 50, 200});
    DrawText("KD-Tree 3D | Complejidad O(log n)", ANCHO_VENTANA - 250, ALTO_VENTANA - 32, 14, LIGHTGRAY);
}

// ================================================
// GENERAR PRODUCTOS COMPLETO
// ================================================

std::vector<Producto> crearTodosLosProductos() {
    std::vector<Producto> productos;
    
    // Posiciones base de las filas
    float posX[] = {3.5f, 7.5f, 12.5f, 17.0f};
    float posZ[] = {2.5f, 6.0f, 10.0f, 14.5f};
    
    // Nombres de productos por categoria
    std::vector<std::string> alimentos = {
        "Arroz", "Fideos", "Avena", "Cereal", "Harina", "Sal", "Azucar", "Miel",
        "Leche", "Yogurt", "Queso", "Crema", "Mantequilla", "Huevos", "Pan",
        "Galletas", "Chocolate", "Cafe", "Te", "Canela", "Especias", "Ajo",
        "Pasta", "Pure", "Sopa", "Caldo", "Consome", "Mayonesa", "Ketchup"
    };
    
    std::vector<std::string> limpieza = {
        "Detergente", "Jabon", "Shampoo", "Papel", "Lavavajilla", "Lejia",
        "Desinfectante", "Cloro", "Cloro", "Esponja", "Trapeador", "Escoba",
        "Aire", "Limpiavidrios", "Limpiahormig", "Desodorante"
    };
    
    std::vector<std::string> bebidas = {
        "Agua", "Gaseosa", "Jugo", "Cerveza", "Vino", "Refresco",
        "Energizante", "Te Frio", "Agua 2L", "Agua 1L"
    };
    
    std::vector<std::string> conservas = {
        "Atun", "Sardinas", "Maiz", "Chicharos", "Frijoles", "Conservas",
        "Salsa", "Mermelada", "Mani", "Nueces", "Pasas", "Datiles"
    };
    
    std::vector<std::string> otros = {
        "Pasta Dental", "Cepillo", "Enjuague", "Razor", "Crema",
        "Panales", "Toallas", "Algodon", "Curitas", "Termometro"
    };
    
    int idxAlimentos = 0;
    int idxLimpieza = 0;
    int idxBebidas = 0;
    int idxConservas = 0;
    int idxOtros = 0;
    
    // Generar productos para cada repisa, cada nivel
    for (int fila = 0; fila < NUM_FILAS; fila++) {
        for (int col = 0; col < NUM_COLS; col++) {
            float baseX = posX[fila];
            float baseZ = posZ[col];
            
            for (int nivel = 0; nivel < NUM_NIVELES; nivel++) {
                float alturaY = nivel * 1.3f + 0.5f;
                
                // 3 productos por nivel
                for (int prodNivel = 0; prodNivel < PRODUCTOS_POR_NIVEL; prodNivel++) {
                    float offsetX = -0.6f + prodNivel * 0.6f;
                    float offsetZ = (rand() % 100) / 100.0f * 0.8f;
                    
                    std::string nombre;
                    
                    // Seleccionar categoria basada en fila
                    if (fila == 0) {
                        // Fila de alimentos
                        nombre = alimentos[idxAlimentos % alimentos.size()];
                        idxAlimentos++;
                    } else if (fila == 1) {
                        // Fila de limpieza
                        nombre = limpieza[idxLimpieza % limpieza.size()];
                        idxLimpieza++;
                    } else if (fila == 2) {
                        // Fila de bebidas y conservas
                        if (nivel < 2) {
                            nombre = bebidas[idxBebidas % bebidas.size()];
                            idxBebidas++;
                        } else {
                            nombre = conservas[idxConservas % conservas.size()];
                            idxConservas++;
                        }
                    } else {
                        // Fila de otros y mas alimentos
                        if (nivel < 2) {
                            nombre = otros[idxOtros % otros.size()];
                            idxOtros++;
                        } else {
                            nombre = alimentos[idxAlimentos % alimentos.size()];
                            idxAlimentos++;
                        }
                    }
                    
                    // Agregar sufijo unico
                    char sufijo[20];
                    sprintf(sufijo, "-%d", (int)productos.size() + 1);
                    nombre += sufijo;
                    
                    float posX = baseX + offsetX;
                    float posZ = baseZ + offsetZ;
                    
                    productos.push_back(Producto(nombre, posX, alturaY, posZ));
                }
            }
        }
    }
    
    return productos;
}

// ================================================
// MOVIMIENTO
// ================================================

void moverTrabajador(Vector3& pos) {
    float dx = 0, dz = 0;
    
    if (IsKeyDown(KEY_W)) dz -= VELOCIDAD;
    if (IsKeyDown(KEY_S)) dz += VELOCIDAD;
    if (IsKeyDown(KEY_A)) dx -= VELOCIDAD;
    if (IsKeyDown(KEY_D)) dx += VELOCIDAD;
    if (IsKeyDown(KEY_Q)) pos.y += VELOCIDAD;
    if (IsKeyDown(KEY_E)) pos.y -= VELOCIDAD;
    
    // Girar en primera persona
    if (modoPrimeraPersona) {
        if (IsKeyDown(KEY_LEFT)) anguloVision += VELOCIDAD_GIRO * 0.02f;
        if (IsKeyDown(KEY_RIGHT)) anguloVision -= VELOCIDAD_GIRO * 0.02f;
    }
    
    // Aplicar movimiento
    if (modoPrimeraPersona) {
        // En primera persona, moverse en la direccion de la vista
        float nx = pos.x + dx * cos(anguloVision) - dz * sin(anguloVision);
        float nz = pos.z + dx * sin(anguloVision) + dz * cos(anguloVision);
        pos.x = nx;
        pos.z = nz;
    } else {
        // Tercera persona normal
        pos.x += dx;
        pos.z += dz;
    }
    
    // Limites
    if (pos.x < 0.5f) pos.x = 0.5f;
    if (pos.x > ALMACEN_ANCHO - 0.5f) pos.x = ALMACEN_ANCHO - 0.5f;
    if (pos.z < 0.5f) pos.z = 0.5f;
    if (pos.z > ALMACEN_LARGO - 0.5f) pos.z = ALMACEN_LARGO - 0.5f;
    if (pos.y < 0.5f) pos.y = 0.5f;
    if (pos.y > ALMACEN_ALTO) pos.y = ALMACEN_ALTO;
}

// ================================================
// MAIN
// ================================================

int main() {
    InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "Almacen 3D - KD-Tree");
    
    Camera3D camera;
    camera.position.x = 14.0f;
    camera.position.y = 12.0f;
    camera.position.z = 16.0f;
    camera.target.x = 10.0f;
    camera.target.y = 2.0f;
    camera.target.z = 9.0f;
    camera.up.x = 0.0f;
    camera.up.y = 1.0f;
    camera.up.z = 0.0f;
    camera.fovy = 55.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    Vector3 posicionInicial = {6.0f, 1.5f, 9.0f};
    Vector3 posicionTrabajador = posicionInicial;
    
    // Crear productos y KD-Tree
    std::vector<Producto> productos = crearTodosLosProductos();
    KDTree* tree = new KDTree();
    for (const auto& prod : productos) {
        tree->insertar(prod);
    }
    
    Producto* productoCercano = tree->buscarMasCercano(posicionTrabajador);
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        // Entrada
        moverTrabajador(posicionTrabajador);
        
        if (IsKeyPressed(KEY_T)) {
            posicionTrabajador = posicionInicial;
            anguloVision = 0.0f;
        }
        
        // Cambiar entre primera y tercera persona
        if (IsKeyPressed(KEY_V)) {
            modoPrimeraPersona = !modoPrimeraPersona;
        }
        
        if (IsKeyPressed(KEY_R)) {
            camera.position.x = 14.0f;
            camera.position.y = 12.0f;
            camera.position.z = 16.0f;
            anguloVision = 0.0f;
        }
        
        // Rotar camara en tercera persona
        if (!modoPrimeraPersona && (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON))) {
            UpdateCamera(&camera, CAMERA_ORBITAL);
        }
        
        // Buscar con KD-Tree
        productoCercano = tree->buscarMasCercano(posicionTrabajador);
        
        // Actualizar camara en primera persona
        if (modoPrimeraPersona) {
            camera.target = posicionTrabajador;
            camera.position.x = posicionTrabajador.x - sin(anguloVision) * 0.5f;
            camera.position.y = posicionTrabajador.y + 0.3f;
            camera.position.z = posicionTrabajador.z - cos(anguloVision) * 0.5f;
        }
        
        // Dibujo
        BeginDrawing();
            ClearBackground({210, 210, 210, 255});
            
            BeginMode3D(camera);
                dibujarPiso();
                dibujarParedes();
                dibujarEstanterias();
                
                // Productos
                for (const auto& prod : productos) {
                    bool esCercano = (productoCercano != nullptr && 
                                   prod.nombre == productoCercano->nombre);
                    dibujarProducto(prod.posicion, esCercano);
                }
                
                // Trabajador solo en tercera persona
                if (!modoPrimeraPersona) {
                    dibujarTrabajador(posicionTrabajador);
                }
                
                // Linea al producto cercano
                if (productoCercano != nullptr && !modoPrimeraPersona) {
                    dibujarLinea(posicionTrabajador, productoCercano->posicion);
                }
                
            EndMode3D();
            
            // Paneles 2D
            dibujarPanelInfo(productoCercano ? productoCercano->nombre : "Ninguno", 
                         posicionTrabajador, 
                         productoCercano ? productoCercano->posicion : posicionTrabajador,
                         (int)productos.size());
            dibujarControles();
            dibujarIndicadorPOV();
            
        EndDrawing();
    }
    
    delete tree;
    CloseWindow();
    
    return 0;
}