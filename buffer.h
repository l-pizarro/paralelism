#pragma once
#include "stdlib.h"

typedef struct IFID {
  int   instruccionActual;
  int   instruccionSgte;
  char* extracto1;
  char* extracto2;
  char* extracto3;
  char* extracto4;

} IFID;

typedef struct IDEX {
  int instruccion;
  int registroRs;
  int registroRt;
  int registroRd;
  int signoExtendido;

  // Control
  int regDst;
  int aluOp0;
  int aluOp1;
  int aluSrc;
  int branch;
  int memRead;
  int memWrite;
  int memtoReg;
  int regWrite;

} IDEX;

typedef struct EXMEM {
  int aluResultado;
  int registroRd;
  int direccionMemoria;

  // Control
  int branch;
  int memRead;
  int memWrite;
  int memtoReg;
  int regWrite;

} EXMEM;

typedef struct MEMWB {
  int datoRegistro;
  int destino;
  int datoMemoria;

  // Control
  int memtoReg;
  int regWrite;
  
} MEMWB;


IFID* ifId();
IDEX* idEx();
EXMEM* exMem();
MEMWB* memWb();

void printIFID(IFID* buffer);
void printIDEX(IDEX* buffer);
void printEXMEM(EXMEM* buffer);
void printMEMWB(MEMWB* buffer);
