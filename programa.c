#include "programa.h"
#include "string.h"

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

  IFID* IFID   = ifId();
  IDEX* IDEX   = idEx();
  EXMEM* EXMEM = exMem();
  MEMWB* MEMWB = memWb();

  Control* control = createControl();

  int* memoria = (int*)calloc(128,sizeof(int));
  memoria[100] = 33;

  int IF  = 1;
  int ID  = 0;
  int EX  = 0;
  int MEM = 0;
  int WB  = 0;
  int indice = 0;
  int tipoInstruccion = 0;


  while (indice < nroInstrucciones + 4) {

    char* aux1 = NULL;
    char* aux2 = NULL;
    char* aux3 = NULL;
    char* aux4 = NULL;

    //printf("=====%2d =====================================================================\n\n", indice);

    // printf("Antes\n");
    // printIFID(IFID);
    // printIDEX(IDEX);
    // printEXMEM(EXMEM);
    // printMEMWB(MEMWB);

    // WB
    if (WB && indice < nroInstrucciones + 4) {

      if (MEMWB->memtoReg == 1 && MEMWB->regWrite == 1) {
        ((*registros)[MEMWB->registroRd]).valor = MEMWB->dataMemoria;
      }
      else if (MEMWB->memtoReg == 0 && MEMWB->regWrite == 1){
        ((*registros)[MEMWB->registroRd]).valor = MEMWB->aluResultado;
      }

    }

    // MEM
    if (MEM && indice < nroInstrucciones + 3) {
      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRs)) {
        printf("  > [MEM - %d] ForwardA = 01 !!\n\n", indice);
       }
      if (MEMWB->regWrite && (MEMWB->registroRd == IDEX->registroRt)) {
        printf("  > [MEM - %d] ForwardB = 01 !!\n\n", indice);
      }

      if (EXMEM->memRead == 1 && EXMEM->memWrite == 0) {
        MEMWB->dataMemoria      = memoria[EXMEM->direccionMemoria];
        MEMWB->registroRd  = EXMEM->registroRd;
      }
      else if (EXMEM->memRead == 0 && EXMEM->memWrite == 1) {

      }
      else {
        MEMWB->aluResultado     = EXMEM->aluResultado;
        MEMWB->registroRd  = EXMEM->registroRd;
      }

      MEMWB->memtoReg = EXMEM->memtoReg;
      MEMWB->regWrite = EXMEM->regWrite;
    }

    // EX
    if (EX && indice < nroInstrucciones + 2) {
      //printEXMEM(EXMEM);

      // printf("  Forwarding EX Ciclo %d\n", indice);
      // printf("      EX/MEM.regWrite:%5d\n", EXMEM->regWrite);
      // printf("      EX/MEM.registerRd:%3d\n", EXMEM->registroRd);
      // printf("      ID/EX.registerRt:%4d\n", IDEX->registroRt);
      // printf("      ID/EX.registerRs:%4d\n", IDEX->registroRs);

      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRs)) {
        printf("  > [EX  - %d] ForwardA = 10 !!\n\n", indice);
      }

      if (EXMEM->regWrite && (EXMEM->registroRd == IDEX->registroRt)) {
        printf("  > [EX  - %d] ForwardB = 10 !!\n\n", indice);
      }

      switch (IDEX->instruccion) {
        case 1:
        EXMEM->registroRd   = IDEX->registroRd;
        EXMEM->aluResultado = ((*registros)[IDEX->registroRs]).valor + ((*registros)[IDEX->registroRt]).valor;
        break;

        case 2:
        EXMEM->registroRd   = IDEX->registroRd;
        EXMEM->aluResultado = ((*registros)[IDEX->registroRs]).valor - ((*registros)[IDEX->registroRt]).valor;
        break;

        case 3:
        EXMEM->registroRd   = IDEX->registroRd;
        EXMEM->aluResultado = ((*registros)[IDEX->registroRs]).valor * ((*registros)[IDEX->registroRt]).valor;
        break;

        case 4:
        EXMEM->registroRd   = IDEX->registroRd;
        EXMEM->aluResultado = ((*registros)[IDEX->registroRs]).valor / ((*registros)[IDEX->registroRt]).valor;
        break;

        case 5:
        EXMEM->registroRd   = IDEX->registroRs;
        EXMEM->aluResultado = ((*registros)[IDEX->registroRt]).valor + IDEX->signoExtendido;
        break;

        case 6:
        EXMEM->registroRd   = IDEX->registroRs;
        EXMEM->aluResultado = ((*registros)[IDEX->registroRt]).valor - IDEX->signoExtendido;
        break;

        case 7:
        EXMEM->registroRd = IDEX->registroRs;
        EXMEM->direccionMemoria = (abs(((*registros)[IDEX->registroRt]).valor) + (IDEX->signoExtendido/4))%128;
        break;

        case 8:
        memoria[(abs(((*registros)[IDEX->registroRs]).valor) + (IDEX->signoExtendido/4))%128] = ((*registros)[IDEX->registroRt]).valor;
        break;

        case 9:
        break;
        case 10:
        break;
        default:
        break;
      }

      EXMEM->branch   = IDEX->branch;
      EXMEM->memRead  = IDEX->memRead;
      EXMEM->memWrite = IDEX->memWrite;
      EXMEM->memtoReg = IDEX->memtoReg;
      EXMEM->regWrite = IDEX->regWrite;

      //printEXMEM(EXMEM);
      //printf("\n\n");

    }

    // ID
    if (ID && indice < nroInstrucciones + 1) {

      obtenerTipoInstruccion(IFID->extracto1, &tipoInstruccion);
      actualizarControl(&control, tipoInstruccion);
      int i, j, k;
      switch (tipoInstruccion) {

        // add
        case 1:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        obtenerNroRegistro(registros, IFID->extracto4, &k);
        IDEX->registroRd = i;
        IDEX->registroRs = j;
        IDEX->registroRt = k;
        break;

        // sub
        case 2:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        obtenerNroRegistro(registros, IFID->extracto4, &k);
        IDEX->registroRd = i;
        IDEX->registroRs = j;
        IDEX->registroRt = k;
        break;

        // mul
        case 3:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        obtenerNroRegistro(registros, IFID->extracto4, &k);
        IDEX->registroRd = i;
        IDEX->registroRs = j;
        IDEX->registroRt = k;
        break;

        // div
        case 4:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        obtenerNroRegistro(registros, IFID->extracto4, &k);
        IDEX->registroRd = i;
        IDEX->registroRs = j;
        IDEX->registroRt = k;
        break;

        // addi
        case 5:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        IDEX->registroRs     = i;
        IDEX->registroRt     = j;
        IDEX->signoExtendido = atoi(IFID->extracto4);
        break;

        // subi
        case 6:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        IDEX->registroRs     = i;
        IDEX->registroRt     = j;
        IDEX->signoExtendido = atoi(IFID->extracto4);
        break;

        // lw
        case 7:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto4, &j);
        IDEX->registroRt     = i;
        IDEX->signoExtendido = atoi(IFID->extracto3);
        IDEX->registroRs     = j;
        break;

        case 8:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto4, &j);
        IDEX->registroRt     = i;
        IDEX->signoExtendido = atoi(IFID->extracto3);
        IDEX->registroRs     = j;
        break;

        default:
        break;
      }

      IDEX->regDst      = control->regDst;
      IDEX->aluOp0      = control->aluOp0;
      IDEX->aluOp1      = control->aluOp1;
      IDEX->aluSrc      = control->aluSrc;
      IDEX->branch      = control->branch;
      IDEX->memRead     = control->memRead;
      IDEX->memWrite    = control->memWrite;
      IDEX->memtoReg    = control->memtoReg;
      IDEX->regWrite    = control->regWrite;
      IDEX->instruccion = tipoInstruccion;

    }

    // IF
    if (IF && indice < nroInstrucciones) {

      aux1 = strtok(instrucciones[indice], " )(,:\n");
      aux2 = strtok(NULL, " )(,:\n");

      if (aux2 != NULL) {
        aux3 = strtok(NULL, " )(,:\n");
        if (aux3 != NULL) {
          aux4 = strtok(NULL, " )(,:\n");
        }
      }

      IFID->instruccionSgte   = indice + 1;
      IFID->instruccionActual = indice;
      IFID->extracto1         = aux1;
      IFID->extracto2         = aux2;
      IFID->extracto3         = aux3;
      IFID->extracto4         = aux4;

    }


    if (IF) {
      if (ID) {
        if (EX) {
          if (MEM) {
            WB = 1;
          }
          else {
            MEM = 1;
          }
        }
        else {
          EX = 1;
        }
      }
      else {
        ID = 1;
      }
    }

    // printf("Despues\n");
    // printIFID(IFID);
    // printIDEX(IDEX);
    // printEXMEM(EXMEM);
    // printMEMWB(MEMWB);
    //printf("==============================================================================\n\n");
    indice++;
  }

  printf("\n");
  for (int n = 0; n < 32; n++) {
    printf("%5s: %3d\n", ((*registros)[n]).nombre, ((*registros)[n]).valor);
  }
  // printf("\n");
  //
  // for(int m = 0; m < 128; m+=8) {
  //   printf("%d %d %d %d %d %d %d %d\n",memoria[m],memoria[m+1],memoria[m+2],memoria[m+3],memoria[m+4],memoria[m+5],memoria[m+6],memoria[m+7]);
  // }
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
