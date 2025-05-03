a. Puede terminar en deadlock si todos logran tomar el tenedor derecho, lock(der(i)). Todos los mutex estarian tomados y ninguno lo soltaria hasta tomar otro tenedor (lock(izq(i))), pero no lo logran nunca porque todos estan ocupados.

b. 