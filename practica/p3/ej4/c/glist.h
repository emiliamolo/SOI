#ifndef __GLIST_H__
#define __GLIST_H__

typedef void* (*FuncionCopia)(void*);
typedef void (*FuncionDestructora)(void*);
typedef void (*FuncionVisitante)(void*);

typedef struct _GNodo {
    void *dato;
    struct _GNodo *sig;
} GNodo;

typedef struct _GList {
    GNodo *primero, *ultimo;
} GList;

/**
 * Crea una nueva lista.
 */
GList glist_crear();

/**
 * Libera la memoria ocupada por la lista.
 */
void glist_destruir(GList, FuncionDestructora);

/**
 * Devuelve 1 si la lista está vacía.
 * Caso contrario devuelve 0.
 */
int glist_vacia(GList);

/**
 * Agrega un elemento al final de la lista.
 */
GList glist_agregar_final(GList, void*, FuncionCopia);

/**
 * Elimina el primer elemento de la lista.
 */
GList glist_eliminar_inicio(GList, FuncionDestructora);

/**
 * Recorre los elementos de la lista utilizando
 * la función visitante.
 */
void glist_recorrer(GList, FuncionVisitante);

#endif /* __GLIST_H__ */