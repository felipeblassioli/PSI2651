# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# http://mrbook.org/blog/tutorials/make/
CC=g++
CFLAGS=-Wall

ODIR=obj
SRCDIR=src

_OBJ = main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

CFLAGS += `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs opencv`

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

ep1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o ep1