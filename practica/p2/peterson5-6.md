5) Implemente el algoritmo de Peterson para solucionar el problema del jardin ornamental. Tenga en cuenta lo discutido sobre barreras de memoria.
El *algortimo de peterson* soluciona el problema del *jardin ornamental* si consideramos un sistema con un unico procesador. Garantiza *exclusion mutua* entre los procesos.
Pero en un sistema *multicore* se rompe por ejecución fuera de orden y store buffering.
Por lo tanto, es necesario agregar barreras de memoria o mutex para sincronizar de manera correcta.
Una opcion es agregar barreras de memoria (asm("mfence")) y usando `volatile` para evitar que el compilador optimice el acceso a flag y turno.

6) Considere el problema del jardín ornamental en un sistema con un único procesador.
6) a- ¿Sigue habiendo un problema? Justifique.
El algoritmo de Peterson fue diseñado pensando en sistemas con un solo procesador. En ese contexto, no hay ejecución paralela real: los hilos se intercalan, pero nunca se ejecutan exactamente al mismo tiempo.
No hay ejecución fuera de orden visible:
Aunque el compilador o procesador podrían reordenar instrucciones para optimizar, esto no rompe la exclusión mutua, ya que los cambios no se propagan a otros núcleos.
No hay store buffering: En un sistema monocore, la memoria es compartida por todos los hilos y accedida en el mismo orden secuencial. No existe el problema de que un núcleo no vea los cambios de otro por retrasos en caché.

6) b- Si implementa el algoritmo de Peterson, ¿son necesarias las barreras de memoria? 
En un sistema con un solo procesador, no son estrictamente necesarias las barreras de memoria. En, multicore SI.

6) c- Si el incremento se hace con la instrucción incl de x86, ¿hay problema? Puede aprovechar la siguiente función:    
    static inline void incl(int *p) { 
        asm("incl %0" : "+m"(*p) : : "memory"); 
    }
 Con un solo hilo no habría problema.
6) d- ¿Qué pasa con la implementación con incl al tener más de un procesador?
 incl no garantiza exclusión mutua entre hilos que se ejecutan en distintos núcleos, no es atomica.
 Se carga el dato de memoria y luego se incrementa.
6) e- Repita el experimento con esta versión de incl: 
    static inline void incl(int *p) { 
        asm("lock; incl %0" : "+m"(*p) : : "memory"); 
    }
Con esta versión, el incremento es atómico y seguro incluso en sistemas multiprocesador. 