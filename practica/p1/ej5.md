Es free() una llamada al sistema? Por que s o por que no?
Tecnicamente no es una llamada al sistema. La libreria de c intenta evitar llamadas al sistema cuando es posible, si hace llamadas al sistema cuando es necesario. No podria liberar la memoria sin llamar al sistema, pero podría ser conveniente no liberar la memoria, ahorrando una llamada al sistema, para luego reutilizar esa memoria ya reservada en una futura llamada a malloc.

