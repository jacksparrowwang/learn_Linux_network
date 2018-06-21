# a.out:producer_consumer_thread.c
# 	gcc -o $@ $^ -lpthread

# .PHONY:clean

# clean:
# 	rm -f a.out
#


a.out:POSIX_produce_consume.c
	gcc  -o $@ $^ -lpthread

.PHONY:clean

clean:
	rm -f a.out
