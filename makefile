CC=gcc
CFLAGS=-I. -lm
DEPS = test_equality.h
OBJ = test_equality.o lodepng.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test_equality: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
