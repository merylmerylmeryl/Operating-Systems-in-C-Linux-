find: proj01.o
	g++ -o find proj01.o

proj01.o: proj01.cpp
	g++ -Wall -c proj01.cpp
clean:
	rm -f find proj01.o
