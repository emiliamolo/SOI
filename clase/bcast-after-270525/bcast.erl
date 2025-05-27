-module(bcast).
-export([iniciar/0, finalizar/1]).
-export([broadcast/2, registrar/1]).
-export([loopBroadcast/1]).
-export([proceso1/1, proceso2/1, procesoReceiver/1, receiverLoop/1]).

loopBroadcast(PidList) -> 
    receive  
        {register, Pid} -> loopBroadcast([Pid|PidList]);
        {bcast, Value} ->
            lists:foreach(fun(Pid)-> Pid ! {msg, Value} end, PidList),
            loopBroadcast(PidList);
        fin -> loopok
    end.


iniciar() ->
    PidBcast = spawn(bcast, loopBroadcast, [[]]),
    spawn(bcast, proceso1, [PidBcast]),
    spawn(bcast, proceso2, [PidBcast]).

broadcast(PidBcast, Value) -> PidBcast ! {bcast, Value}.

finalizar(PidBcast) -> PidBcast ! fin.

registrar(PidBcast) -> PidBcast ! {register, self()}.

receiverLoop(PidBcast) -> 
    receive
        {msg, Value} -> 
            io:fwrite("~p~n",[Value]),
            receiverLoop(PidBcast);
        fin -> 
            io:fwrite("Receiver apagando...~n"),
            finok
    end.

procesoReceiver(PidBcast) ->
    registrar(PidBcast),
    receiverLoop(PidBcast).

proceso1(PidBcast) ->
    PidReceiver = spawn(bcast, procesoReceiver, [PidBcast]),
    timer:sleep(1500),
    broadcast(PidBcast, "HOLA DESDE PROCESO 1"),
    timer:sleep(3000),
    PidReceiver ! fin.
    
proceso2(PidBcast) ->
    PidReceiver = spawn(bcast, procesoReceiver, [PidBcast]),
    timer:sleep(500),
    broadcast(PidBcast, "HOLA DESDE PROCESO 2"),
    timer:sleep(3000),
    PidReceiver ! fin.