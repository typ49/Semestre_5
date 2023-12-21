# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra

# Targets
all: TP6 TP6_V2 TP6_V3

TP6: TP6.c
	$(CC) $(CFLAGS) -o TP6 TP6.c

TP6_V2: TP6_V2.c
	$(CC) $(CFLAGS) -o TP6_V2 TP6_V2.c

TP6_V3 : TP6_V3.c
	$(CC) $(CFLAGS) -o TP6_V3 TP6_V3.c -lpthread

# Phony target to run the programs
run: all
	./TP6 10000000
	./TP6_V2 10000000
	./TP6_V3 10000000

# Phony target to clean the directory
clean:
	rm -f TP6 TP6_V2

# Declare phony targets
.PHONY: all run clean
