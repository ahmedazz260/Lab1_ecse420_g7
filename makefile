CC = gcc
CFLAGS  = -I. -lm

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: test_equality

# To create the executable file count we need the object files
#
test_equality:  test_equality.o lodepng.o
	$(CC) -o test_equality test_equality.o lodepng.o $(CFLAGS)

rectify:  test_rectification.o lodepng.o
	$(CC) -o rectify test_rectification.o lodepng.o $(CFLAGS)

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) count *.o *~
