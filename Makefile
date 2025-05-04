CC = gcc

CFLAGS = -Wall -g $(shell pkg-config --cflags gtk4) -I/usr/include/json-c -Iinc 

LDFLAGS = $(shell pkg-config --libs gtk4) -ljson-c -luuid -lmosquitto 

SRC = $(wildcard src/*.c) 

OBJ = $(SRC:.c=.o)

BIN = hapa-mqtt-client

all: $(BIN)

$(BIN) : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f src/*.o $(BIN)

rebuild: clean all

