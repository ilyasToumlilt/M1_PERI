# TP04 - Pilotage d'un LCD en mode user
#
# @author Alexandra Hospital <hospital.alex@gmail.com>
# @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
#
# @version 1.0

SRC=src
OBJ=obj
BIN=bin
INC=include
LIB=lib

CFLAGS=-Wall -Wfatal-errors -O2
LDFLAGS=-static -L$(LIB) -lgpio

PROGS=lcdRpi
EXECS=$(BIN)/lcdRpi.x

#fac :
CROSSDIR        = /users/enseig/franck/peri
KERNELDIR       = $(CROSSDIR)/linux-rpi-3.18.y
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-
#home :
#CROSS_COMPILE    = /tmp/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-


all: directories $(PROGS)

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

lcdRpi: $(BIN)/lcdRpi.x

$(BIN)/lcdRpi.x: $(SRC)/lcdRpi.c
	$(CROSS_COMPILE)gcc -o $@ $< -I$(INC) -O2 -static

upload: 
	scp -P5024 $(EXECS) pi@132.227.102.36:~/hospital-toumlilt/

upload-home: 
	scp $(EXECS) pi@$(HOME_IP):~/my_peri/

clean:
	rm -f $(OBJ)/*.o $(BIN)/*.x $(SRC)/*~

distclean: clean
	rm -f $(LIB)/*.a

cleanall:
	rm -rf $(OBJ) $(BIN) $(LIB)
