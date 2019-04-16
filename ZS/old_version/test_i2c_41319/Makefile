CC=$(EC535)/arm-linux/bin/arm-linux-gcc -static
PROGS = test_i2c41319
LDFLAGS = -lm

$(PROGS): test_i2c41319.o
	$(CC) test_i2c41319.o $(LDFLAGS) -o $(PROGS)

all: $(PROGS)


clean: 
	rm -f $(PROGS)  *.o *~
