output: main.o operations.o
	gcc main.o operations.o -o run.out -lpthread

main.o: main.c globals.h operations.h
	gcc -c main.c -std=c11

operations.o: operations.c operations.h globals.h
	gcc -c operations.c -std=c11

clean:
	rm *.o run.*