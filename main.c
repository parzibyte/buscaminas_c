#include <assert.h>
#include <ctype.h>  // toupper
#include <stdio.h>
#include <stdlib.h>  // rand
#include <unistd.h>  // getpid
#define COLUMNAS 4
#define FILAS 4
#define ESPACIO_SIN_DESCUBRIR '?'
#define ESPACIO_DESCUBIERTO ' '
#define MINA 'M'
#define CANTIDAD_MINAS 5
#define ERROR_MINA_ENCONTRADA 1
#define ERROR_ESPACIO_YA_DESCUBIERTO 2
#define ERROR_NINGUNO 3
#define DEBUG 1

int obtenerMinasCercanas(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
  int conteo = 0, filaInicio, filaFin, columnaInicio, columnaFin;
  if (fila <= 0) {
    filaInicio = 0;
  } else {
    filaInicio = fila - 1;
  }

  if (fila + 1 >= FILAS) {
    filaFin = FILAS - 1;
  } else {
    filaFin = fila + 1;
  }

  if (columna <= 0) {
    columnaInicio = 0;
  } else {
    columnaInicio = columna - 1;
  }
  if (columna + 1 >= COLUMNAS) {
    columnaFin = COLUMNAS - 1;
  } else {
    columnaFin = columna + 1;
  }
  // printf("FI: %d FF: %d CI: %d CF: %d", filaInicio, filaFin, columnaInicio,
  //        columnaFin);

  int m;
  for (m = filaInicio; m <= filaFin; m++) {
    int l;
    for (l = columnaInicio; l <= columnaFin; l++) {
      // printf("tablero[m][l]=%c\n", tablero[m][l]);
      if (tablero[m][l] == MINA) {
        conteo++;
      }
    }
  }
  return conteo;
}

// Devuelve un número aleatorio entre minimo y maximo, incluyendo a minimo y
// maximo
// https://parzibyte.me/blog/2019/03/21/obtener-numeros-aleatorios-c/
int aleatorio_en_rango(int minimo, int maximo) {
  return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

void iniciarTablero(char tablero[FILAS][COLUMNAS]) {
  int l;
  for (l = 0; l < FILAS; l++) {
    int m;
    for (m = 0; m < COLUMNAS; m++) {
      tablero[l][m] = ESPACIO_SIN_DESCUBRIR;
    }
  }
}

void colocarMina(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
  tablero[fila][columna] = MINA;
}

void colocarMinas(char tablero[FILAS][COLUMNAS]) {
  int l;
  for (l = 0; l < CANTIDAD_MINAS; l++) {
    int fila = aleatorio_en_rango(0, FILAS - 1);
    int columna = aleatorio_en_rango(0, COLUMNAS - 1);
    colocarMina(fila, columna, tablero);
  }
}

void imprimirSeparadorEncabezado() {
  int m;
  for (m = 0; m <= COLUMNAS; m++) {
    printf("----");
    if (m + 2 == COLUMNAS) {
      printf("-");
    }
  }
  printf("\n");
}

void imprimirSeparadorFilas() {
  int m;
  for (m = 0; m <= COLUMNAS; m++) {
    printf("+---");
    if (m + 2 == COLUMNAS) {
      printf("+");
    }
  }
  printf("\n");
}

void imprimirEncabezado() {
  imprimirSeparadorEncabezado();
  printf("|   ");
  int l;
  for (l = 0; l < COLUMNAS; l++) {
    printf("| %d ", l + 1);
    if (l + 1 == COLUMNAS) {
      printf("|");
    }
  }
  printf("\n");
}

char enteroACaracter(int numero) {
  return numero + '0';
}

void imprimirTablero(char tablero[FILAS][COLUMNAS]) {
  imprimirEncabezado();
  imprimirSeparadorEncabezado();
  char letra = 'A';
  int l;
  for (l = 0; l < FILAS; l++) {
    int m;
    printf("| %c ", letra);
    letra++;
    for (m = 0; m < COLUMNAS; m++) {
      char verdaderaLetra = ESPACIO_SIN_DESCUBRIR;
      char letraActual = tablero[l][m];
      if (letraActual == MINA) {
        verdaderaLetra = ESPACIO_SIN_DESCUBRIR;
      } else if (letraActual == ESPACIO_DESCUBIERTO) {
        int minasCercanas = obtenerMinasCercanas(l, m, tablero);
        // printf("Minas: %d", minasCercanas);
        verdaderaLetra = enteroACaracter(minasCercanas);
      }

      if (letraActual == MINA && DEBUG) {
        verdaderaLetra = MINA;
      }
      printf("| %c ", verdaderaLetra);
      if (m + 1 == COLUMNAS) {
        printf("|");
      }
    }
    printf("\n");
    imprimirSeparadorFilas();
  }
}

// Recibe la fila, columna y tablero. La fila y columna deben ser tal y como las
// proporciona el usuario. Es decir, la columna debe comenzar en 1 (no en cero
// como si fuera un índice) y la fila debe ser una letra
int abrirCasilla(char filaLetra, int columna, char tablero[FILAS][COLUMNAS]) {
  filaLetra = toupper(filaLetra);
  columna--;
  int fila = filaLetra - 'A';
  assert(columna < COLUMNAS && columna >= 0);
  assert(fila < FILAS && fila >= 0);
  if (tablero[fila][columna] == MINA) {
    return ERROR_MINA_ENCONTRADA;
  }
  if (tablero[fila][columna] == ESPACIO_DESCUBIERTO) {
    return ERROR_ESPACIO_YA_DESCUBIERTO;
  }
  tablero[fila][columna] = ESPACIO_DESCUBIERTO;
  return ERROR_NINGUNO;
}

int main() {
  char tablero[FILAS][COLUMNAS];
  // Alimentar rand
  srand(getpid());
  iniciarTablero(tablero);
  colocarMina(1, 1, tablero);
  colocarMina(0, 1, tablero);
  colocarMina(1, 0, tablero);
  abrirCasilla('A', 1, tablero);
  abrirCasilla('A', 3, tablero);
  abrirCasilla('A', 4, tablero);
  abrirCasilla('B', 3, tablero);
  abrirCasilla('B', 4, tablero);
  abrirCasilla('C', 1, tablero);
  abrirCasilla('C', 2, tablero);
  abrirCasilla('C', 3, tablero);
  abrirCasilla('C', 4, tablero);
  abrirCasilla('D', 1, tablero);
  abrirCasilla('D', 2, tablero);
  abrirCasilla('D', 3, tablero);
  abrirCasilla('D', 4, tablero);
  imprimirTablero(tablero);
  //   while (1) {
  //       int fila, columna;
  //     printf("Ingresa la columna: ")
  //   }
  return 0;
}