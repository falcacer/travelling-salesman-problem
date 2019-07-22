#ifndef __SList_H__
#define __SList_H__

#include <stddef.h>

typedef struct _SNodo
{
  char *dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

// Esta funcion crea una lista.
SList slist_crear();

// Esta funcion verifica si la lista se encuentra vacia.
int slist_vacio(SList lista);

// Esta funcion recibe una lista y un dato agregado dicho dato al final
// de la lista.
SList slist_agregar_inicio(SList lista, char *dato);

// Esta funcion calcula la longitud de la lista.
int slist_len(SList lista);

// Esta funcion destruye la lista.
void slist_destruir(SList lista);

// Esta funcion imprime la lista.
void slist_recorrer(SList lista);

#endif /* __SList_H__ */