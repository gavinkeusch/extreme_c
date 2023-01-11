all: ex8_4

ex8_4: animal.o cat.o duck.o main.o
	gcc animal.o cat.o duck.o main.o -o ex8_4.out

debug: animal.o cat.o duck.o debug_main.o
	gcc animal.o cat.o duck.o debug_main.o -o ex8_4_debug.out

animal.o: chapter8_4_animal.c
	gcc -c chapter8_4_animal.c -o animal.o

cat.o: chapter8_4_cat.c
	gcc -c chapter8_4_cat.c -o cat.o

duck.o: chapter8_4_duck.c
	gcc -c chapter8_4_duck.c -o duck.o

main.o: chapter8_4_main.c
	gcc -c chapter8_4_main.c -o main.o

debug_main.o: chapter8_4_main.c
	gcc -g -c chapter8_4_main.c -o debug_main.o

clean:
	rm *.o *.out