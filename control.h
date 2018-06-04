#pragma once
#include "stdlib.h"

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
