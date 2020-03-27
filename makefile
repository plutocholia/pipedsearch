OBJECTS = main.o PipeSearch.o LoadBalancer.o Command.o Utills.o Worker.o
CC = g++ -std=c++11

all: main
	make clean

main: $(OBJECTS)
	$(CC) -o main $(OBJECTS)

main.o: main.cpp
	$(CC) -c main.cpp

PipeSearch.o: PipeSearch.cpp PipeSearch.h
	$(CC) -c PipeSearch.cpp

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h
	$(CC) -c LoadBalancer.cpp

Command.o: Command.cpp Command.h
	$(CC) -c Command.cpp

Utills.o: Utills.cpp Utills.h
	$(CC) -c Utills.cpp

Worker.o: Worker.cpp Worker.h
	$(CC) -c Worker.cpp

clean:
	rm *.o