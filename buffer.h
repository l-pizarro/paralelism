#pragma once
#include "stdlib.h"

// Estructuras IFID IDEX EXMEM MEMWB
//  Representación de los registro (Hardware) a través de
//  valores enteros y strings para el almacenamiento de la
//  información durante el transcurso de la ejecucion del
//  pipeline de 5 etapas

typedef struct IFID {
  int   instruccionActual;
  int   instruccionSgte;
  char* extracto1;
  char* extracto2;
  char* extracto3;
  char* extracto4;
  int imp;

} IFID;

typedef struct IDEX {
  int instruccion;
  int registroRs;
  int registroRt;
  int registroRd;
  int signoExtendido;
  int imp;


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
  int beqTaken;
  int imp;


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
  int beqTaken;
  int imp;

  // Control
  int memtoReg;
  int regWrite;

} MEMWB;


IFID* ifId();
IDEX* idEx();
EXMEM* exMem();
MEMWB* memWb();
