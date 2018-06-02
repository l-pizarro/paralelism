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
  char* instruccion;
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
  int memWrite;

} IDEX;

typedef struct EXMEM {
  int aluResult;
  int registroRd;
  int regDst;

  // Control
  int branch;
  int memRead;
  int memWrite;
  int memtoReg;
  int memWrite;

} EXMEM;

typedef struct MEMWB {
  int memToRegister;
  int registroRd;

  // Control
  int memtoReg;
  int memWrite;
} MEMWB;


IFID* ifId();

IDEX* idEx();

EXMEM* exMem();

MEMWB* memWb();
