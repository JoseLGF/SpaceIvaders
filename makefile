# Compiler to use
CC := g++

# Folders
SRCDIR    := src
BUILDDIR  := build
TARGETDIR := bin

# Targets
EXECUTABLE := emu8080
TARGET     := $(TARGETDIR)/$(EXECUTABLE)

# Final Path for the target executable.
INSTALLBINDIR := "$$HOME/bin"

# Code Lists
SRCEXT  := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Folder Lists
# Note: Intentionally excludes the root of the include folder so the lists are clean
INCDIRS   := $(shell find include/* -name '*.h' -exec dirname {} \; | sort | uniq)
INCLIST   := $(patsubst include/%,-I include/%,$(INCDIRS))
BUILDLIST := $(patsubst include/%,$(BUILDDIR)/%,$(INCDIRS))

# Shared Compiler Flags
CFLAGS := -g
OFLAGS  = $(LIBDIRS) -lm -lpthread -lncurses \
		  -lsfml-graphics -lsfml-window -lsfml-system

INC := -I include #$(INCLIST) -I /usr/local/include
LIB := -L /usr/local/lib

CFLAGS += -std=c++11

# rule for the main program
$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "  Linking $(TARGET)"; $(CC) $^ -o $(TARGET) $(LIB) $(OFLAGS) $(CFLAGS)

# rule for the object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $<..."; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

install: $(TARGET)
	install $(TARGET) $(INSTALLBINDIR)

clean:
	@echo "Cleaning $(TARGET)..."; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean


# Old makefile
# CC=g++

# CFLAGS=-std=gnu++11

# LIBS=-lsfml-graphics -lsfml-window -lsfml-system

# DEPS= i8080.h

# OBJ= main.o i8080.o \
# 	 impl_instructions_branch.o \
# 	 impl_instructions_other.o \
# 	 impl_instructions_stack.o \
# 	 impl_instructions_move.o \
# 	 impl_instructions_call.o \
# 	 impl_instructions_inc_dec.o \
# 	 impl_instructions_add_sub.o \
# 	 impl_instructions_logical.o \
# 	 impl_instructions_io_special.o \

# %.o: %.cpp $(DEPS)
# 	$(CC) -c -o $@ $<  $(CFLAGS)

# ./i8080emu: $(OBJ)
# 	$(CC) -o $@ $^ $(LIBS)  $(CFLAGS)

# .PHONY: clean

# clean:
# 	rm -f *.o
# 	rm i8080emu
