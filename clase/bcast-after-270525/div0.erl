-module(div0).
-export([safezero/2, test/0, dummy/0, catchException/0]).

safezero(M, N) ->
    case catch (M/N) of 
        {'EXIT', {badarith, Args}} -> io:format("División por cero ~nTraza ~p ~n", [Args]); % io:format o io:fwrite son iguales
        Res -> Res
    end.

dummy() -> throw(die). % me creo mi propia excepcion llamada die

catchException() ->
    case catch dummy() of 
        die -> io:format("Excepcion die propia~n");
        Res -> Res
    end.

test() ->
    safezero(4,2),
    io:fwrite("ESTOY PROBANDO~n"),
    safezero(12,0),
    %exit(normal),
    catchException().

