all: main.o List.o utils.o
	gcc -o main main.o utils.o List.o
	rm -rf *.o 
	./main

main.o: main.c commons/utils.h commons/List.h
	gcc -c main.c 

utils.o: commons/utils.c commons/utils.h commons/List.h
	gcc -c commons/utils.c

List.o: commons/List.c commons/List.h
	gcc -c commons/List.c
	
