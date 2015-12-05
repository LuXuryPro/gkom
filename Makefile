LDFLAGS=-lm -lGL -lGLU -lglut

all: gravity

gravity: gravity.o math3d.o camera.o gui.o
	$(CC) -o gravity $(LDFLAGS) $^

%.o: %.c
	$(CC) -c -o $@ $^
