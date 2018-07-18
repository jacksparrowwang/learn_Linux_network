
ROOT=$(shell pwd)
INCLUDE=-I$(ROOT)/include/
LIB=-L$(ROOT)/lib/
LIB_NAME=-lmysqlclient

.PHONY:all

all:server client 

# insert:db.c
# 	gcc -o $@ $^ $(LIB)  $(LIB_NAME) $(INCLUDE)

server:gobang_server.c db.c
	gcc -o $@ $^ -lpthread $(LIB)  $(LIB_NAME) $(INCLUDE)

client:gobang_client.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -f server client select insert
