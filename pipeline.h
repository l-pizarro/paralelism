#pragma once
#include "stdlib.h"

// Estructura Etapa
// Representación de una etapa de Pipeline similar
// a la utilizada para los nodos de una lista enlazada
typedef struct Etapa{
  int id;
  struct Etapa* siguiente;
} Etapa;


// Entrada: - Numero entero
// Salida:	- Puntero a etapa
// Descripción: Función que solicita la memoria necesaria
// para una estructura Etapa y le asigna la identificacion
// correspondiente (id).
Etapa* crearEtapa(int id);

void eliminarEtapa(Etapa* etapa);

void insertarEtapa(Etapa* pipeline, int id);

Etapa* removerEtapa(Etapa* pipeline);

Etapa* crearPipeline();
