/*
  ____          _____               _ _           _
 |  _ \        |  __ \             (_) |         | |
 | |_) |_   _  | |__) |_ _ _ __ _____| |__  _   _| |_ ___
 |  _ <| | | | |  ___/ _` | '__|_  / | '_ \| | | | __/ _ \
 | |_) | |_| | | |  | (_| | |   / /| | |_) | |_| | ||  __/
 |____/ \__, | |_|   \__,_|_|  /___|_|_.__/ \__, |\__\___|
         __/ |                               __/ |
        |___/                               |___/

    Blog:       https://parzibyte.me/blog
    Ayuda:      https://parzibyte.me/blog/contrataciones-ayuda/
    Contacto:   https://parzibyte.me/blog/contacto/

    Copyright (c) 2020 Luis Cabrera Benito
    Licenciado bajo la licencia MIT

    El texto de arriba debe ser incluido en cualquier redistribución

  Errores esperados, documentación y esas cosas...
  - No puede haber más filas que letras del abecedario, porque solo se imprime
  hasta la Z
  - No puede haber más de 9 columnas porque ya no se imprimiría bien, y porque
  ya no se convertiría correctamente el entero a cadena para mostrar las minas
  cercanas
*/

#include <assert.h>
#include <ctype.h>  // toupper
#include <stdio.h>
#include <stdlib.h>  // rand
#include <unistd.h>  // getpid
#define COLUMNAS 5
#define FILAS 5
#define ESPACIO_SIN_DESCUBRIR '.'
#define ESPACIO_DESCUBIERTO ' '
#define MINA '*'
#define CANTIDAD_MINAS 5
#define ERROR_MINA_ENCONTRADA 1
#define ERROR_ESPACIO_YA_DESCUBIERTO 2
#define ERROR_NINGUNO 3
#define DEBUG 0

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
  int m;
  for (m = filaInicio; m <= filaFin; m++) {
    int l;
    for (l = columnaInicio; l <= columnaFin; l++) {
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
int aleatorioEnRango(int minimo, int maximo) {
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

void colocarMinasAleatoriamente(char tablero[FILAS][COLUMNAS]) {
  int l;
  for (l = 0; l < CANTIDAD_MINAS; l++) {
    int fila = aleatorioEnRango(0, FILAS - 1);
    int columna = aleatorioEnRango(0, COLUMNAS - 1);
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
    if (m == COLUMNAS) {
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

void imprimirTablero(char tablero[FILAS][COLUMNAS], int deberiaMostrarMinas) {
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
        verdaderaLetra = enteroACaracter(minasCercanas);
      }

      if (letraActual == MINA && (DEBUG || deberiaMostrarMinas)) {
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

int noHayCasillasSinAbrir(char tablero[FILAS][COLUMNAS]) {
  int l;
  for (l = 0; l < FILAS; l++) {
    int m;
    for (m = 0; m < COLUMNAS; m++) {
      char actual = tablero[l][m];
      if (actual == ESPACIO_SIN_DESCUBRIR) {
        return 0;
      }
    }
  }
  return 1;
}

int main() {
  printf("** BUSCAMINAS **\nBy Parzibyte\n");
  char tablero[FILAS][COLUMNAS];
  int deberiaMostrarMinas = 0;
  // Alimentar rand
  srand(getpid());
  iniciarTablero(tablero);
  colocarMinasAleatoriamente(tablero);
  while (1) {
    imprimirTablero(tablero, deberiaMostrarMinas);
    if (deberiaMostrarMinas) {
      break;
    }
    int columna;
    char fila;
    printf("Ingresa la fila: ");
    scanf(" %c", &fila);
    printf("Ingresa la columna: ");
    scanf("%d", &columna);
    int status = abrirCasilla(fila, columna, tablero);
    if (noHayCasillasSinAbrir(tablero)) {
      printf("Has ganado\n");
      deberiaMostrarMinas = 1;
    } else if (status == ERROR_ESPACIO_YA_DESCUBIERTO) {
      printf("Ya has abierto esta casilla\n");
    } else if (status == ERROR_MINA_ENCONTRADA) {
      printf("Has perdido\n");
      deberiaMostrarMinas = 1;
    }
  }
  return 0;
}