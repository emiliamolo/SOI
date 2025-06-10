-module(cliente).
-export([cliente/0, run/1]).
-define(Puerto, 8000). % hacer en un header idealmente
-define(Dir, "localhost"). % hacer en un header idealmente

cliente() ->
    case gen_tcp:connect(?Dir, ?Puerto, [binary, {packet, 1}]) of
        {ok, Socket} -> run(Socket);
        {error, Reason} -> io:format("Se produjo un error: ~p~n", [Reason])
    end.

run(Socket) -> 
    {ok, Str} = io:fread("Enviar: ","~s"),
    gen_tcp:send(Socket, Str),
    case Str of
        ["CHAU"] -> gen_tcp:close(Socket),
                    ok;
        _      -> 
            receive
                {tcp, Socket, Paquete} -> io:format("Recibí ~p ~n", [Paquete])
            end,
            run(Socket)
    end.