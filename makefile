CC=g++

CFLAGS=-std=gnu++11

LIBS=-lsfml-graphics -lsfml-window -lsfml-system

DEPS= i8080.h

OBJ= main.o i8080.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)

./i8080emu: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)  $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
