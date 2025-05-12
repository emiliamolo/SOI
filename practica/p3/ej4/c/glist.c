#include <stdlib.h>

#include "glist.h"

GList glist_crear() {
    GList nuevo;
    nuevo.primero = nuevo.ultimo = NULL;
    return nuevo;
}

void glist_destruir(GList list, FuncionDestructora destroy) {
    GNodo *cur = list.primero;
    while (cur) {
        GNodo *aux = cur;
        cur = cur->sig;
        destroy(aux->dato);
        free(aux);
    }
}

int glist_vacia(GList list) {
    return list.primero == NULL;
}

GList glist_agregar_final(GList list, void *dato, FuncionCopia copy) {
    GNodo *nuevo = malloc(sizeof(GNodo));
    nuevo->dato = copy(dato);
    nuevo->sig = NULL;
    if (list.ultimo)
        list.ultimo->sig = nuevo;
    list.ultimo = nuevo;
    if (!list.primero)
        list.primero = list.ultimo;
    return list;
}

GList glist_eliminar_inicio(GList list, FuncionDestructora destroy) {
    if (!list.primero)
        return list;
    GNodo *nuevoInicio = list.primero->sig;
    destroy(list.primero->dato);
    free(list.primero);
    list.primero = nuevoInicio;
    if (!list.primero)
        list.ultimo = list.primero;
    return list;
}

void glist_recorrer(GList list, FuncionVisitante visit) {
    for (GNodo *cur = list.primero; cur; cur = cur->sig)
        visit(cur);
}