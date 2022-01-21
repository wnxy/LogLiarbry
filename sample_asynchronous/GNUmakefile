main: main.o SPLog.o
	g++ -std=c++11 main.o SPLog.o -o main -lpthread

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -o main.o

SPLog.o: ../runtime/SPLog.cpp
	g++ -std=c++11 -c ../runtime/SPLog.cpp -o SPLog.o

clean: 
	rm *.o main

run:
	./main