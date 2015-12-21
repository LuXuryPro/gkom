LDFLAGS=-lm -lGL -lGLU -lglut -lGLEW -lSOIL
CFLAGS=-g

all: gravity tags

gravity: gravity.o math3d.o camera.o gui.o shader.o file_util.o skybox.o mesh.o object.o
	$(CC) -o gravity $(LDFLAGS) $^
	ctags -R --sort=yes .

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
