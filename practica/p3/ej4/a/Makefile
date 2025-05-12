CFLAGS += -pthread -O99

ALL_SOURCES = $(wildcard *.c)
MAINS = $(filter-out $(EXCLUDE), $(ALL_SOURCES))
PROGS = $(patsubst %.c,%,$(MAINS))

all: rw

rw: rw.c rwlock.c
	$(CC) $(CFLAGS) $^ -o $@


