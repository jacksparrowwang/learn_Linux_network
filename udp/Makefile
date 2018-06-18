.PHONY:all

all:client_udp server_udp

client_udp: client_udp.c 
	gcc -o $@ $^ -static

server_udp: server_udp.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -f client_udp server_udp
