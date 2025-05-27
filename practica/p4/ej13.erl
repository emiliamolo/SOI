-module(ej13).
-export([init/0, receiveChild/2, suma/3]).

init() ->
    spawn(ej13, suma, [1, 8, self()]),
    receive
        {1, Val} -> io:fwrite("La suma es ~p~n", [Val])
    end.

receiveChild(Id, N) ->
    if Id =< N ->
        receive
            {Id, Val} -> Val
        end;
    true ->
        0
    end.

suma(Id, N, Pid) ->
    Izq = 2*Id,
    Der = Izq+1,
    if (Izq =< N) ->
        spawn(ej13, suma, [Izq, N, self()]);
    true -> 
        nizq
    end,
    if (Der =< N) ->
        spawn(ej13, suma, [Der, N, self()]);
    true ->
        nder
    end,
    
    Val = receiveChild(Izq, N) + receiveChild(Der, N) + Id,
    Pid ! {Id, Val}.
