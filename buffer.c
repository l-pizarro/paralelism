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
