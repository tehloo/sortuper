CC = g++
CPPFLAGS = -W -g
TARGET = sortuper
OBJECT = main.o RawEntry.o EntryManager.o TorAdoptor.o

all : $(TARGET)

$(TARGET) : $(OBJECT)
	$(CC) $(CPPFLAGS) -o $@ $^

clean :
	rm *.o sortuper
