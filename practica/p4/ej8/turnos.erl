-module(turnos).
-export([server/0, turnero_run/1, nuevo_turno/1, get_request/1]).

turnero_run(N) ->
    receive
        {nuevo, Pid} -> 
            Pid ! {turno, N},
            turnero_run(N+1)
    end.

server() ->
    PidTurnero = spawn(fun () -> turnero_run(1) end),
    register(turnero, PidTurnero),

    % {packet,1} agrega un header de 1 byte a cada paquete indicando la cantidad de bytes del paquete,
    % y lo elimina al recibir, automáticamente devolviendo a nuestro programa los paquetes enteros,
    % en vez de paquetes pegados o divididos si no lo usáramos, siendo que el protocolo TCP puede
    % hacerlo por defecto en el bajo nivel por ser un stream de datos.
    % Esto tiene que estar tanto del lado del servidor como del cliente.
    % OBS: Cuando lo hacíamos en C, leíamos bit a bit, hasta un \n, nuestro delimitador.
    {ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}, {packet, 1}]),
    wait_connect(ListenSocket, 0).

wait_connect(ListenSocket, N) ->
    {ok, Socket} = gen_tcp:accept(ListenSocket),
    spawn (fun () -> wait_connect (ListenSocket, N+1) end),
    get_request(Socket).

get_request(Socket) ->
    receive
        {tcp, Socket, Paquete} -> 
            case Paquete of 
                "NUEVO" -> 
                    spawn(fun () -> nuevo_turno(Socket) end),
                    get_request(Socket);
                "CHAU" -> 
                    gen_tcp:close(Socket);
                _ -> 
                    gen_tcp:send(Socket, "ENOCOMMAND"),
                    get_request(Socket)
            end
    end.

nuevo_turno(Socket) -> 
    turnero ! {nuevo, self()},
    receive
        {turno, N} -> gen_tcp:send(Socket, integer_to_list(N))
    end.

% TODO: Testear con test_cliente, agregar ~n a los mensajes para mantener consistente el protocolo!!!!