#include <assert.h>
#include <ctype.h>  // toupper
#include <stdio.h>
#include <stdlib.h>  // rand
#include <unistd.h>  // getpid
#define COLUMNAS 7
#define FILAS 7
#define ESPACIO_SIN_DESCUBRIR '?'
#define ESPACIO_DESCUBIERTO ' '
#define MINA 'M'
#define CANTIDAD_MINAS 5
#define ERROR_MINA_ENCONTRADA 1
#define ERROR_ESPACIO_YA_DESCUBIERTO 2

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

void colocarMinas(char tablero[FILAS][COLUMNAS]) {
  int l;
  for (l = 0; l < CANTIDAD_MINAS; l++) {
    int fila = aleatorio_en_rango(0, FILAS - 1);
    int columna = aleatorio_en_rango(0, COLUMNAS - 1);
    tablero[fila][columna] = MINA;
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
      printf("| %c ", tablero[l][m]);
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
}

int main() {
  char tablero[FILAS][COLUMNAS];
  // Alimentar rand
  srand(getpid());
  iniciarTablero(tablero);
  colocarMinas(tablero);
  imprimirTablero(tablero);
  abrirCasilla('A', 1, tablero);
  imprimirTablero(tablero);
  abrirCasilla('G', 7, tablero);
  imprimirTablero(tablero);
  //   while (1) {
  //       int fila, columna;
  //     printf("Ingresa la columna: ")
  //   }
  return 0;
}