#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecturaArchivos.h"
#include "registro.h"
#include "buffer.h"
#include "control.h"

void obtenerTipoInstruccion(char* instruccion, int* tipo);

void ejecutar(char** instrucciones, int nroInstrucciones, Registro** registros);

// Entrada: - No posee
// Salida:	- No posee
// Descripción: Este procedimiento realiza la llamada al resto de
// funciones y procedimientos en un correcto orden
void iniciar();

// add $t1, $t1, $t2
// add $t1, $t1, $t3
// add $t1, $t1, $t4
// add $t1, $t1, $t5
// sw $t1, 0($t2)
// add $t1, $t1, $t5
// add $t1, $t1, $t5
// add $t1, $t1, $t5
// sw $t1, 0($t2)
// lw $s7, 0($t2)


// add $t1, $t1, $t2
// add $t1, $t1, $t3
// j End
// add $t1, $t1, $t2
// End:
// add $t1, $t1, $t2
