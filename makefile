output: main.o
	gcc main.o -o run.out

main.o: main.c
	gcc -c -lpthread -std=c11 main.c

clean:
	rm *.o run.*