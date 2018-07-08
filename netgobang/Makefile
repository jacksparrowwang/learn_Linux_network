
ROOT=$(shell pwd)
INCLUDE=-I$(ROOT)/include
LIB=-L$(ROOT)/lib
LIB_NAME=-lmysqlclient

.PHONY:all

all:server client insert select

insert:insert.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)

select:select.c
	gcc -o $@ $^

server:gobang_server.c
	gcc -o $@ $^ -lpthread

client:gobang_client.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -f server client select insert
