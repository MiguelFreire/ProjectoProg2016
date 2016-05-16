

CC=gcc

CFLAGS= -g -I/usr/local/include -Wall -pedantic -std=c99

LIB= -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image


TARGET= blackjack

FILES= ./src/*.c

all:
	@ echo "Compiling blackjack"
	@ echo "André Agostinho	| Miguel Freire"
	@ echo "	84001	|	84Cenas"
	@ echo ""

	$(CC) $(CFLAGS) $(LIB) $(FILES) -o $(TARGET)

clean:
	rm $(TARGET)
