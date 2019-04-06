cmd_/ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.o := arm-linux-gcc -Wp,-MD,/ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/.i2c.o.d  -nostdinc -isystem /ad/eng/courses/ec/ec535/gumstix/oe/cross/bin/../lib/gcc/arm-angstrom-linux-gnueabi/4.1.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=xscale -Wa,-mcpu=xscale  -msoft-float -Uarm -fno-omit-frame-pointer -fno-optimize-sibling-calls  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign   -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(i2c)"  -D"KBUILD_MODNAME=KBUILD_STR(i2c)" -c -o /ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.o /ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.c

deps_/ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.o := \
  /ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.c \
    $(wildcard include/config/hard/i2c.h) \
    $(wildcard include/config/sys/i2c/speed.h) \
    $(wildcard include/config/cpu/monahans.h) \
    $(wildcard include/config/sys/i2c/init/board.h) \
    $(wildcard include/config/sys/eeprom/page/write/bits.h) \

/ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.o: $(deps_/ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.o)

$(deps_/ad/eng/users/m/a/mahdiulc/Desktop/ENGEC535_GestureControlCar/MC/pxa/i2c.o):
