-module(ej3).
-export([bal/1, server/1, sender/1, init/0, spawnSenders/2]).

% bal(Servs) ->
%     receive
%         {req, Arg, Pid} ->
%         Proc = lists:nth(rand:uniform(length(Servs)), Servs),
%         Proc ! { req, Arg, self() },
%         receive
%             Reply -> Pid ! Reply % el problema es que se queda esperando la respuesta y no va a manejar otros pedidos que lleguen en el medio
%         end
%     end,
%     bal(Servs).

sender(PidBal) ->
    PidBal ! {req, [], self()},
    receive
        {svok, Id} ->
            io:fwrite("Llegó ok de: ~p~n",[Id])
    end.

bal(Servs) ->
    receive
        {req, Arg, Pid} ->
        io:fwrite("Llegó un mensaje al balanceador!\n"),
        Proc = lists:nth(rand:uniform(length(Servs)), Servs),
        ReplyPid = spawn(
            fun () ->
                receive
                    Reply -> Pid ! Reply
                end
            end
        ),
        Proc ! { req, Arg, ReplyPid }
    end,
    bal(Servs).

server(Id) ->
    receive
        {req, _, Pid} -> 
            timer:sleep(1000),
            Pid ! {svok, Id}
    end,
    server(Id).

spawnSenders(0, _) -> spawnok;
spawnSenders(Cnt, PidBal) -> 
    spawn(ej3, sender, [PidBal]),
    timer:sleep(250),
    spawnSenders(Cnt-1, PidBal).

init() ->
    Pid1 = spawn(ej3, server, [1]),
    Pid2 = spawn(ej3, server, [2]),
    Pid3 = spawn(ej3, server, [3]),
    PidBal = spawn(ej3, bal, [[Pid1, Pid2, Pid3]]),
    spawnSenders(10, PidBal).