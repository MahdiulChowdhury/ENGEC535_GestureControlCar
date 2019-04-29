CC=$(EC535)/arm-linux/bin/arm-linux-gcc -static
PROGS = test_i2c41619
#LDFLAGS = -lm

$(PROGS): test_i2c41619.o
	$(CC) test_i2c41619.o -o $(PROGS)

all: $(PROGS)


clean: 
	rm -f $(PROGS)  *.o *~
