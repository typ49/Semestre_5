CC=gcc
CFLAGS=-Wall -g -std=c99 -Wextra -D_DEFAULT_SOURCE 

TARGET1=client
TARGET2=serveur

all: $(TARGET1) $(TARGET2)


%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET1): client.o 
	$(CC) $^ -o $@


$(TARGET2): serveur.o
	$(CC) $^ -o $@

clean:
	rm -f *.o

mrproper: clean
	rm -f $(TARGET1) $(TARGET2)

