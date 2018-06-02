#include "control.h"

Control* control() {
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
