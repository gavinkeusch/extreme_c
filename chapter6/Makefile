all: example

example: main.o private.o
	gcc main.o private.o -o ex6_3.out

private.o: chapter6_3.c
	gcc -c chapter6_3.c -o private.o

main.o: chapter6_3_example.c
	gcc -c chapter6_3_example.c -o main.o

clean:
	rm *.o *.out