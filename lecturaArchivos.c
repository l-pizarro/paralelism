#include "lecturaArchivos.h"

char** leerArchivo(int* cantidadLineas, int tipo, int* validez) {
  FILE*  archivo;
  char   linea[50];
  char   ruta[50];
  char** lineas;
  if (tipo == 0) {
    printf("\nIngrese archivo con programa en MIPS:\n   > ");
  }
  else {
    printf("\nIngrese archivo con registros:\n   > ");
  }
  scanf("%s", ruta);

  archivo       = fopen(ruta, "r");
  lineas = NULL;

  if (archivo){
    *cantidadLineas = 0;
    lineas = (char**)calloc(1, sizeof(char*));
    while (fgets(linea, 50, archivo)) {
      lineas                  = (char**)realloc(lineas, (*cantidadLineas + 1) * sizeof(char*));
      lineas[*cantidadLineas] = (char*)malloc(sizeof(char) * strlen(linea));
      strcpy(lineas[*cantidadLineas], linea);
      (*cantidadLineas) ++;
    }
    fclose(archivo);
    (*validez) ++;
  }
  else {
    printf("  > El archivo '%s' no se encuentra!!\n", ruta);
  }
  return lineas;
}
