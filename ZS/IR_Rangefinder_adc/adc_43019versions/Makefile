CC=$(EC535)/arm-linux/bin/arm-linux-gcc -static
PROGS = ADS1115_v43019 
#LDFLAGS = -lm

$(PROGS): ADS1115_v43019.o
	$(CC) ADS1115_v43019.o -o $(PROGS)

all: $(PROGS)


clean: 
	rm -f $(PROGS)  *.o *~
