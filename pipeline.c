#include "pipeline.h"

Etapa* crearEtapa(int id) {
    Etapa* etapa     = (Etapa*)malloc(sizeof(Etapa));
    etapa->id        =  id;
    etapa->siguiente = NULL;
    return etapa;
}

void eliminarEtapa(Etapa* etapa) {
  free(etapa->siguiente);
  free(etapa);
}

void insertarEtapa(Etapa* pipeline, int id) {
  Etapa* nuevaEtapa = crearEtapa(id);
  Etapa* actual     = pipeline;

  if (pipeline == NULL) {
    pipeline = nuevaEtapa;
    return;
  }

  while (actual->siguiente != NULL) {
    actual = actual->siguiente;
  }

  actual->siguiente = nuevaEtapa;

}

Etapa* removerEtapa(Etapa* pipeline) {
  if (pipeline != NULL) {
    Etapa* nuevoPipeline = pipeline->siguiente;
    eliminarEtapa(pipeline);
    return nuevoPipeline;
  }

  return NULL;
}
