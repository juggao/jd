CC=gcc
CFLAGS=-I. -Wall -g -v
DEPS = 
OBJ = calc.o  
OBJISO = iso.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

jd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lm
	
iso: $(OBJISO)
	$(CC) -o $@ $^ $(CFLAGS) -lm
