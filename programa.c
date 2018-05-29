#include "programa.h"

void print(int largo, char** arreglo) {
  int i;
  for (i = 0; i < largo; i++) {
    printf("%s\n", arreglo[i]);
  }
}

void iniciar() {
  // int nroInstrucciones = 0;
  // int nroRegistros     = 0;
  //
  // //printf("Ingrese archivo con instrucciones\n");
  // char** instrucciones = leerArchivo(&nroInstrucciones);
  // //printf("Ingrese archivo con registros\n");
  // char** infoRegistros = leerArchivo(&nroRegistros);
  //
  // Registro* registros = crearRegistros(nroRegistros, infoRegistros);
  //
  //

}
