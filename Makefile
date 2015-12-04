LDFLAGS=-lm -lGL -lGLU -lglut

all: gravity

gravity: gravity.o math3d.o
	$(CC) -o gravity $(LDFLAGS) $^

gravity.o: gravity.c
	$(CC) -c -o $@ $^

math3d.o: math3d.c
	$(CC) -c -o $@ $<
