CC = g++
CPPFLAGS = -W -g -std=c++11
TARGET = sortuper
OBJECT = main.o RawEntry.o EntryManager.o TorAdoptor.o TorManager.o

all : $(TARGET)

$(TARGET) : $(OBJECT)
	$(CC) $(CPPFLAGS) -o $@ $^

clean :
	rm *.o sortuper
