CC=gcc
CFLAGS=-Wall -g -std=c99 -Wextra -D_DEFAULT_SOURCE -pthread
LDFLAGS=-g
LDLIBS=-pthread
TARGET=tpnote

all: $(TARGET)

%.o: %.c grid.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): main.o grid.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@


clean:
	rm -f *.o

mrproper: clean
	rm -f $(TARGET)
