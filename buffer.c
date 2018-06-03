#include "buffer.h"
#include "stdio.h"
IFID* ifId() {
  IFID* buffer = (IFID*)malloc(sizeof(IFID));

  return buffer;
}

IDEX* idEx() {
  IDEX* buffer = (IDEX*)malloc(sizeof(IDEX));

  return buffer;
}

EXMEM* exMem() {
  EXMEM* buffer = (EXMEM*)malloc(sizeof(EXMEM));

  return buffer;
}

MEMWB* memWb() {
  MEMWB* buffer = (MEMWB*)malloc(sizeof(MEMWB));

  return buffer;
}

void printIFID(IFID* buffer) {
  printf("[ IF/ID  ] >  Ex:%6s  Ex:%5s  Ex:%5s  Ex:%5s",buffer->extracto1,buffer->extracto2,buffer->extracto3,buffer->extracto4);
  printf("  PC:%5d  PC+4:%3d\n", buffer->instruccionActual,buffer->instruccionSgte);
}
void printIDEX(IDEX* buffer) {
  printf("[ ID/EX  ] >  Ins:%5d  rs:%5d  rt:%5d  rd:%5d",buffer->instruccion,buffer->registroRs,buffer->registroRt,buffer->registroRd);
  printf("  Sig:%4d\n", buffer->signoExtendido);
}
void printEXMEM(EXMEM* buffer) {
  printf("[ EX/MEM ] >  Alu:%5d  Dir:%4d  rs:%5d  rt:%5d  rd:%5d\n", buffer->aluResultado, buffer->direccionMemoria, buffer->registroRs, buffer->registroRt, buffer->registroRd);
}
void printMEMWB(MEMWB* buffer) {
  printf("[ MEM/WB ] >  Alu:%5d  Reg:%4d  DaM:%4d\n", buffer->aluResultado, buffer->registroDestino, buffer->dataMemoria);
}
