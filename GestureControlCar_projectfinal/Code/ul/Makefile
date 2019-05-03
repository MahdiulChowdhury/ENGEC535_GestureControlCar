CC=$(EC535)/arm-linux/bin/arm-linux-gcc -static
PROGS = ul_car_control
LDFLAGS = -lm

$(PROGS): ul_car_control.o
	$(CC) ul_car_control.o $(LDFLAGS) -o $(PROGS)

all: $(PROGS)


clean: 
	rm -f $(PROGS)  *.o *~
