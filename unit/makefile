CC=g++

CFLAGS=-std=gnu++11

LIBS=-lgtest -lpthread -lgtest_main

DEPS= i8080.h

BUILDOBJ= ../build/i8080.o \
		  ../build/io_devices.o \
		  ../build/instructions.o \

TESTOBJ= test_add.o \
	 test_mov.o \
	 test_branch.o \
	 test_call.o \
	 test_inc_dec.o \
	 test_logical.o \
	 test_io_special.o \
	 test_stack.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)

./unittest: $(TESTOBJ)
	$(CC) $(BUILDOBJ) -o $@ $^ $(LIBS)  $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
