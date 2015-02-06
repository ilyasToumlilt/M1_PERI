# TP01 - Communication inter processus par tubes
#
# @author Alexandra Hospital <hospital.alex@gmail.com>
# @author Ilyas Toumlilt <toumlilt.ilyas@gmail.com>
#
# @version 1.1
# @package toumlilt/M1/PERI

CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: directories server fake

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

# regles generales :
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $< -I$(INC)

$(BIN)/% : $(OBJ)/%.o
	$(CC) -o $@ $< -lpthread
#fin regles generales

fake: $(BIN)/fake server
	$(BIN)/fake

$(BIN)/server: $(SRC)/server.py
	python $< &
server: $(BIN)/server

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~

.PHONY: all clean cleanall

# auto-generated makefile from ilyas's automake
