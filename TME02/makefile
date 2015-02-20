# TP02 - outils de dev et GPIO
#
# @author Alexandra Hospital <hospital.alex@gmail.com>
# @author Ilyas     Toumlilt <toumlilt.ilyas@gmail.com>
#
# @version 1.1

# en TP :
CROSS_COMPILE ?= bcm2708hardfp-

# home :
#CROSS_COMPILE ?= /home/toumlilt/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-

SRC=src
OBJ=obj
BIN=bin
INC=include
LIB=lib

CFLAGS=-Wall -Wfatal-errors -O2
LDFLAGS=-static -L$(LIB) -lgpio

PROGS=hello_world lab1 lab2 lab3 lab4
EXECS=$(BIN)/hello_world.x $(BIN)/lab1.x $(BIN)/lab2.x $(BIN)/lab3.x $(BIN)/lab4.x

all: directories $(PROGS)

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

hello_world: $(BIN)/hello_world.x #exo1
lab1: $(BIN)/lab1.x               #exo2
lab2: $(BIN)/lab2.x               #exo3
lab3: $(BIN)/lab3.x		  #exo4
lab4: $(BIN)/lab4.x               #exo6

$(BIN)/hello_world.x: $(SRC)/hello_world.c
	$(CROSS_COMPILE)gcc -o $@ $< -I$(INC) -O2 -static

$(BIN)/lab1.x: $(SRC)/lab1.c $(LIB)/libgpio.a
	$(CROSS_COMPILE)gcc -o $@ $^ $(LDFLAGS) -I$(INC)

$(BIN)/lab2.x: $(SRC)/lab2.c $(LIB)/libgpio.a
	$(CROSS_COMPILE)gcc -o $@ $^ $(LDFLAGS) -I$(INC)

$(BIN)/lab3.x: $(SRC)/lab3.c $(LIB)/libgpio.a
	$(CROSS_COMPILE)gcc -o $@ $^ $(LDFLAGS) -I$(INC)

$(BIN)/lab4.x: $(SRC)/lab4.c $(LIB)/libgpio.a
	$(CROSS_COMPILE)gcc -o $@ $^ $(LDFLAGS) -I$(INC)

$(LIB)/libgpio.a: $(OBJ)/gpio_value.o $(OBJ)/gpio_config.o $(OBJ)/gpio_setup.o
	$(CROSS_COMPILE)ar -rcs $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CROSS_COMPILE)gcc -o $@ -c $(CFLAGS) $< -I$(INC)

upload: $(PROGS)
	scp -P5024 $(EXECS) pi@132.227.102.36:~/hospital-toumlilt/

upload-home: $(PROGS)
	scp $(EXECS) pi@$(HOME_IP):~/my_peri/

clean:
	rm -f $(OBJ)/*.o $(BIN)/*.x $(SRC)/*~

distclean: clean
	rm -f $(LIB)/*.a

cleanall:
	rm -rf $(OBJ) $(BIN) $(LIB)

