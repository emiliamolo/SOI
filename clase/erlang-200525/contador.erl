-module(contador).
-export([run/0, counter/1]).

counter(Cnt) ->
    receive
        inc -> 
            io:fwrite("Nuevo valor: ~p~n",[Cnt+1]),
            counter(Cnt+1);
        {query, PidRequest} ->
            io:fwrite("Valor consultado: ~p~n",[Cnt]),
            PidRequest ! Cnt,
            counter(Cnt);
        fin ->
            io:fwrite("Termina counter ~n"),
            counterok
    end.

run() -> % Guardar el pid devuelto para mandar mensajes con '!'
    PidContador = spawn(contador, counter, [0]),
    PidContador.