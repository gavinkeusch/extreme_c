all: ex18_4

ex18_4: shared_mem.o shared_int32.o shared_mutex.o shared_cond.o main.o
	gcc shared_mem.o shared_int32.o shared_mutex.o shared_cond.o main.o -lpthread -lrt -o ext18_4.out

shared_mem.o: chapter18_4_shared_mem.c
	gcc -c chapter18_4_shared_mem.c -o shared_mem.o

shared_int32.o: chapter18_4_shared_int32.c
	gcc -c chapter18_4_shared_int32.c -o shared_int32.o

shared_mutex.o: chapter18_4_shared_mutex.c
	gcc -c chapter18_4_shared_mutex.c -o shared_mutex.o

shared_cond.o:chapter18_4_shared_cond.c
	gcc -c chapter18_4_shared_cond.c -o shared_cond.o

main.o: chapter18_4_main.c
	gcc -c chapter18_4_main.c -o main.o

clean:
	rm *.o *.out