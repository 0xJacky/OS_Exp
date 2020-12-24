CC=gcc
CFLAGS = -g -Wall -lpthread
OBJS = mainProg.o Consumer.o Producer.o ProducerConsumerUtilities.o
TARGET = mainProg

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

run:
	./$(TARGET)

clean:
	rm  -rf  *.o $(TARGET)
