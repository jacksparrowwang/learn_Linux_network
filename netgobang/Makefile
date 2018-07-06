.PHONY:all

all:server client

server:gobang_server.c
	g++ -o $@ $^ -lpthread

client:gobang_client.c
	g++ -o $@ $^

.PHONY:clean

clean:
	rm -f server client
