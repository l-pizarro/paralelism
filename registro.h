#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura Registro
//  Representación de un registro (Hardware) a través de un
// valor entero (estado) que almacena la información del
// registro y su respectivo nombre

typedef struct Registro {
  int valor;
  int largo;
  int* memoria;
  char* nombre;
} Registro;

// Entrada: - No posee
// Salida:	- Puntero a registro
// Descripción: Esta función solicita espacio en memoria para
// los 32 registros que utiliza el programa.
Registro* crearRegistros(int nroRegistros, char** infoRegistros);

// Entrada: - Puntero a registro
// Salida:	- No posee
// Descripción: Procedimiento que libera la memoria solicitada para
// los 32 registros utilizados por el programa.
void liberarRegistros(Registro* r);
