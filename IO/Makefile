.PHONY:all

all:server_poll client

server_poll:poll_server.c
	gcc -o $@ $^ -std=c99
client:client.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm server_poll client
