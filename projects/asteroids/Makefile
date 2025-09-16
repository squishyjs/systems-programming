# compiler
CC = gcc

# flags
CLFAGS = -Wall -Wextra -std=c11

# libraries
LIBS = -lraylib -lm -ldl -lpthread -lGl -lX11

# source file
SRC = main.c
SRC_DIR = src

# executable
OUT = asteroids

# all:
# 	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)

build-src:
	$(CC) $(CFLGAS) -o $(OUT) $(SRC_DIR)/$(SRC) $(LIBS)

# make clean (mclean)
clean:
	rm -f $(OUT)
