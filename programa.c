#include "programa.h"
#include "string.h"

void print(int largo, char** arreglo) {
  int i;
  for (i = 0; i < largo; i++) {
    printf("%s\n", arreglo[i]);
  }
}

void obtenerTipoInstruccion(char* instruccion, int* tipo) {
  if (!strcmp(instruccion, "add") || !strcmp(instruccion, "sub")) {

  }
}

void ejecutar(char** instrucciones, int nroInstrucciones, Registro** registros) {

  Control* control = control();

  IFID* IFID   = ifId();
  IDEX* IDEX   = idEx();
  EXMEM* EXMEM = exMem();
  MEMWB* MEMWB = memWb();

  int indice = 0;

  while (indice < nroInstrucciones) {
    char* aux1 = strtok(instrucciones[indice], " )(,:\n");
    char* aux2 = strtok(NULL, " )(,:\n");
    char* aux3 = NULL;
    char* aux4 = NULL;

    if (aux2 != NULL) {
      aux3 = strtok(NULL, " )(,:\n");
      if (aux3 != NULL) {
        aux4 = strtok(NULL, " )(,:\n");
      }
    }
    indice++;
  }
}

void iniciar() {
  int nroInstrucciones = 0;
  int nroRegistros     = 0;

  //printf("Ingrese archivo con instrucciones\n");
  char** instrucciones = leerArchivo(&nroInstrucciones);
  //printf("Ingrese archivo con registros\n");
  char** infoRegistros = leerArchivo(&nroRegistros);

  Registro* registros  = crearRegistros(nroRegistros, infoRegistros);

  ejecutar(instrucciones, nroInstrucciones, &registros);

}
