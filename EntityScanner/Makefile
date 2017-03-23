CC = g++
CFLAG = -W -g
TARGET = sortuper
OBJECT = main.o RawEntry.o EntryManager.o TorAdoptor.o

all : $(TARGET)

$(TARGET) : $(OBJECT)
	$(CC) $(CFLAG) -o $@ $^

clean :
	rm *.o sortuper
