-module(mafter).
-export([sleep/1, empty_mailbox/0]).

sleep(Millis) ->
    receive
    after
        Millis -> 
            sleepok
    end.
% if no matching message has arrived within Millis milliseconds, then sleepok is evaluated instead
% erlang corre en una maquina virtual (beam)
% los procesos que crea erlang, los crea esa maquina virtual y lo hace de forma mas liviana
% es mas de alto nivel, no podemos controlar ciertas cosas

empty_mailbox() -> 
    receive
        _Msg -> empty_mailbox() % siempre hace primero el receive, y lo recorre en orden con los match
    after 
        0 -> emptyok
    end.


% Los procesos en erlang se comunican con identificadores de procesos, distintos a los PIDS
% Tienen la forma <a.b.c>: a -> computadora b -> ?? c -> ??
% Le podemos poner un nombre a un proceso con un atomo
% Podemos ver los procesos registrados por erlang con registered()
% En computacion distribuida es importante que se pueda caer un proceso sin romper todo ??
% Regitramos un proceso con register(name, Pid) o borrar : unregister(Name)
% whereis(Name) -> devuelve el identificador asociado al pid

% TODO: priority_process/0
% erlang ya tiene un sistema de prioridad, que le daria mas prioridad a un proceso que a otro para ejecutarse por mas tiempo?