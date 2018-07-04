.PHONY:all

all:server client

server:gobang_server.c
	g++ -o $@ $^ -lpthread -g

client:gobang_client.c
	g++ -o $@ $^ -g

.PHONY:clean

clean:
	rm -f server client
