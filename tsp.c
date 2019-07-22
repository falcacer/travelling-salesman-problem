#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "slist.h"
#define SIZEOFNAME 42

typedef struct _Ciudades
{
  char **nombres;
  int cantidad;
  int *movimientos;
  int *matrizcostos;
  int costot;
} * Ciudades;

// crear_ciudades: int -> Ciudades.
// Esta funcion recibe un numero >= 1 y crea la estructura necesaria para
// resolver el problema.
Ciudades crear_ciudades(int cantidad)
{
  Ciudades nuevaciudades = malloc(sizeof(struct _Ciudades));
  nuevaciudades->costot = __INT_MAX__;
  nuevaciudades->movimientos = calloc(cantidad, sizeof(int));
  if (cantidad < 1)
  {
    nuevaciudades->nombres = NULL;
    nuevaciudades->cantidad = cantidad;
    nuevaciudades->matrizcostos = NULL;
    return nuevaciudades;
  }
  nuevaciudades->cantidad = cantidad;
  nuevaciudades->nombres = malloc(sizeof(char *) * cantidad);
  nuevaciudades->matrizcostos = calloc((cantidad * cantidad), sizeof(int));
  return nuevaciudades;
}

void ciudad_destruir(Ciudades c)
{
  for (int i = 0; i < c->cantidad; i++)
  {
    free(c->nombres[i]);
  }
  free(c->nombres);
  free(c->matrizcostos);
  free(c->movimientos);
  free(c);
}

// Dada una lista con los nombres de las ciudades y el nombre de una de estas
// ciudades la funcion obtener_index devuelve el indice de la lista donde se
// encuentra dicha ciudad.
int obtener_index(SList list, char *ciudad)
{
  SList nodo = list;
  int i = 0;
  for (; nodo != NULL && strcmp(ciudad, nodo->dato); i++, nodo = nodo->sig)
    ;
  return i;
}

// Esta funcion llena el campo matrizcostos de la esctructura ciudades
// con los costos de cada viaje.
void ciudades_matriz(Ciudades c, int fila, int columna, int costo)
{
  if (fila < c->cantidad && columna < c->cantidad)
    c->matrizcostos[c->cantidad * fila + columna] = costo;
  else
  {
    printf("ERROR DE INDICE.\n");
  }
}

// Imprime la matriz de costos.
void imprimir_matriz(Ciudades c)
{
  for (int i = 0; i < c->cantidad; i++)
  {
    for (int j = 0; j < c->cantidad; j++)
    {
      printf("%d ", c->matrizcostos[c->cantidad * i + j]);
    }
    printf("\n");
  }
}

char *obtener_elemento(SList list, int pos)
{
  SNodo *nodo = list;
  int i = 0;
  while (i != pos)
  {
    nodo = nodo->sig;
    i++;
  }
  return nodo->dato;
}

char *obtener_ciudad(int indice, char **ciudades)
{
  return ciudades[indice];
}

Ciudades input(char *ingreso)
{
  FILE *input;
  input = fopen(ingreso, "r");
  if (input == NULL)
  {
    printf("EL ARCHIVO NO EXISTE.\n");
    Ciudades nuevaciudad = crear_ciudades(0);
    return nuevaciudad;
  }
  int Flag = 1;
  int cantidad = 0;
  char nombres[SIZEOFNAME];
  char *token;
  SList list = slist_crear();
  fscanf(input, "%s", nombres); // ignora la primera linea.
  while (Flag)
  {
    fscanf(input, "%s", nombres);
    if (!strcmp(nombres, "Costos"))
      Flag = 0;

    if (Flag)
    {
      int len = strlen(nombres);
      if (nombres[len - 1] == ',')
        nombres[len - 1] = '\0';
      char *cadena = malloc(sizeof(char) * (len + 1));
      strcpy(cadena, nombres);
      list = slist_agregar_inicio(list, cadena);
      cantidad++;
    }
  }
  Ciudades ciudades = crear_ciudades(cantidad);
  for (int i = 0; i < cantidad; i++)
  {
    char *ciudad = obtener_elemento(list, i);
    ciudades->nombres[i] = ciudad;
  }
  while (fscanf(input, "%s", nombres) != EOF)
  {
    token = strtok(nombres, ",");
    int index1 = obtener_index(list, token);
    token = strtok(NULL, ",");
    int index2 = obtener_index(list, token);
    token = strtok(NULL, "\n");
    int costo = atoi(token);
    ciudades_matriz(ciudades, index1, index2, costo);
    ciudades_matriz(ciudades, index2, index1, costo);
  }
  // imprimir_matriz(ciudades);
  slist_destruir(list);
  fclose(input);
  return ciudades;
}

