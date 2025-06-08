-module(servEcho).
-define(Puerto, 1234).
-export([start/0, receptor/1, echo/1]).

start() ->
    {ok, Socket} = gen_tcp:listen(?Puerto, [binary, {active, false}]),
    spawn(?MODULE, receptor,[Socket]). % esto podria ser solo receptor(Socket) sin spawn

receptor(Socket) ->
    case gen_tcp:accept(Socket) of
        {ok, CSocket} -> 
            spawn(?MODULE, echo,[CSocket]),
            receptor(Socket);
        {error, _} -> error % deberia manejar todos los posibles errores, ver manual
    end.

echo(Socket) ->
    case gen_tcp:recv(Socket,0) of % con 0 es para leer todo lo que pueda
        {ok, Paquete} -> 
            io:format("El servidor recibió ~p ~n", [Paquete]),
            gen_tcp:send(Socket, Paquete),
            echo(Socket);
        {error, closed} -> io:format("Se cerró la conexion ~n")
    end.