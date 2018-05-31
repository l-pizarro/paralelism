#pragma once
#include "stdlib.h"

// Estructura Etapa
// Representación de una etapa de Pipeline similar
// a la utilizada para los nodos de una lista enlazada
typedef struct Etapa{
  struct Etapa* siguiente;
  int id;
  char* extracto1;
  char* extracto2;
  char* extracto3;
  char* extracto4;

} Etapa;


// Entrada: - Numero entero
// Salida:	- Puntero a etapa
// Descripción: Función que solicita la memoria necesaria
// para una estructura Etapa y le asigna la identificacion
// correspondiente (id).
Etapa* crearEtapa(int id);

void liberarEtapa(Etapa* etapa);

void insertarEtapa(Etapa** pipeline, Etapa* nueveEtapa);

void removerEtapa(Etapa** pipeline);

Etapa* crearPipeline(int nroEtapas);

int largo(Etapa* pipeline);

void imprimirPipeline(Etapa* pipeline);
