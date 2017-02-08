sortuper: main.o RawEntry.o
	g++ -g -o sortuper main.o RawEntry.o

main.o : RawEntry.hpp main.cpp
	g++ -g -c main.cpp

RawEntry.o : RawEntry.hpp RawEntry.cpp
	g++ -g -c RawEntry.cpp
