all: shell.o
	gcc -o shell.out shell.o

shell.o: shell_functions.o
	gcc -c shell.c
	
shell_functions.o:
	gcc -c shell_functions.c

run:
	./shell.out
	
clean:
	rm *.o