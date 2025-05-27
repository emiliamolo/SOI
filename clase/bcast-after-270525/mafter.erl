-module(mafter).
-export([sleep/1, empty_mailbox/0]).

sleep(Millis) ->
    receive
    after
        Millis -> 
            sleepok
    end.

empty_mailbox() -> 
    receive
        _ -> emptyok
    after 
        0 -> emptyok
    end.

% TODO: priority_process/0