#ifndef __COLA_H__
#define __COLA_H__

#include "glist.h"

typedef GList Cola;

/**
 * Crea una nueva cola. 
 */
Cola cola_crear();

/**
 * Libera la memoria ocupada por una cola. 
 */
void cola_destruir(Cola, FuncionDestructora);

/**
 * Devuelve 1 si la cola es vacía.
 * Caso contrario devuelve 0
 */
int cola_es_vacia(Cola);

/**
 * Devuelve el primer elemento de una cola,
 * sin eliminarlo.
*/
void* cola_inicio(Cola);

/**
 * Agrega un elemento al final de la cola.
 */
Cola cola_encolar(Cola, void*, FuncionCopia);

/**
 * Elimina el primer elemento de la cola.
 */
Cola cola_desencolar(Cola, FuncionDestructora);

/**
 * Recorre los elementos de la cola en orden del primero
 * al último.
 */
void cola_imprimir(Cola, FuncionVisitante);

#endif /* __COLA_H__ */