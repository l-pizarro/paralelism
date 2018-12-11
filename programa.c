#include "programa.h"

#define SEPARADOR " ,():\n"

void obtenerTipoInstruccion(char* instruccion, int* tipo) {
  if (instruccion != NULL) {
    if (!strcmp(instruccion, "nop")){
      *tipo = 0;
    }

    else if (!strcmp(instruccion, "add")) {
      *tipo = 1;
    }

    else if (!strcmp(instruccion, "sub")) {
      *tipo = 2;
    }

    else if (!strcmp(instruccion, "mul")) {
      *tipo = 3;
    }

    else if (!strcmp(instruccion, "div")) {
      *tipo = 4;
    }


    else if (!strcmp(instruccion, "addi")) {
      *tipo = 5;
    }

    else if (!strcmp(instruccion, "subi")) {
      *tipo = 6;
    }

    else if (!strcmp(instruccion, "lw")) {
      *tipo = 7;
    }

    else if (!strcmp(instruccion, "sw")) {
      *tipo = 8;
    }

    else if (!strcmp(instruccion, "beq")) {
      *tipo = 9;
    }

    else {
      *tipo = 10;
    }
  }

}

void obtenerNroRegistro(Registro** registros, char* registro, int* nroRegistro) {
  int i;
  for (i = 0; i < 32; i++) {
    if (!strcmp((*registros)[i].nombre, registro)) {
      *nroRegistro = i;
      break;
    }
  }
}

void obtenerEtiqueta(char* etiqueta, char** instrucciones, int nroInstrucciones, int* indice) {
  int i;

  for (i = 0; i < nroInstrucciones; i++) {
    if (strstr(instrucciones[i], ":") != NULL) {
      if (strstr(instrucciones[i], etiqueta) != NULL) {
        *indice = i;
        break;
      }
    }
  }

}

