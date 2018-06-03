#include "control.h"

Control* createControl() {
  Control* control = (Control*)malloc(sizeof(Control));

  control->regDst = 0;
  control->aluOp1 = 0;
  control->aluOp0 = 0;
  control->aluSrc = 0;
  control->branch = 0;
  control->memRead = 0;
  control->memWrite = 0;
  control->regWrite = 0;
  control->memtoReg = 0;

  return control;
}

void actualizarControl(Control** control, int tipoInstruccion) {
  if (tipoInstruccion < 7) {
    (*control)->regDst   = 1;
    (*control)->aluOp1   = 1;
    (*control)->aluOp0   = 0;
    (*control)->aluSrc   = 0;
    (*control)->branch   = 0;
    (*control)->memRead  = 0;
    (*control)->memWrite = 0;
    (*control)->regWrite = 1;
    (*control)->memtoReg = 0;
  }

  else if (tipoInstruccion == 7) {
    (*control)->regDst   = 0;
    (*control)->aluOp1   = 0;
    (*control)->aluOp0   = 0;
    (*control)->aluSrc   = 1;
    (*control)->branch   = 0;
    (*control)->memRead  = 1;
    (*control)->memWrite = 0;
    (*control)->regWrite = 1;
    (*control)->memtoReg = 1;
  }

  else if (tipoInstruccion == 8) {
    (*control)->aluOp1   = 0;
    (*control)->aluOp0   = 0;
    (*control)->aluSrc   = 1;
    (*control)->branch   = 0;
    (*control)->memRead  = 0;
    (*control)->memWrite = 1;
    (*control)->regWrite = 0;
  }

  else if (tipoInstruccion == 9) {
    (*control)->aluOp1   = 0;
    (*control)->aluOp0   = 1;
    (*control)->aluSrc   = 0;
    (*control)->branch   = 1;
    (*control)->memRead  = 0;
    (*control)->memWrite = 0;
    (*control)->regWrite = 0;
  }
  else {

  }
}
