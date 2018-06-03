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
  memoria[4] = 77;

  int indice = 0;
  int tipoInstruccion = 0;
  int IF  = 1;
  int ID  = 0;
  int EX  = 0;
  int MEM = 0;
  int WB  = 0;


  while (indice < nroInstrucciones + 4) {
    printf("===============================================\n");

    char* aux1 = NULL;
    char* aux2 = NULL;
    char* aux3 = NULL;
    char* aux4 = NULL;

    // WB
    if (WB && indice < nroInstrucciones + 4) {
      // printf("WB:\n");
      if (MEMWB->memtoReg == 1 && MEMWB->regWrite == 1) {
        ((*registros)[MEMWB->registroDestino]).valor = MEMWB->dataMemoria;
      }
      else if (MEMWB->memtoReg == 0 && MEMWB->regWrite == 1){
        ((*registros)[MEMWB->registroDestino]).valor = MEMWB->aluResultado;
      }

      if (MEM == 0) {
        WB = 0;
      }
    }

    // MEM
    if (MEM && indice < nroInstrucciones + 3) {
      // printf("MEM:\n");
      // printf("  > memRead = %d\n  > memWrite = %d\n", EXMEM->memRead, EXMEM->memWrite);
      if (EXMEM->memRead == 1 && EXMEM->memWrite == 0) {
        MEMWB->dataMemoria      = memoria[EXMEM->direccionMemoria];
        // printf("  > memoria: %d\n", memoria[EXMEM->direccionMemoria]);
        MEMWB->registroDestino  = EXMEM->registroRt;
      }
      else if (EXMEM->memRead == 0 && EXMEM->memWrite == 1) {

      }
      else {
        MEMWB->aluResultado     = EXMEM->aluResultado;
        MEMWB->registroDestino  = EXMEM->registroRs;
      }

      MEMWB->memtoReg = EXMEM->memtoReg;
      MEMWB->regWrite = EXMEM->regWrite;
      printMEMWB(MEMWB);

      if (EX == 0) {
        MEM = 0;
      }
    }

    // EX
    if (EX && indice < nroInstrucciones + 2) {
      // printf("EX:\n");
      switch (IDEX->instruccion) {
        case 1:
        // printf("  > add\n");
        EXMEM->aluResultado = IDEX->registroRs + IDEX->registroRt;
        EXMEM->registroRd   = IDEX->registroRd;
        break;
        case 2:
        // printf("  > sub\n");
        EXMEM->aluResultado = IDEX->registroRs - IDEX->registroRt;
        EXMEM->registroRd   = IDEX->registroRd;
        break;
        case 3:
        // printf("  > mul\n");
        EXMEM->aluResultado = IDEX->registroRs * IDEX->registroRt;
        EXMEM->registroRd   = IDEX->registroRd;
        break;
        case 4:
        // printf("  > div\n");
        EXMEM->aluResultado = IDEX->registroRs / IDEX->registroRt;
        EXMEM->registroRd   = IDEX->registroRd;
        break;
        case 5:
        // printf("  > addi\n");
        EXMEM->aluResultado = IDEX->registroRt + IDEX->signoExtendido;
        EXMEM->registroRs   = IDEX->registroRs;
        break;
        case 6:
        // printf("  > subi\n");
        EXMEM->aluResultado = IDEX->registroRt - IDEX->signoExtendido;
        EXMEM->registroRs   = IDEX->registroRs;
        break;
        case 7:
        // printf("  > lw\n");
        EXMEM->registroRt = IDEX->registroRt;
        EXMEM->direccionMemoria = (abs(IDEX->registroRs) + (IDEX->signoExtendido/4))%128;
        break;
        case 8:
        // printf("  > sw\n");
        memoria[(abs(IDEX->registroRs) + (IDEX->signoExtendido/4))%128] = IDEX->registroRt;
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

      printEXMEM(EXMEM);

      if (ID == 0) {
        EX = 0;
      }
    }

    // ID
    if (ID && indice < nroInstrucciones + 1) {
      // printf("ID:\n");
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
        IDEX->registroRs = ((*registros)[j]).valor;
        IDEX->registroRt = ((*registros)[k]).valor;
        // printf("  > r1:%d r2:%d r3:%d\n", IDEX->registroRs, IDEX->registroRt, IDEX->registroRd);
        break;

        // sub
        case 2:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        obtenerNroRegistro(registros, IFID->extracto4, &k);
        IDEX->registroRd = i;
        IDEX->registroRs = ((*registros)[j]).valor;
        IDEX->registroRt = ((*registros)[k]).valor;
        // printf("  > r1:%d r2:%d r3:%d\n", IDEX->registroRs, IDEX->registroRt, IDEX->registroRd);
        break;

        // mul
        case 3:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        obtenerNroRegistro(registros, IFID->extracto4, &k);
        IDEX->registroRs = ((*registros)[j]).valor;
        IDEX->registroRt = ((*registros)[k]).valor;
        IDEX->registroRd = i;
        // printf("  > r1:%d r2:%d r3:%d\n", IDEX->registroRs, IDEX->registroRt, IDEX->registroRd);
        break;

        // div
        case 4:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        obtenerNroRegistro(registros, IFID->extracto4, &k);
        IDEX->registroRd = i;
        IDEX->registroRs = ((*registros)[j]).valor;
        IDEX->registroRt = ((*registros)[k]).valor;
        // printf("  > r1:%d r2:%d r3:%d\n", IDEX->registroRs, IDEX->registroRt, IDEX->registroRd);
        break;

        // addi
        case 5:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        IDEX->registroRs     = i;
        IDEX->registroRt     = ((*registros)[j]).valor;
        IDEX->signoExtendido = atoi(IFID->extracto4);
        // printf("  > r1:%d r2:%d off:%d\n", IDEX->registroRs, IDEX->registroRt, IDEX->signoExtendido);
        break;

        // subi
        case 6:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto3, &j);
        IDEX->registroRs     = i;
        IDEX->registroRt     = ((*registros)[j]).valor;
        IDEX->signoExtendido = atoi(IFID->extracto4);
        // printf("  > r1:%d r2:%d off:%d\n", IDEX->registroRs, IDEX->registroRt, IDEX->signoExtendido);
        break;

        // lw
        case 7:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto4, &j);
        IDEX->registroRt     = i;
        IDEX->signoExtendido = atoi(IFID->extracto3);
        IDEX->registroRs     = ((*registros)[j]).valor;
        // printf("  > r1:%d off:%d r2:%d\n", IDEX->registroRt, IDEX->signoExtendido, IDEX->registroRs);
        break;

        case 8:
        obtenerNroRegistro(registros, IFID->extracto2, &i);
        obtenerNroRegistro(registros, IFID->extracto4, &j);
        IDEX->registroRt     = ((*registros)[i]).valor;
        IDEX->signoExtendido = atoi(IFID->extracto3);
        IDEX->registroRs     = ((*registros)[j]).valor;
        // printf("  > r1:%d off:%d r2:%d\n", IDEX->registroRt, IDEX->signoExtendido, IDEX->registroRs);
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

      printIDEX(IDEX);

      if (IF == 0) {
        ID = 0;
      }

    }

    // IF
    if (IF && indice < nroInstrucciones) {
      // printf("IF:\n");
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

      printIFID(IFID);
    }

    // CASO BORDE
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

    indice++;
  }

  printf("\n");
  for (int n = 0; n < 32; n++) {
    printf("%s: %d\n", ((*registros)[n]).nombre, ((*registros)[n]).valor);
  }
  printf("\n");

  for(int m = 0; m < 128; m+=8) {
    printf("%d %d %d %d %d %d %d %d\n",memoria[m],memoria[m+1],memoria[m+2],memoria[m+3],memoria[m+4],memoria[m+5],memoria[m+6],memoria[m+7]);
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