void ejecutar(char** instrucciones, int nroInstrucciones, Registro** registros) {

  IFID*  IFID  = ifId();
  IDEX*  IDEX  = idEx();
  EXMEM* EXMEM = exMem();
  MEMWB* MEMWB = memWb();
  Control* CONTROL = crearControl();

  int indice    = 0;
  int etiquetas = 0;
  int* MEMORIA  = (int*)calloc(128, sizeof(int));

  FILE* csv1 = fopen("S1.csv", "w");
  FILE* csv2 = fopen("S2.csv", "w");


  fprintf(csv1, "Ciclo;IF;ID;EX;MEM;WB\n");
  fprintf(csv2, "Ciclo;Datos;Control\n");

  int ciclo = 1;
  int volver = 0;
  while (indice < nroInstrucciones + 4) {
    fprintf(csv1, "%d", ciclo);
    fprintf(csv2, "%d", ciclo);
    char* aux1  = NULL;
    char* aux2  = NULL;
    char* aux3  = NULL;
    char* aux4  = NULL;
    char* temp  = NULL;

    int rs           = 0;
    int rt           = 0;
    int rd           = 0;
    int offset       = 0;
    int instruccion  = 0;
    int aluResultado = 0;
    int direccionMem = 0;
    int regDestinoEx = 0;
    int datoMemoria  = 0;
    int datoRegistro = 0;
    int destino      = 0;
    int forwardA     = 0;
    int forwardB     = 0;
    int jump         = 0;
    int stall        = 0;
    int beqTaken     = 0;
    int imp          = 0;

    // ETAPA IF
    if (indice < nroInstrucciones) {
      imp = indice;

      if (strstr(instrucciones[indice], "j") != NULL) {
        temp = (char*)calloc(strlen(instrucciones[indice]), sizeof(char));
        strcpy(temp, instrucciones[indice]);

        aux1  = strtok(temp, SEPARADOR);
        aux2  = strtok(NULL, SEPARADOR);
        aux3  = strtok(NULL, SEPARADOR);
        aux4  = strtok(NULL, SEPARADOR);

        obtenerEtiqueta(aux2, instrucciones, nroInstrucciones, &jump);
      }
      else if (strstr(instrucciones[indice], "beq") != NULL) {
        temp = (char*)calloc(strlen(instrucciones[indice]), sizeof(char));
        strcpy(temp, instrucciones[indice]);

        aux1  = strtok(temp, SEPARADOR);
        aux2  = strtok(NULL, SEPARADOR);
        aux3  = strtok(NULL, SEPARADOR);
        aux4  = strtok(NULL, SEPARADOR);

        obtenerEtiqueta(aux4, instrucciones, nroInstrucciones, &jump);
        volver = indice + 1;
      }

      else {
        if (strstr(instrucciones[indice], ":") != NULL) {
          indice++;
          imp++;
          etiquetas++;

        }
        temp = (char*)calloc(strlen(instrucciones[indice]), sizeof(char));
        strcpy(temp, instrucciones[indice]);

        aux1  = strtok(temp, SEPARADOR);
        aux2  = strtok(NULL, SEPARADOR);
        aux3  = strtok(NULL, SEPARADOR);
        aux4  = strtok(NULL, SEPARADOR);
      }
      char* impresion = (char*)calloc(strlen(instrucciones[indice]) - 1, sizeof(char));
      strncpy(impresion, instrucciones[indice], strlen(instrucciones[indice]) - 1);
      fprintf(csv1, ";%s", impresion);
      free(impresion);
    }
    else {
      fprintf(csv1, ";-");
    }

    // ETAPA ID
    if (ciclo > 1 && indice < nroInstrucciones + 1) {
      char* impresion = (char*)calloc(strlen(instrucciones[IFID->imp]) - 1, sizeof(char));
      strncpy(impresion, instrucciones[IFID->imp], strlen(instrucciones[IFID->imp]) - 1);
      fprintf(csv1, ";%s", impresion);
      free(impresion);
      obtenerTipoInstruccion(IFID->extracto1, &instruccion);
      actualizarControl(&CONTROL, instruccion);
      if (instruccion) {
        if (instruccion < 5) {
          obtenerNroRegistro(registros, IFID->extracto2, &rd);
          obtenerNroRegistro(registros, IFID->extracto3, &rs);
          obtenerNroRegistro(registros, IFID->extracto4, &rt);
        }
        else if (instruccion < 7) {
          obtenerNroRegistro(registros, IFID->extracto2, &rt);
          obtenerNroRegistro(registros, IFID->extracto3, &rs);
          offset = atoi(IFID->extracto4);
        }
        else if (instruccion < 9){
          obtenerNroRegistro(registros, IFID->extracto2, &rt);
          obtenerNroRegistro(registros, IFID->extracto4, &rs);
          offset = atoi(IFID->extracto3);
        }
        else if (instruccion == 9) {
          obtenerNroRegistro(registros, IFID->extracto2, &rt);
          obtenerNroRegistro(registros, IFID->extracto3, &rs);
          offset = 1;
        }
      }
    }
    else {
      fprintf(csv1, ";-");
    }

    // ETAPA EX
    if (ciclo > 2 && indice < nroInstrucciones + 2) {
      char* impresion = (char*)calloc(strlen(instrucciones[IDEX->imp]) - 1, sizeof(char));
      strncpy(impresion, instrucciones[IDEX->imp], strlen(instrucciones[IDEX->imp]) - 1);
      fprintf(csv1, ";%s", impresion);
      free(impresion);
      if (IDEX->memRead && ((IDEX->registroRt == rs) || (IDEX->registroRt == rt))) {

        char* instruccionPostergada = (char*)calloc(strlen(instrucciones[indice - 1]), sizeof(char));
        strcpy(instruccionPostergada, instrucciones[indice - 1]);

        aux1  = strtok(instruccionPostergada, SEPARADOR);
        aux2  = strtok(NULL, SEPARADOR);
        aux3  = strtok(NULL, SEPARADOR);
        aux4  = strtok(NULL, SEPARADOR);

        rs          = -1;
        rt          = -1;
        rd          = -1;
        instruccion = -1;

        actualizarControl(&CONTROL, instruccion);
        stall = 1;
      }

      forwardA = 0;
      forwardB = 0;

      int registroConflicto = 0;

      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRs)) {
        forwardA = 1;
        registroConflicto = EXMEM->registroRd;
      }
      if (MEMWB->regWrite && (MEMWB->destino == IDEX->registroRs)) {
        forwardA = 2;
        EXMEM->registroRd = MEMWB->destino;
      }

      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRt)) {
        forwardB = 1;
        registroConflicto = EXMEM->registroRd;
      }
      if (MEMWB->regWrite && (MEMWB->destino == IDEX->registroRt)) {
        forwardB = 2;
        registroConflicto = MEMWB->destino;
      }

      if (forwardA) {
        fprintf(csv2, ";%s", (*registros)[registroConflicto].nombre);
      }
      else {
        fprintf(csv2, ";-");
      }

      switch (IDEX->instruccion) {

        case 1:
        regDestinoEx = IDEX->registroRd;
        if (forwardA == 1 && forwardB == 1) {
          aluResultado = EXMEM->aluResultado + EXMEM->aluResultado;
        }
        else if (forwardA == 1) {
          aluResultado = EXMEM->aluResultado + ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 1) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor + EXMEM->aluResultado;
        }
        else if (forwardA == 2 && forwardB == 2) {
          aluResultado = MEMWB->datoMemoria + MEMWB->datoMemoria;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoMemoria + ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor + MEMWB->datoMemoria;
        }
        else {
          aluResultado = ((*registros)[IDEX->registroRs]).valor + ((*registros)[IDEX->registroRt]).valor;
        }
        break;
        case 2:
        regDestinoEx = IDEX->registroRd;
        if (forwardA == 1 && forwardB == 1) {
          aluResultado = EXMEM->aluResultado - EXMEM->aluResultado;
        }
        else if (forwardA == 1) {
          aluResultado = EXMEM->aluResultado - ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 1) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor - EXMEM->aluResultado;
        }
        else if (forwardA == 2 && forwardB == 2) {
          aluResultado = MEMWB->datoMemoria - MEMWB->datoMemoria;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoMemoria - ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor - MEMWB->datoMemoria;
        }
        else {
          aluResultado = ((*registros)[IDEX->registroRs]).valor - ((*registros)[IDEX->registroRt]).valor;
        }
        break;
        case 3:
        regDestinoEx = IDEX->registroRd;
        if (forwardA == 1 && forwardB == 1) {
          aluResultado = EXMEM->aluResultado * EXMEM->aluResultado;
        }
        else if (forwardA == 1) {
          aluResultado = EXMEM->aluResultado * ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 1) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor * EXMEM->aluResultado;
        }
        else if (forwardA == 2 && forwardB == 2) {
          aluResultado = MEMWB->datoMemoria * MEMWB->datoMemoria;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoMemoria * ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor * MEMWB->datoMemoria;
        }
        else {
          aluResultado = ((*registros)[IDEX->registroRs]).valor * ((*registros)[IDEX->registroRt]).valor;
        }
        break;
        case 4:
        regDestinoEx = IDEX->registroRd;
        if (forwardA == 1 && forwardB == 1) {
          aluResultado = EXMEM->aluResultado / EXMEM->aluResultado;
        }
        else if (forwardA == 1) {
          aluResultado = EXMEM->aluResultado / ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 1) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor / EXMEM->aluResultado;
        }
        else if (forwardA == 2 && forwardB == 2) {
          aluResultado = MEMWB->datoMemoria / MEMWB->datoMemoria;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoMemoria / ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor / MEMWB->datoMemoria;
        }
        else {
          aluResultado = ((*registros)[IDEX->registroRs]).valor / ((*registros)[IDEX->registroRt]).valor;
        }
        break;
        case 5:
        regDestinoEx = IDEX->registroRt;
        if (forwardA == 1) {
          aluResultado =  EXMEM->aluResultado + IDEX->signoExtendido;
        }
        else if (forwardA == 2) {
          aluResultado =  MEMWB->datoMemoria + IDEX->signoExtendido;
        }
        else {
          aluResultado = ((*registros)[IDEX->registroRs]).valor + IDEX->signoExtendido;
        }
        break;
        case 6:
        regDestinoEx = IDEX->registroRt;
        if (forwardA == 1) {
          aluResultado =  EXMEM->aluResultado - IDEX->signoExtendido;
        }
        else if (forwardA == 2) {
          aluResultado =  MEMWB->datoMemoria - IDEX->signoExtendido;
        }
        else {
          aluResultado = ((*registros)[IDEX->registroRs]).valor - IDEX->signoExtendido;
        }
        break;
        case 7:
        regDestinoEx = IDEX->registroRt;
        if (forwardA == 1) {
          direccionMem = (abs(EXMEM->aluResultado) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardA == 2) {
          direccionMem = (abs(MEMWB->datoMemoria) + (IDEX->signoExtendido/4))%128;
        }
        else {
          direccionMem = (abs(((*registros)[IDEX->registroRs]).valor) + (IDEX->signoExtendido/4))%128;
        }
        break;
        case 8:
        if (forwardA == 1 && forwardB == 1) {
          aluResultado = EXMEM->aluResultado;
          direccionMem = (abs(EXMEM->aluResultado) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardA == 1) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor;
          direccionMem = (abs(EXMEM->aluResultado) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardB == 1) {
          aluResultado = EXMEM->aluResultado;
          direccionMem = (abs(((*registros)[IDEX->registroRs]).valor) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardA == 2 && forwardB == 2) {
          aluResultado = MEMWB->datoMemoria;
          direccionMem = (abs(MEMWB->datoMemoria) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardA == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor;
          direccionMem = (abs(MEMWB->datoMemoria) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardB == 2) {
          aluResultado = MEMWB->datoMemoria;
          direccionMem = (abs(((*registros)[IDEX->registroRt]).valor) + (IDEX->signoExtendido/4))%128;
        }
        else {
          aluResultado = ((*registros)[IDEX->registroRs]).valor;
          direccionMem = (abs(((*registros)[IDEX->registroRt]).valor) + (IDEX->signoExtendido/4))%128;
        }
        break;
        case 9:
        if (IDEX->signoExtendido) {
          if (forwardA == 1 && forwardB == 1) {
            if (EXMEM->aluResultado != EXMEM->aluResultado) {
              beqTaken = -1;
            }
          }
          else if (forwardA == 1) {
            if (EXMEM->aluResultado != ((*registros)[IDEX->registroRt]).valor) {
              beqTaken = -1;
            }
          }
          else if (forwardB == 1) {
            if (EXMEM->aluResultado != ((*registros)[IDEX->registroRs]).valor) {
              beqTaken = -1;
            }
          }
          else if (forwardA == 2 && forwardB == 2) {
            if (MEMWB->datoMemoria != MEMWB->datoMemoria) {
              beqTaken = -1;
            }
          }
          else if (forwardA == 2) {
            if (MEMWB->datoMemoria != ((*registros)[IDEX->registroRt]).valor) {
              beqTaken = -1;
            }
          }
          else if (forwardB == 2) {
            if (MEMWB->datoMemoria != ((*registros)[IDEX->registroRs]).valor) {
              beqTaken = -1;
            }
          }
          else {
            if (((*registros)[IDEX->registroRt]).valor != ((*registros)[IDEX->registroRs]).valor) {
              beqTaken = -1;
            }
          }

          break;
        }
        break;
        case 10:
        break;
        default:
        break;
      }
    }
    else {
      fprintf(csv1, ";-");
      fprintf(csv2, ";-");
    }

    // ETAPA MEM
    if (ciclo > 3 && indice < nroInstrucciones + 3) {
      char* impresion = (char*)calloc(strlen(instrucciones[EXMEM->imp]) - 1, sizeof(char));
      strncpy(impresion, instrucciones[EXMEM->imp], strlen(instrucciones[EXMEM->imp]) - 1);
      fprintf(csv1, ";%s", impresion);
      free(impresion);
      if (EXMEM->beqTaken == -1) {
        aux1 = NULL;
        aux2 = NULL;
        aux3 = NULL;
        aux4 = NULL;
        instruccion = 0;
        rs = 0;
        rt = 0;
        rd = 0;
        offset = 0;
        CONTROL->regDst = 0;
        CONTROL->aluOp0 = 0;
        CONTROL->aluOp1 = 0;
        CONTROL->aluSrc = 0;
        CONTROL->branch = 0;
        CONTROL->memRead = 0;
        CONTROL->memWrite = 0;
        CONTROL->memtoReg = 0;
        CONTROL->regWrite = 0;
        aluResultado = 0;
        regDestinoEx = 0;
        direccionMem = 0;
        IDEX->branch = 0;
        IDEX->memRead = 0;
        IDEX->memWrite = 0;
        IDEX->memtoReg = 0;
        IDEX->regWrite = 0;
        beqTaken = 0;
        indice = volver - 1;
        volver = 0;
        // Stall por beq
        fprintf(csv2, ";si");
      }
      else {
        fprintf(csv2, ";-");
      }

      if (EXMEM->memRead == 1 && EXMEM->memWrite == 0) {
        destino      = EXMEM->registroRd;
        datoMemoria = MEMORIA[EXMEM->direccionMemoria];
      }
      else if (EXMEM->memRead == 0 && EXMEM->memWrite == 1) {
        MEMORIA[EXMEM->direccionMemoria] = EXMEM->aluResultado;
      }
      else {
        datoRegistro = EXMEM->aluResultado;
        destino      = EXMEM->registroRd;
      }
    }
    else {
      fprintf(csv1, ";-");
      fprintf(csv2, ";-");
    }

    // ETAPA WB
    if (ciclo > 4 && indice < nroInstrucciones + 4) {
      char* impresion = (char*)calloc(strlen(instrucciones[MEMWB->imp]) - 1, sizeof(char));
      strncpy(impresion, instrucciones[MEMWB->imp], strlen(instrucciones[MEMWB->imp]) - 1);
      fprintf(csv1, ";%s", impresion);
      free(impresion);
      if (MEMWB->memtoReg == 1 && MEMWB->regWrite == 1) {
        ((*registros)[MEMWB->destino]).valor = MEMWB->datoMemoria;
      }
      else if (MEMWB->memtoReg == 0 && MEMWB->regWrite == 1){
        ((*registros)[MEMWB->destino]).valor = MEMWB->datoRegistro;
      }
    }
    else {
      fprintf(csv1, ";-");
    }


    // PUSH INFORMACION
    // MEM

    MEMWB->datoRegistro = datoRegistro;
    MEMWB->destino      = destino;
    MEMWB->datoMemoria  = datoMemoria;
    MEMWB->memtoReg     = EXMEM->memtoReg;
    MEMWB->regWrite     = EXMEM->regWrite;
    MEMWB->imp          = EXMEM->imp;
    // EX
    EXMEM->aluResultado     = aluResultado;
    EXMEM->registroRd       = regDestinoEx;
    EXMEM->direccionMemoria = direccionMem;
    EXMEM->branch           = IDEX->branch;
    EXMEM->memRead          = IDEX->memRead;
    EXMEM->memWrite         = IDEX->memWrite;
    EXMEM->memtoReg         = IDEX->memtoReg;
    EXMEM->regWrite         = IDEX->regWrite;
    EXMEM->beqTaken         = beqTaken;
    EXMEM->imp              = IDEX->imp;
    // ID
    IDEX->instruccion    = instruccion;
    IDEX->registroRs     = rs;
    IDEX->registroRt     = rt;
    IDEX->registroRd     = rd;
    IDEX->signoExtendido = offset;
    IDEX->regDst         = CONTROL->regDst;
    IDEX->aluOp0         = CONTROL->aluOp0;
    IDEX->aluOp1         = CONTROL->aluOp1;
    IDEX->aluSrc         = CONTROL->aluSrc;
    IDEX->branch         = CONTROL->branch;
    IDEX->memRead        = CONTROL->memRead;
    IDEX->memWrite       = CONTROL->memWrite;
    IDEX->memtoReg       = CONTROL->memtoReg;
    IDEX->regWrite       = CONTROL->regWrite;
    IDEX->imp            = IFID->imp;
    // IF
    IFID->extracto1 = aux1;
    IFID->extracto2 = aux2;
    IFID->extracto3 = aux3;
    IFID->extracto4 = aux4;
    IFID->imp = imp;


    if (!stall) {
      if (jump) {
        indice = jump;
      }
      else {
        indice ++;
      }
    }
    else {
      stall = 0;
    }
    ciclo ++;
    fprintf(csv1, "\n");
    fprintf(csv2, "\n");
  }
  fclose(csv1);
  fclose(csv2);

  free(IFID->extracto1);
  free(IFID->extracto2);
  free(IFID->extracto3);
  free(IFID->extracto4);
  free(IFID);
  free(IDEX);
  free(EXMEM);
  free(MEMWB);
  free(CONTROL);
  free(MEMORIA);
}

void iniciar() {
  int i;
  int nroInstrucciones = 0;
  int nroRegistros     = 0;
  int validez          = 0;
  printf("\n=====================================\n");

  char** instrucciones = leerArchivo(&nroInstrucciones, 0, &validez);

  char** infoRegistros = leerArchivo(&nroRegistros, 1, &validez);

  Registro* registros  = crearRegistros(nroRegistros, infoRegistros);

  if (validez == 2) {
    ejecutar(instrucciones, nroInstrucciones, &registros);
    printf("\n * Archivos generados exitosamente *\n");
    printf("\n=====================================\n");
  }
  else {
    printf("\n * Los archivos no se han generado*\n");
    printf("\n=====================================\n");
  }

  for (i = 0; i < 32; i++) {
    free(registros[i].nombre);
  }
  free(registros);

  for (i = 0; i < nroInstrucciones; i++) {
    free(instrucciones[i]);
  }
  free(instrucciones);

  for (i = 0; i < nroInstrucciones; i++) {
    free(infoRegistros[i]);
  }
  free(infoRegistros);
}
