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

_OBJ = ui.o helpers.o candidate.o detector.o grayscale.o main.o app.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/detector/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

ep1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o ep1

pack:
	zip -r felipe_blassioli.zip include/ src/ obj/ Makefile default_template.png relatorio.pdf