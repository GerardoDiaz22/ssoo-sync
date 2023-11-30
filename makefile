output: main.o
	gcc main.o -o run.out

main.o: main.c
	gcc -c main.c

clean:
	rm *.o run.*