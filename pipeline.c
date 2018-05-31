#include "pipeline.h"
#include "stdio.h"

Etapa* crearEtapa(int id) {
    Etapa* etapa     = (Etapa*)malloc(sizeof(Etapa));
    etapa->id        =  id;
    etapa->siguiente = NULL;
    etapa->extracto1 = NULL;
    etapa->extracto2 = NULL;
    etapa->extracto3 = NULL;
    etapa->extracto4 = NULL;

    return etapa;
}

void liberarEtapa(Etapa* etapa) {
  free(etapa->siguiente);
  free(etapa);
}

void insertarEtapa(Etapa** pipeline, Etapa* nuevaEtapa) {
  Etapa* actual     = *pipeline;

  if (pipeline == NULL) {
    *pipeline = nuevaEtapa;
    return;
  }

  while (actual->siguiente != NULL) {
    actual = actual->siguiente;
  }

  actual->siguiente = nuevaEtapa;

}

void removerEtapa(Etapa** pipeline) {
  if (pipeline != NULL) {
    Etapa* nuevoPipeline = (*pipeline)->siguiente;
    *(pipeline) = nuevoPipeline;
  }
}

Etapa* crearPipeline(int nroEtapas) {
  int i;
  Etapa* pipeline = crearEtapa(0);
  for (i = 1; i < nroEtapas; i++) {
    Etapa* nuevaEtapa = crearEtapa(0);
    insertarEtapa(&pipeline, nuevaEtapa);
  }

  return pipeline;
}

int largo(Etapa* pipeline) {
  int largo     = 0;
  Etapa* actual = pipeline;

  while (actual != NULL) {
    actual = actual->siguiente;
    largo ++;
  }

  return largo;
}

void imprimirPipeline(Etapa* pipeline) {
  Etapa* actual = pipeline;

  while (actual != NULL) {
    printf("%s %s %s %s\n", actual->extracto1
                          , actual->extracto2
                          , actual->extracto3
                          , actual->extracto4 );

  actual = actual->siguiente;
  }
}
