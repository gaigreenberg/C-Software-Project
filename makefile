FLAGS 	= -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS 	= -lm
SOURCE	= main.c spmat.c pIter.c division.c Bmat.c
OBJECTS	= main.o spmat.o pIter.o division.o Bmat.o
HEADER	= spmat.h pIter.h division.h Bmat.h

all: $(OBJECTS)
	gcc $(OBJECTS) -o cluster $(LIBS)

clean:
	rm -rf $(OBJECTS) cluster
	

main.o: main.c
	gcc $(FLAGS) -c main.c

spmat.o: spmat.c
	gcc $(FLAGS) -c spmat.c

pIter.o: pIter.c
	gcc $(FLAGS) -c pIter.c

division.o: division.c
	gcc $(FLAGS) -c division.c

Bmat.o: Bmat.c
	gcc $(FLAGS) -c Bmat.c





