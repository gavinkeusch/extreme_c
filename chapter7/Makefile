all: ex7_1 ex7_2

ex7_1: ex7_1.o engine.o car.o
	gcc ex7_1.o car.o engine.o -o ex7_1.out

engine.o: chapter7_1_engine.c
	gcc -c chapter7_1_engine.c -o engine.o

car.o: chapter7_1_car.c
	gcc -c chapter7_1_car.c -o car.o

ex7_1.o: chapter7_1_example.c
	gcc -c chapter7_1_example.c -o ex7_1.o

ex7_2: ex7_2.o player.o gun.o
	gcc gun.o player.o ex7_2.o -o ex7_2.out

player.o: chapter7_2_player.c
	gcc -c chapter7_2_player.c -o player.o

gun.o: chapter7_2_gun.c
	gcc -c chapter7_2_gun.c -o gun.o

ex7_2.o: chapter7_2_example.c
	gcc -c chapter7_2_example.c -o ex7_2.o

clean:
	rm *.o *.out