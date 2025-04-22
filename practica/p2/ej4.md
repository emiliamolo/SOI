4) ¿Puede fallar la siguiente aserción (wr y rd corren en un thread cada uno)? Explique. Si puede fallar, arregle el programa.
volatile int x = 0; 
volatile int y = 0; 
void * wr(void *arg) { x = 123; y = 1; } 
void * rd(void *arg) {
    while (!y); 
    assert(x == 123); 
}

`volatile` solo le indica al compilador que no optimice el acceso a esa variable. La lee y escribe directamente en memoria cada vez, no la guarda en registros. Esto hace que no la optimice fuera del loop y que no la elimine por considerar que "no cambia".  
PERO, esto no garantiza que no se ejecute fuera de orden o que se sincronizen las memorias. Por lo tanto, al igual que en el ejercicio 3 puede fallar. Por ejemplo: `wr` ejecuta y = 1, cambia de contexto y `rd` sale del while y assert(x == 123) es falso.

> The volatile keyword was devised to prevent compiler optimizations that might render code incorrect in the presence of certain asynchronous events. For example, if you declare a primitive variable as volatile, the compiler is not permitted to cache it in a register -- a common optimization that would be disastrous if that variable were shared among multiple threads. So the general rule is, if you have variables of primitive type that must be shared among multiple threads, declare those variables volatile. But you can actually do a lot more with this keyword: you can use it to catch code that is not thread safe, and you can do so at compile time. This article shows how it is done; the solution involves a simple smart pointer that also makes it easy to serialize critical sections of code. https://stackoverflow.com/questions/246127/why-is-volatile-needed-in-c
