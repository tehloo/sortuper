sortuper: main.o RawEntry.o
	g++ -o sortuper main.o RawEntry.o

main.o : RawEntry.hpp main.cpp
	g++ -c main.cpp

RawEntry.o : RawEntry.hpp RawEntry.cpp
	g++ -c RawEntry.cpp
