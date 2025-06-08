-module(servRecords).
-include("servRecords.hrl").
%%%%%%
%% Pequeño ejercicio de clase
%% el servidor aceptara 4 tipos de pedidos/mensajes:
%% + nuevoId(Nombre, PidResp) -> Se generará un nuevo identificar para `Nombre` y se responderá al cliente.
%% + buscarId(Id, PidResp) -> Se responde a `PidResp` el nombre asociado a `Id`.
%% + verLista(PidResp) -> Se envía a `Pidresp` la lista de pares (Id,Nombre).
%% + finalizar(PidResp) -> Se finaliza el servicio y se responde con un `ok`.
%%

%% Creación y eliminación del servicio
-export([iniciar/0, finalizar/0]).

%% Servidor
-export([serverinit/1]).

%% Librería de Acceso
-export([nuevoNombre/1, quienEs/1, listaDeIds/0]).

%% Funciones de testing
-export([test/0,client/0]).

% Iniciar crea el proceso servidor, y devuelve el PId.
iniciar() -> 
  Pid = spawn(?MODULE, serverinit, [self()]),
  register(server, Pid).

%%%%%%%%%%%%%% Librería de Acceso
%% Dado un nombre y un servidor le pide que cree un identificador único.
nuevoNombre(Nombre) ->
  server ! #nuevoId{nombre = Nombre, cid = self()},
  receive
    {ok, Id} -> io:format("Se asoció el id ~p al nombre ~p ~n", [Id, Nombre]);
    _ -> error
  end.

%% Función que recupera el nombre desde un Id
quienEs(Id) ->
  server ! #buscarId{nid = Id, cid = self()},
  receive
    {ok, Nombre} -> io:format("El nombre asociado al id es ~p ~n", [Nombre]);
    _ -> error
  end.

%% Pedimos la lista completa de nombres e identificadores.
listaDeIds() ->
  server ! #verLista{cid = self()},
  receive
    {ok, Lista} -> io:format("Lista en el servidor ~p ~n", [Lista]);
    _ -> error
  end.

% Ya implementada :D!
finalizar() ->
  server ! #finalizar{cid = self()},
  receive
    fin -> io:format("el servidor de apago~n", []);
    _ -> error
  end.

%%%%%%%%%%% Servidor
%% Función de servidor de nombres.
serverinit(PInit) ->
  %% Comenzamos con un mapa nuevo, y un contador en 0.
  servnombres(maps:new(), 0),
  PInit ! ok.


servnombres(Map, N) ->
  receive
      %% Llega una petición para crear un Id para nombre
      #nuevoId{nombre = Nombre, cid = CId} -> 
        Map2 = maps:put(N, Nombre, Map),
        CId ! {ok, N},
        servnombres(Map2, N+1);
      %% Llega una petición para saber el nombre de tal Id
      #buscarId{nid = NId, cid = CId} -> 
        case maps:find(NId, Map) of
          {ok, Nombre} -> 
            CId ! {ok, Nombre}; % no quiero la clave ???
            error -> CId ! error
        end,
        servnombres(Map, N);
      %% Entrega la lista completa de Ids con Nombres.
      #verLista{cid = CId} -> 
        CId ! {ok, maps:to_list(Map)},
        servnombres(Map, N);
      %% Cerramos el servidor.
      #finalizar{cid = CId} -> CId ! fin;
      _ -> 
        io:format("mensaje incorrecto~n"),
        servnombres(Map, N)
  end.

client() ->
  nuevoNombre(pepa),
  listaDeIds(),
  nuevoNombre(pepe),
  listaDeIds(),
  quienEs(0),
  
  quienEs(1),
  finalizar(),
  ok.

test() ->
  servid_template:iniciar(),
  spawn(?MODULE, client, []),
  ok.
