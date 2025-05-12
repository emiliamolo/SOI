#include "cola.h"

#include <stdlib.h>

Cola cola_crear() {
    return glist_crear();
}

void cola_destruir(Cola cola, FuncionDestructora destroy) {
    glist_destruir(cola, destroy);
}

int cola_es_vacia(Cola cola) {
    return glist_vacia(cola);
}

void* cola_inicio(Cola cola) {
    if (cola_es_vacia(cola))
        return NULL;
    return cola.primero->dato;
}

Cola cola_encolar(Cola cola, void *dato, FuncionCopia copy) {
    return glist_agregar_final(cola, dato, copy);
}

Cola cola_desencolar(Cola cola, FuncionDestructora destroy) {
    return glist_eliminar_inicio(cola, destroy);
}

void cola_imprimir(Cola cola, FuncionVisitante visit) {
    glist_recorrer(cola, visit);
}

