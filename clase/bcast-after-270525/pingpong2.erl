-module(pingpong2).
-export([pong/0, ping/0, play/1, ping_aux/1, pong_aux/0]).

% linkeo de procesos, los linkeamos, los dos estan linkeados, es mutuo
% si uno muere le avisa al otro y viceversa, tambien si termina correctamente
% si termino correctamente por defecto se ignora
% si no hay varias opciones: morir tambien, replicar .. o algo mas
ping_aux(PidPong) ->
    link(PidPong),
    ping(). % es bidireccionar, con esto alcanza, no hay que linkear pong

pong_aux() -> 
    process_flag(trap_exit, true), % para poder detectar la señal
    pong().
ping() ->
    receive
        {0, PidPong} -> 
            io:fwrite("final ping~n"),
            PidPong ! fin,
            pingok;
        % ping muere y pong no se entera
        {3, _PidPong} -> error(aAaA); % no termina, tira un error
        %{3, _PidPong} -> exit("aAaA"); % tampoco termina, pong queda esperando para siempre, no tira error
        
        {N, PidPong} -> 
            io:fwrite("Ping! Recv: ~p ~n", [N]),
            PidPong ! {N-1, self()},
            ping();
        fin -> 
            io:fwrite("Ping fin~n"),
            pingok
    end.

pong() ->
    receive
        {0, PidPing} -> 
            io:fwrite("final pong~n"),
            PidPing ! fin,
            pongok;
        {N, PidPing} -> 
            io:fwrite("Pong! Recv: ~p ~n", [N]),
            PidPing ! {N-1, self()},
            pong();
        fin -> 
            io:fwrite("Pong fin~n"),
            pongok;
        {'EXIT', _From, _Reason} -> 
            PidPing = spawn(?MODULE, ping_aux, [self()]), % murio ping, lo vuelo a lanzar
            io:format("Ping murio lo vuelvo a hacer~n"),
            io:format("nuevo ping ~p ~n", [PidPing]),
            PidPing ! {9, self()},
            pong()
        
    end.

play(N) -> 
    PidPong = spawn(?MODULE, pong_aux, []),
    io:format("Pid de pong: ~p ~n", [PidPong]),
    PidPing = spawn(?MODULE, ping_aux, [PidPong]),
    io:format("Pid de ping: ~p ~n", [PidPing]),
    PidPong ! {N, PidPing},
    playok.