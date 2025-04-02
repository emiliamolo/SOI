En el hijo cambia el `getpid()` ya que tiene un pid distinto.
En cambio, al ejecutar `exec()`, se mantiene el pid del padre, ya que el proceso ejecutado toma el pid de dicho padre.