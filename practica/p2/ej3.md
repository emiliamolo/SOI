int x = 0, y = 0, a = 0, b = 0;
void * foo(void *arg) { 
    x = 1; 
    a = y; 
    return NULL; 
} 
void * bar(void *arg) { 
    y = 1; 
    b = x; 
    return NULL; 
} 
int main() {
    pthread_t t0, t1; 
    pthread_create(&t0, NULL, foo, NULL); 
    pthread_create(&t1, NULL, bar, NULL); 
    pthread_join(t0, NULL); 
    pthread_join(t1, NULL); 
    assert (a || b); 
    return 0; 
}

Puede fallar debido a la falta de sincronizacion y concurrencia. Las dos funciones corren en threads diferentes y acceden a variables compartidas sin sincronización lo que puede causar que falle. 
Por ejemplo: 
    - foo corre primero, x = 1, a = y, como y todavia no se escribió a = 0
    - Luego corre bar, y = 1, b = x.  
    Si los threads corren en nucleos distintos, como cada nucleo tiene su propia cache, estas podrian estar desactualizadas. Entonces, aunque foo ya escribió `x=1` puede suceder que las *caches sigan desactualizadas* y bar lea x = 0.
Tambien puede fallar por la *ejecución fuera de orden*(out-of-order execution): Tanto los compiladores como el hardware pueden reordenar la instrucciones para optimizar el rendimiento, como no se esta sincronizando de ninguna manera no hay garantías de que las operaciones sucedan en el orden en que se escribieron en el código. Esto sucede al tener varios procesadores, cada nucleo puede reordenar las intrucciones internamente si no sabe que estas variables son compartidas y modificadas por otros threads. Si tengo un solo nucleo, tambien puede suceder tanto que el compilador reordene como que el CPU ejecute en otro orden para aprovechar el pipeline. El reodenamiento se hace de forma que desde su punto de vista el resultado final sea el mismo, pero esto ya no es suficiente porque otros hilos pueden trabajar sobre resultados intermedios. 
