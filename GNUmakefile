main: main.o SWLog.o
	g++ main.o SWLog.o -o main -lpthread

main.o: sample/main.cpp
	g++ -c sample/main.cpp -o main.o

SWLog.o: runtime/SWLog.cpp
	g++ -c runtime/SWLog.cpp -o SWLog.o

clean: 
	rm *.o main

run:
	./main