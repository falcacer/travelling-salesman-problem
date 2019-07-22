#include "slist.h"
#include <stdio.h>
#include <stdlib.h>

SList slist_crear()
{
  return NULL;
}

int slist_vacio(SList lista)
{
  return (lista == NULL);
}

SList slist_agregar_inicio(SList lista, char *dato)
{
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

int slist_len(SList lista)
{
  if (slist_vacio(lista))
  {
    return 0;
  }
  int contador = 1;
  SList i = lista;
  while (i->sig != NULL)
  {
    contador++;
    i = i->sig;
  }
  return contador;
}

void slist_destruir(SList lista)
{
  SNodo *nodoAEliminar;
  while (lista != NULL)
  {
    nodoAEliminar = lista;
    lista = lista->sig;
    free(nodoAEliminar);
  }
}

void slist_recorrer(SList lista)
{
  for (SNodo *nodo = lista; nodo != NULL; nodo = nodo->sig)
  {
    printf("%s\n", nodo->dato);
  }
}
