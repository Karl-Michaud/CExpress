# Detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS settings
    CC = gcc
    SHARED_FLAG = -dynamiclib
    INSTALL_NAME_FLAG = -Wl,-install_name,/usr/local/lib/libCExpress.dylib
    LIB_EXT = dylib
else ifeq ($(UNAME_S),Linux)
    # Linux settings
    CC = gcc
    SHARED_FLAG = -shared
    INSTALL_NAME_FLAG = -Wl,-soname,libCExpress.so
    LIB_EXT = so
else
    $(error Unsupported OS: $(UNAME_S))
endif

CFLAGS = -Wall -fPIC -Iinclude
SRC = $(wildcard include/CExpress/*.c)
OBJ = $(SRC:.c=.o)
TARGET = libCExpress.$(LIB_EXT)

PREFIX ?= /usr/local
LIBPATH = $(PREFIX)/lib
INCPATH = $(PREFIX)/include/CExpress

all: $(TARGET)

# Build shared library with proper OS flags
$(TARGET): $(OBJ)
	$(CC) $(SHARED_FLAG) -o $@ $^ $(INSTALL_NAME_FLAG)

# Compile .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f include/CExpress/*.o $(TARGET)

install: $(TARGET)
	mkdir -p $(LIBPATH)
	mkdir -p $(INCPATH)
	cp $(TARGET) $(LIBPATH)/
	cp include/CExpress/*.h $(INCPATH)/
	@echo "CExpress installed:"
	@echo "  Library: $(LIBPATH)/$(TARGET)"
	@echo "  Headers: $(INCPATH)"

uninstall:
	rm -f $(LIBPATH)/$(TARGET)
	rm -rf $(INCPATH)
	make clean
	@echo "CExpress fully uninstalled"
