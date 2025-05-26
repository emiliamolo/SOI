-module(ej13).
-export([].).

init() ->
    spawn(ej13, suma, [0,6, -1]).


% FIXME: Terminar!!
suma(Id, N, Pid) ->
    Izq = 2*Id,
    Der = Izq +1,
    if (Izq =< N) ->
        spawn(ej13, suma, [Izq, N, self()])
        end,
    if (Der =< N) ->
        spawn(ej13, suma, [Der, N, self()])
        end,
    
    if (Izq =< N)
        receive 
            {N, Id} = 
        end,
    if (Der =< N)
        receive 
            {}
            
        end,
        end;
