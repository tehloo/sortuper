sortuper: main.o RawEntry.o EntryManager.o
	g++ -g -o sortuper main.o RawEntry.o EntryManager.o

main.o : RawEntry.hpp main.cpp
	g++ -g -c main.cpp

RawEntry.o : RawEntry.hpp RawEntry.cpp
	g++ -g -c RawEntry.cpp

EntryManager.o : EntryManager.hpp EntryManager.cpp
	g++ -g -c EntryManager.cpp
