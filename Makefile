CC = gcc
CFLAGS = -Wall -fPIC -Iinclude
LDFLAGS = -shared
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = libCExpress.dylib

PREFIX ?= /usr/local

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

install: $(TARGET)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/CExpress
	cp $(TARGET) $(PREFIX)/lib/
	cp include/CExpress/*.h $(PREFIX)/include/CExpress/

uninstall:
	rm -f $(PREFIX)/lib/$(TARGET)
	rm -rf $(PREFIX)/include/CExpress
	rm -f libCExpress.dylib