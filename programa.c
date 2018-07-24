#include "programa.h"

#define SEPARADOR " ,():\n"

void print(int largo, char** arreglo) {
  int i;
  for (i = 0; i < largo; i++) {
    printf("%s\n", arreglo[i]);
  }
}

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

void obtenerEtiquetaJump(char* etiqueta, char** instrucciones, int nroInstrucciones, int* indice) {
  int i;

  for (i = 0; i < nroInstrucciones; i++) {
    if (strstr(instrucciones[i], ":") != NULL) {
      if (strstr(instrucciones[i], etiqueta) != NULL) {
        *indice = i;
        printf("LABEL: %s\n", instrucciones[i]);
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

  MEMORIA[4]    = 7;

  int ciclo = 1;

  while (indice < nroInstrucciones + 4) {
    printf("CICLO %d\n", ciclo);

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

    // ETAPA IF
    if (indice < nroInstrucciones) {
      if (strstr(instrucciones[indice], "j") != NULL) {
        temp = (char*)calloc(strlen(instrucciones[indice]), sizeof(char));
        strcpy(temp, instrucciones[indice]);

        aux1  = strtok(temp, SEPARADOR);
        aux2  = strtok(NULL, SEPARADOR);
        aux3  = strtok(NULL, SEPARADOR);
        aux4  = strtok(NULL, SEPARADOR);

        obtenerEtiquetaJump(aux2, instrucciones, nroInstrucciones, &jump);
      }
      else {
        if (strstr(instrucciones[indice], ":") != NULL) {
          indice++;
          etiquetas++;

        }
        else {
          temp = (char*)calloc(strlen(instrucciones[indice]), sizeof(char));
          strcpy(temp, instrucciones[indice]);

          aux1  = strtok(temp, SEPARADOR);
          aux2  = strtok(NULL, SEPARADOR);
          aux3  = strtok(NULL, SEPARADOR);
          aux4  = strtok(NULL, SEPARADOR);
        }
      }
      printf("IF\n");
    }

    // ETAPA ID
    if (ciclo > 1 && indice < nroInstrucciones + 1) {

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

        }

        else {

        }
      }
      printf("ID\n");
    }

    // ETAPA EX
    if (ciclo > 2 && indice < nroInstrucciones + 2) {
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

      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRs)) {
        printf("FW 1\n");
        forwardA = 1;
      }
      if (MEMWB->regWrite && (MEMWB->destino == IDEX->registroRs)) {
        printf("FW 3\n");
        forwardA = 2;
      }

      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRt)) {
        printf("FW 2\n");
        forwardB = 1;
      }
      if (MEMWB->regWrite && (MEMWB->destino == IDEX->registroRt)) {
        printf("FW 4\n");
        forwardB = 2;
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
            printf("\n\n ALU: %d\n", aluResultado);
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
        printf("El load word se guarda en %d\n", regDestinoEx);
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
        break;
        case 10:
        break;
        default:
        break;
      }

      printf("EX\n");
    }

    // ETAPA MEM
    if (ciclo > 3 && indice < nroInstrucciones + 3) {
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
      printf("MEM\n");
    }

    // ETAPA WB
    if (ciclo > 4 && indice < nroInstrucciones + 4) {
      if (MEMWB->memtoReg == 1 && MEMWB->regWrite == 1) {
        ((*registros)[MEMWB->destino]).valor = MEMWB->datoMemoria;
        printf("%d\n", MEMWB->datoMemoria);
      }
      else if (MEMWB->memtoReg == 0 && MEMWB->regWrite == 1){
        ((*registros)[MEMWB->destino]).valor = MEMWB->datoRegistro;
        printf("%d\n", MEMWB->datoRegistro);
      }
      printf("WB\n");
    }

    // PUSH INFORMACION
    // MEM

    MEMWB->datoRegistro = datoRegistro;
    MEMWB->destino      = destino;
    MEMWB->datoMemoria  = datoMemoria;
    MEMWB->memtoReg     = EXMEM->memtoReg;
    MEMWB->regWrite     = EXMEM->regWrite;
    // EX
    EXMEM->aluResultado     = aluResultado;
    EXMEM->registroRd       = regDestinoEx;
    EXMEM->direccionMemoria = direccionMem;
    EXMEM->branch           = IDEX->branch;
    EXMEM->memRead          = IDEX->memRead;
    EXMEM->memWrite         = IDEX->memWrite;
    EXMEM->memtoReg         = IDEX->memtoReg;
    EXMEM->regWrite         = IDEX->regWrite;
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
    // IF
    IFID->extracto1 = aux1;
    IFID->extracto2 = aux2;
    IFID->extracto3 = aux3;
    IFID->extracto4 = aux4;


    printIFID(IFID);
    printIDEX(IDEX);
    printEXMEM(EXMEM);
    printMEMWB(MEMWB);
    printf("\n\n");

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
  }

  for (int a = 0; a < 32; a++) {
    printf("%5s: %3d\n", ((*registros)[a]).nombre, ((*registros)[a]).valor);
  }

  printf("\n  > %d\n", MEMORIA[99]);

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
