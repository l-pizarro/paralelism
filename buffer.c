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
  printf("[ IF/ID ] >  %s %s %s %s",buffer->extracto1,buffer->extracto2,buffer->extracto3,buffer->extracto4);
  printf(" PC: %d  PC+4: %d\n", buffer->instruccionActual,buffer->instruccionSgte);
}
void printIDEX(IDEX* buffer) {
  printf("[ ID/EX ] >  Inst:%d rs:%d rt:%d rd:%d",buffer->instruccion,buffer->registroRs,buffer->registroRt,buffer->registroRd);
  printf(" Sign: %d\n", buffer->signoExtendido);
}
void printEXMEM(EXMEM* buffer) {
  printf("[ EX/MEM ] >  Alu:%d Direc:%d rs:%d rt%d rd:%d\n", buffer->aluResultado, buffer->direccionMemoria, buffer->registroRs, buffer->registroRt, buffer->registroRd);
}
void printMEMWB(MEMWB* buffer) {
  printf("[ MEM/WB ]  >  Alu:%d Reg:%d DataMem:%d\n", buffer->aluResultado, buffer->registroDestino, buffer->dataMemoria);
}
