CC=gcc
CFLAGS=-I. -Wall -g -v
DEPS = 
OBJ = calc.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

jd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lm
