OBJECTS = ordered_list.o decision_tree.o queue.o tetris.o
SRC = ordered_list.c decision_tree.c queue.c tetris.c

CC = gcc
CFLAGS = -g -ggdb

TARGET = main

$(TARGET): $(OBJECTS) ordered_list.h decision_tree.h queue.h tetris.h
	$(CC) -o $(TARGET) $(OBJECTS) -lncurses

clean:
	rm $(OBJECTS) $(TARGET)

