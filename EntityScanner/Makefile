CC = g++
CFLAG = -W -g
TARGET = sortuper

$(TARGET) : main.o RawEntry.o EntryManager.o
	$(CC) $(CFLAG) -o $(TARGET) main.o RawEntry.o EntryManager.o

main.o : RawEntry.hpp main.cpp
	$(CC) $(CFLAG) -c main.cpp

RawEntry.o : RawEntry.hpp RawEntry.cpp
	$(CC) $(CFLAG) -c RawEntry.cpp

EntryManager.o : EntryManager.hpp EntryManager.cpp
	$(CC) $(CFLAG) -c EntryManager.cpp

TorAdoptor.o : TorAdoptor.hpp TorAdoptor.cpp
	$(CC) $(CFLAG) -c TorAdoptor.cpp


clean :
	rm *.o sortuper
