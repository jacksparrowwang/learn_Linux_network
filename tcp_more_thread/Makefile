.PHONY:all

all:server client

server:more_thread_server.c
	gcc -o $@ $^ -lpthread

client:client_tcp.c
	gcc -o $@ $^
	
.PHONY:clean

clean:
	rm -f server client
