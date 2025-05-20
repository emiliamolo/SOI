-module(pingpong).
-export([pong/0, ping/0, play/1]).

ping() ->
    receive
        {0, PidPong} -> 
            io:fwrite("final ping~n"),
            PidPong ! fin,
            pingok;
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
            pongok
    end.

play(N) -> 
    PidPong = spawn(pingpong, pong, []),
    PidPing = spawn(pingpong, ping, []),
    PidPong ! {N, PidPing},
    playok.