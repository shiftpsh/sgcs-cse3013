OBJECTS = main.o Str.o
SRC = main.cpp Str.cpp

CC = g++
CPPFLAGS = -g -c

TARGET = main

$(TARGET): $(OBJECTS) Str.h
	$(CC) -o $(TARGET) $(OBJECTS)

clean: 
	rm $(OBJECTS) $(TARGET)
	
