#include "programa.h"

#define SEPARADOR " ,():\n"

void print(int largo, char** arreglo) {
  int i;
  for (i = 0; i < largo; i++) {
    printf("%s\n", arreglo[i]);
  }
}

void obtenerTipoInstruccion(char* instruccion, int* tipo) {
  if (!strcmp(instruccion, "add")) {
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

  else if (!strcmp(instruccion, "j")){
    *tipo = 10;
  }
  else {
    *tipo = 0;
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

void ejecutar(char** instrucciones, int nroInstrucciones, Registro** registros) {

  IFID*  IFID  = ifId();
  IDEX*  IDEX  = idEx();
  EXMEM* EXMEM = exMem();
  MEMWB* MEMWB = memWb();
  Control* CONTROL = crearControl();

  int indice  = 0;

  int* MEMORIA = (int*)calloc(128, sizeof(int));
  MEMORIA[4] = 7;

  while (indice < nroInstrucciones + 4) {

    char* aux1  = NULL;
    char* aux2  = NULL;
    char* aux3  = NULL;
    char* aux4  = NULL;

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

    // ETAPA IF
    if (indice < nroInstrucciones) {
      aux1  = strtok(instrucciones[indice], SEPARADOR);
      aux2  = strtok(NULL, SEPARADOR);
      aux3  = strtok(NULL, SEPARADOR);
      aux4  = strtok(NULL, SEPARADOR);
    }

    // ETAPA ID
    if (indice > 0 && indice < nroInstrucciones + 1) {
      obtenerTipoInstruccion(IFID->extracto1, &instruccion);
      actualizarControl(&CONTROL, instruccion);

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
      else {

      }
    }

    // ETAPA EX
    if (indice > 1 && indice < nroInstrucciones + 2) {
      forwardA = 0;
      forwardB = 0;

      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRs)) {
        forwardA = 1;
      }
      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRt)) {
        forwardB = 1;
      }
      if (MEMWB->regWrite && (MEMWB->destino == IDEX->registroRs)) {
        forwardA = 2;
      }
      if (MEMWB->regWrite && (MEMWB->destino == IDEX->registroRt)) {
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
          aluResultado = MEMWB->datoRegistro + MEMWB->datoRegistro;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoRegistro + ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor + MEMWB->datoRegistro;
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
          aluResultado = MEMWB->datoRegistro - MEMWB->datoRegistro;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoRegistro - ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor - MEMWB->datoRegistro;
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
          aluResultado = MEMWB->datoRegistro * MEMWB->datoRegistro;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoRegistro * ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor * MEMWB->datoRegistro;
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
          aluResultado = MEMWB->datoRegistro / MEMWB->datoRegistro;
        }
        else if (forwardA == 2) {
          aluResultado = MEMWB->datoRegistro / ((*registros)[IDEX->registroRt]).valor;
        }
        else if (forwardB == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor / MEMWB->datoRegistro;
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
          aluResultado =  MEMWB->datoRegistro + IDEX->signoExtendido;
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
          aluResultado =  MEMWB->datoRegistro - IDEX->signoExtendido;
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
          direccionMem = (abs(MEMWB->datoRegistro) + (IDEX->signoExtendido/4))%128;
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
          aluResultado = MEMWB->datoRegistro;
          direccionMem = (abs(MEMWB->datoRegistro) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardA == 2) {
          aluResultado = ((*registros)[IDEX->registroRs]).valor;
          direccionMem = (abs(MEMWB->datoRegistro) + (IDEX->signoExtendido/4))%128;
        }
        else if (forwardB == 2) {
          aluResultado = MEMWB->datoRegistro;
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
    }

    // ETAPA MEM
    if (indice > 2 && indice < nroInstrucciones + 3) {
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

    // ETAPA WB
    if (indice > 3 && indice < nroInstrucciones + 4) {
      if (MEMWB->memtoReg == 1 && MEMWB->regWrite == 1) {
        ((*registros)[MEMWB->destino]).valor = MEMWB->datoMemoria;
      }
      else if (MEMWB->memtoReg == 0 && MEMWB->regWrite == 1){
        ((*registros)[MEMWB->destino]).valor = MEMWB->datoRegistro;
      }
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

    indice ++;
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
