CC=g++

CFLAGS=-std=gnu++11

LIBS=-lsfml-graphics -lsfml-window -lsfml-system

DEPS= i8080.h

OBJ= main.o i8080.o \
	 impl_instructions_branch.o \
	 impl_instructions_other.o \
	 impl_instructions_stack.o \
	 impl_instructions_move.o \
	 impl_instructions_call.o \
	 impl_instructions_inc_dec.o \
	 impl_instructions_add_sub.o \

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)

./i8080emu: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)  $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
