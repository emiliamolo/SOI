-module(ej1).
-export([init/0]).

match_test () ->
    {A,B} = {5,4},  % a) No hay problema
    %{C,C} = {5,4}, % a) No es válido porque se hace un match donde se asigna una misma variable a valores distintos
    {B,A} = {4,5},  % a) El match es válido porque se hace el mismo match (en orden inverso en {}) en la cláusula anterior
    {D,D} = {5,5}.  % a) Es válido porque se asigna el mismo valor a la misma variable (dos veces).

string_test () -> [
    helloworld == 'helloworld',  % c) true, el nombre de un átomo se puede escribir en comillas simples. En este caso, resulta el mismo átomo.
    "helloworld" < 'helloworld', % c) false, al ser de distintos tipos se sigue el orden de la documentación. En este caso, atom < list (string es list de char), y por lo tanto al revés vale false.
    helloworld == "helloworld",  % c) false, porque un átomo no es igual a una cadena
    [$h,$e,$l,$l,$o,$w,$o,$r,$l,$d] == "helloworld", % c) true, porque una cadena en Erlang es igual a un arreglo de char, comparando por código ASCII. $[letra] devuelve su ASCII.
    [104,101,108,108,111,119,111,114,108,100] < {104,101,108,108,111,119,111,114,108,100}, % c) false, al ser de distintos tipos se sigue el orden de la documentación. En este caso, tuple < list, y por lo tanto al revés vale false.
    [104,101,108,108,111,119,111,114,108,100] > 1, % c) true, al ser de distintos tipos se sigue el orden de la documentación. En este caso, number < list, y por lo tanto vale true.
    [104,101,108,108,111,119,111,114,108,100] == "helloworld" % c) Nuevamente, una string es un arreglo de char. La igualdad vale porque el arreglo contiene los códigos ASCII coreresp. a las letras de la string.
].

tuple_test (P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido ({persona, {nombre, _}, {apellido, A}}) -> A.
nombre ({persona, {nombre, N}, {apellido, _}}) -> N.


% filtrar_por_apellido([], _) -> [];
% filtrar_por_apellido([P|Ps], Apellido) -> 
%     ApellidoP = apellido(P),
%     if ApellidoP == Apellido ->
%         [nombre(P)|filtrar_por_apellido(Ps, Apellido)];
%     true ->
%         filtrar_por_apellido(Ps, Apellido)
%     end.
filtrar_por_apellido(Personas, Apellido) -> [nombre(Per) || Per <- Personas, Apellido == apellido(Per)].

init () ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    string_test(),
    Garcias = filtrar_por_apellido([P4, P3, P2, P1], "Garcia").