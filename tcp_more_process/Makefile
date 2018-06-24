.PHONY:all

all:server client

server:more_request_server.c
	gcc -o $@ $^

client:client_tcp.c
	gcc -o $@ $^
	
.PHONY:clean

clean:
	rm -f server client
