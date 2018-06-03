.PHONY:all

all:server client

server:Server.c common.c
	gcc $^ -o $@

client:Client.c common.c
	gcc $^ -o $@

.PHONY:clean

clean:
	rm server client
