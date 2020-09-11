FLAGS 	= -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS 	= -lm
SOURCE	= main.c Matrix.c PowerIteration.c division.c Modularity.c
OBJECTS	= main.o Matrix.o PowerIteration.o division.o Modularity.o
HEADER	= Matrix.h PowerIterartion.h division.h Modularity.h

all: $(OBJECTS)
	gcc $(OBJECTS) -o cluster $(LIBS)

clean:
	rm -rf $(OBJECTS) cluster
	

main.o: main.c
	gcc $(FLAGS) -c main.c

Matrix.o: Matrix.c
	gcc $(FLAGS) -c Matrix.c

PowerIteration.o: PowerIteration.c
	gcc $(FLAGS) -c PowerIteration.c

division.o: division.c
	gcc $(FLAGS) -c division.c

Modularity.o: Modularity.c
	gcc $(FLAGS) -c Modularity.c





