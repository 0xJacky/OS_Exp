CC=gcc
CFLAGS = -g -Wall -lpthread
OBJS = mainProg.o reader.o writer.o utilities.o
TARGET = mainProg

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

run:
	./$(TARGET)

clean:
	rm  -rf  *.o $(TARGET)
