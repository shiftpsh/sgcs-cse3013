OBJECTS = ordered_list.o tetris.o
SRC = ordered_list.c tetris.c

CC = gcc

TARGET = main

$(TARGET): $(OBJECTS) ordered_list.h tetris.h
	$(CC) -o $(TARGET) $(OBJECTS) -lncurses

clean:
	rm $(OBJECTS) $(TARGET)