void obterner_recorrido(Ciudades c, int *actual, int *visitados, int count, int costo)
{
  if (count == c->cantidad - 1)
  {
    int CostoVuelta = c->matrizcostos[c->cantidad * actual[count - 1] + actual[count]];
    if (CostoVuelta && (costo + CostoVuelta) < c->costot)
    {
      for (int i = 0; i < c->cantidad; i++)
      {
        c->movimientos[i] = actual[i];
      }
      c->costot = costo + CostoVuelta;
    }
  }
  else
  {
    for (int i = 1; i < c->cantidad; i++)
    {
      int _costo = c->matrizcostos[c->cantidad * actual[count - 1] + i];
      if (!visitados[i] && _costo)
      {
        actual[count] = i;
        visitados[i] = 1;
        obterner_recorrido(c, actual, visitados, count + 1, (costo + _costo));
        visitados[i] = 0;
      }
    }
  }
}

void resolver(Ciudades c)
{
  int cantidad = c->cantidad;
  int visitados[cantidad];
  int actual[cantidad];
  for (int k = 0; k < cantidad; k++)
  {
    visitados[k] = 0;
    actual[k] = 0;
  }
  for (int i = 1; i < cantidad; i++)
  {
    visitados[i] = 1;
    actual[1] = i;
    int costo1 = c->matrizcostos[cantidad * i];
    if (costo1)
    {
      for (int j = i + 1; j < cantidad; j++)
      {
        int costo2 = c->matrizcostos[cantidad * j];
        if (costo2)
        {
          visitados[j] = 1;
          actual[cantidad - 1] = j;
          obterner_recorrido(c, actual, visitados, 2, (costo1 + costo2));
          visitados[j] = 0;
        }
      }
      visitados[i] = 0;
    }
  }
}

void salida(char *ArchivoNombre, Ciudades c)
{
  FILE *output;
  output = fopen(ArchivoNombre, "w+");
  fprintf(output, "Costo de viaje: %d\n", c->costot);
  for (int i = 0; i < c->cantidad - 2; i++)
  {
    int j = i + 1;
    int index1 = c->movimientos[i];
    int index2 = c->movimientos[j];
    printf("%d %d\n", index1, index2);
    int costo = c->matrizcostos[c->cantidad * index1 + index2];
    if (costo)
    {
      char *ciudad1 = obtener_ciudad(index1, c->nombres);
      char *ciudad2 = obtener_ciudad(index2, c->nombres);
      fprintf(output, "%s,%s,%d\n", ciudad1, ciudad2, costo);
    }
  }
  fclose(output);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Error sobre los argumentos ingresados.\n");
    return 1;
  }
  __clock_t start, end;
  double time;
  Ciudades ciudades = input(argv[1]);

  int cantidad = ciudades->cantidad;

  start = clock();

  resolver(ciudades);

  // obterner_recorrido(ciudades, actual, visitados, 1, 0);
  end = clock();
  time = ((double)end - start / CLOCKS_PER_SEC);
  printf("%fs\n", time);

  // printf("costo total: %d\n", ciudades->costot);
  // for (int i = 0; i < cantidad; i++)
  //   printf("%d\n", ciudades->movimientos[i]);

  // for (int i = 0; i < cantidad; i++)
  //   printf("%s\n", ciudades->nombres[i]);

  salida(argv[2], ciudades);
  ciudad_destruir(ciudades);

  return 0;
}
