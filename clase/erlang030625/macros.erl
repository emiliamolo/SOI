-module(macros).
-export([test/0]).

-define(DEBUG, debug).
-ifdef(DEBUG).
    -define(DBG(Str, Args), io:format(Str, Args)).
-else.
    -define(DBG(Str, Args), ok).
-endif.

test() ->
    ?DBG("Hola estoy probando, ~p ~n", [testing]).