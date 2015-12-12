LDFLAGS=-lm -lGL -lGLU -lglut -lGLEW -lSOIL
CFLAGS=-g

all: gravity

gravity: gravity.o math3d.o camera.o gui.o shader.o file_util.o skybox.o mesh.o
	$(CC) -o gravity $(LDFLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
