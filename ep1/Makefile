# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# http://mrbook.org/blog/tutorials/make/

CC=g++
CFLAGS=-Wall -I$(INCLUDE_DIR)
CFLAGS += `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs opencv`

# Directories
INCLUDE_DIR=include
ODIR=obj
SRCDIR=src

_OBJ = main.o app.o helpers.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

ep1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o ep1