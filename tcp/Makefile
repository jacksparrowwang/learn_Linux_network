.PHONY:all
	
all:server_tcp client_tcp

server_tcp:server_tcp.c
	gcc -o $@ $^

client_tcp:client_tcp.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -f client_tcp server_tcp
