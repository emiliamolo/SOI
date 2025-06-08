-module(clientEcho).
-export([cliente/0]).
-define(Puerto, 1234). % hacer en un header idealmente
-define(Dir, "localhost").

cliente() ->
    case gen_tcp:connect(?Dir, ?Puerto, [binary, {active, false}]) of
        {ok, Socket} -> 
            {ok, Str} = io:fread("Enviar: ","~s"),
            gen_tcp:send(Socket, Str),
            case gen_tcp:recv(Socket, 0) of
                {ok, Paquete} -> io:format("Recibí el echo ~p ~n", [Paquete]);
                {err, Reason} -> io:format("Error: ~p ~n", [Reason])
            end,
            gen_tcp:close(Socket),
            ok;
        {error, Reason} -> io:format("Se produjo un error~p", [Reason])
    end.