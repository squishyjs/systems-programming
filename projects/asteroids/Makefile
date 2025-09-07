# compiler
CC = gcc

# flags
CLFAGS = -Wall -Wextra -std=c11

# libraries
LIBS = -lraylib -lm -ldl -lpthread -lGl -lX11

# source file
SRC = main.c

# executable
OUT = snake

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)

# make clean (mclean)
clean:
	rm -f $(OUT)
