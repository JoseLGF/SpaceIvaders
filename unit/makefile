CC=g++

CFLAGS=-std=gnu++11

LIBS=-lgtest -lpthread -lgtest_main

DEPS= i8080.h

BUILDOBJ= ../build/i8080.o \
		  ../build/impl_instructions_add_sub.o \
		  ../build/impl_instructions_branch.o \
		  ../build/impl_instructions_call.o \
		  ../build/impl_instructions_inc_dec.o \
		  ../build/impl_instructions_io_special.o \
		  ../build/impl_instructions_logical.o \
		  ../build/impl_instructions_move.o \
		  ../build/impl_instructions_other.o \
		  ../build/impl_instructions_stack.o \

TESTOBJ= test_add.o \
	 test_mov.o \
	 test_branch.o \
	 test_call.o \
	 test_inc_dec.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)

./unittest: $(TESTOBJ)
	$(CC) $(BUILDOBJ) -o $@ $^ $(LIBS)  $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
