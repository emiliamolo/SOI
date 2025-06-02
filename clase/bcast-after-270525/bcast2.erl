-module(bcast2).
-export([iniciar/0, finalizar/0]).
-export([broadcast/1, registrar/0]).
-export([loopBroadcast/1]).
-export([ack/0]).
-export([proceso1/0, proceso2/0, procesoReceiver/0, receiverLoop/0]).


%% igual al otro broadcast pero uso register para registrar el servidor y no tener que pasar el pid del dervidos a cada uno
loopBroadcast(PidList) -> 
    receive  
        {register, Pid} -> loopBroadcast([Pid|PidList]), Pid ! servOK;
        {bcast, Value, PidClient} ->
            lists:foreach(fun(Pid)-> Pid ! {msg, Value} end, PidList),
            PidClient ! servOK,
            loopBroadcast(PidList);
        {fin, PidClient} -> unregister(server), PidClient ! servOK;
        _Msg -> servError
    end.


iniciar() ->
    PidBcast = spawn(bcast2, loopBroadcast, [[]]),
    register(server, PidBcast), % registro el PidBcast, como nombre tiene el atomo 'server'
    % prueba
    spawn(bcast2, proceso1, []),
    spawn(bcast2, proceso2, []).

broadcast(Value) -> 
    server ! {bcast, Value, self()},
    ack().

ack() -> % verifico que el servidor funcione
    receive 
        servOK -> ok;
        servError -> err
    end.

finalizar() -> 
    server ! {fin, self()},
    ack(). % creo que habria que hacer unregister()

registrar() -> 
    server ! {register, self()},
    ack().

receiverLoop() -> 
    receive
        {msg, Value} -> 
            io:fwrite("~p~n",[Value]),
            receiverLoop();
        fin -> 
            io:fwrite("Receiver apagando...~n"),
            finok
    end.

procesoReceiver() ->
    registrar(),
    receiverLoop().

proceso1() ->
    PidReceiver = spawn(bcast2, procesoReceiver, []),
    timer:sleep(1500),
    broadcast("HOLA DESDE PROCESO 1"),
    timer:sleep(3000),
    PidReceiver ! fin,
    server ! {fin,self()}. % mato el server ??
    
proceso2() ->
    PidReceiver = spawn(bcast2, procesoReceiver, []),
    timer:sleep(500),
    broadcast("HOLA DESDE PROCESO 2"),
    timer:sleep(3000),
    PidReceiver ! fin.