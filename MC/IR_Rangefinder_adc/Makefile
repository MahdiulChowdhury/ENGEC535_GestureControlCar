CC=$(EC535)/arm-linux/bin/arm-linux-gcc -static
PROGS = ADS1115 
#LDFLAGS = -lm

$(PROGS): ADS1115.o
	$(CC) ADS1115.o -o $(PROGS)

all: $(PROGS)


clean: 
	rm -f $(PROGS)  *.o *~
