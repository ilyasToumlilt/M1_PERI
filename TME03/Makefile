# TP03 - Premier pilote
#
# @author Alexandra Hospital <hospital.alex@gmail.com>
# @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
#
# @version 1.0

SRC=src

#fac :
CROSSDIR        = /users/enseig/franck/peri
KERNELDIR       = $(CROSSDIR)/linux-rpi-3.18.y
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-
#home :
#CROSSDIR      = /home/toumlilt/raspberrypi
#KERNELDIR     = $(CROSSDIR)/linux
#CROSS_COMPILE = $(CROSSDIR)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-
#Salle SAR :
#CROSSDIR      = /tmp/raspberrypi
#KERNELDIR     = $(CROSSDIR)/linux
#CROSS_COMPILE = $(CROSSDIR)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-

EXECS=$(SRC)/module.ko $(SRC)/module_param.ko $(SRC)/ledbtn_lite.ko $(SRC)/ledbtn.ko

all: module

obj-m += $(SRC)/module.o
obj-m += $(SRC)/module_param.o
obj-m += $(SRC)/ledbtn_lite.o
obj-m += $(SRC)/ledbtn.o


module:
	make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules

upload: 
	scp -P5024 $(EXECS) pi@132.227.102.36:~/hospital-toumlilt/

upload-home: 
	scp $(EXECS) pi@$(HOME_IP):~/my_peri/

clean:
	make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean

cleanall:
	rm -rf $(OBJ) $(BIN) $(LIB)

