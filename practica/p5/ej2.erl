-module(ej2).
-export([wait/1, waitexec/3, cronometro/3]).

wait(Millis) -> timer:sleep(Millis).

waitexec(Periodo, FunExec, Args) ->
    wait(Periodo),
    apply(ej2, FunExec, Args).

cronometro(Fun, Hasta, Periodo) -> 
    Fun(),
    spawn(ej2, waitexec, [Periodo, cronometro, [Fun, Hasta - Periodo, Periodo]]).