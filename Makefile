LDFLAGS=-lm -lGL -lGLU -lglut -lGLEW
CFLAGS=-g

all: gravity

gravity: gravity.o math3d.o camera.o gui.o shader.o file_util.o
	$(CC) -o gravity $(LDFLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
