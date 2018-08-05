#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Entrada: - Puntero a número entero
//          - Numero entero
// Salida:	- Arreglo de strings
// Descripción: Función que abre el archivo solitado por
// el usuario y almacena en un arreglo de punteros a
// caracteres cada una de las lineas presentes en el
// archivo. Además modifica el valor del entero al cual
// apunta el puntero de entrada con la cantidad de lineas.
char** leerArchivo(int* cantidadLineas, int tipo, int* validez);
