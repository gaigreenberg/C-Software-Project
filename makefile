FLAGS 	= -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS 	= -lm
SOURCE	= main.c Matrix.c PowerIteration.c Division.c Modularity.c CheckingAlgorithems.c
OBJECTS	= main.o Matrix.o PowerIteration.o Division.o Modularity.o CheckingAlgorithems.o
HEADER	= Matrix.h PowerIterartion.h Division.h Modularity.h CheckingAlgorithems.h

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

CheckingAlgorithems.o : CheckingAlgorithems.c
	gcc $(FLAGS) -c CheckingAlgorithems.c



