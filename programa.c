#include "programa.h"

void print(int largo, char** arreglo) {
  int i;
  for (i = 0; i < largo; i++) {
    printf("%s\n", arreglo[i]);
  }
}

void ejecutar(char** instrucciones, int nroInstrucciones, Registro** registros, Etapa* pipeline) {
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

    Etapa* nuevaEtapa = crearEtapa(0);
    nuevaEtapa->extracto1 =  aux1;
    nuevaEtapa->extracto2 =  aux2;
    nuevaEtapa->extracto3 =  aux3;
    nuevaEtapa->extracto4 =  aux4;

    insertarEtapa(&pipeline, nuevaEtapa);

    // Aqui se supone que pasa lo lindo
    imprimirPipeline(pipeline);
    printf("\n");

    removerEtapa(&pipeline);
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
  Etapa* pipeline      = crearPipeline(5);

  ejecutar(instrucciones, nroInstrucciones, &registros, pipeline);

}
