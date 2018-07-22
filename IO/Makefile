.PHONY:all

all:select_server client

select_server:server_select.c
	gcc -o $@ $^
client:client.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm select_server client
