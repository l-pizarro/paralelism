#pragma once
#include "stdlib.h"

// Estructura Control
//  Representación de la unidad de control (Hardware) a través de una
//  serie de valores enteros (estado) que almacena la información del
//  la unidad de control.

typedef struct Control {
  int regDst;
  int aluOp1;
  int aluOp0;
  int aluSrc;
  int branch;
  int memRead;
  int memWrite;
  int regWrite;
  int memtoReg;

} Control;


Control* crearControl();

void actualizarControl(Control** control, int tipoInstruccion);
