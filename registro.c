#include "registro.h"

Registro* crearRegistros(int nroRegistros, char** infoRegistros) {

  char* token = NULL;

  Registro* registros = (Registro*)calloc(32, sizeof(Registro));

  for (int i = 0; i < nroRegistros; i++) {
      token = strtok(infoRegistros[i], " ");
      registros[i].nombre  = (char*)calloc(strlen(token), sizeof(char));
      strcpy(registros[i].nombre, token);
      token = strtok(NULL, " )(,");
      registros[i].valor   = atoi(token);
  }

  return registros;
}

void liberarRegistros(Registro* r) {
  int i;

  for (i = 0; i < 32; i++) {
    free(r[i].nombre);
    free(r[i].memoria);
  }
  free(r);
}
