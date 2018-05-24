.PHONY:all 

all: queue_receive queue_send

queue_send:queue_send.c common.c
	gcc $^ -o $@

queue_receive:queue_receive.c common.c
	gcc $^ -o $@

.PHONY:clean

clean:
	rm queue_receive queue_send
