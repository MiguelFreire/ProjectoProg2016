

CC=gcc

CFLAGS= -g  -Wall -pedantic -std=c99

LIB= -I/usr/local/include -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image


TARGET= blackjack

FILES= ./src/*.c

all:
	@ echo "Compiling blackjack"
	@ echo "---------------------------------------------------"
	@ echo "	André Agostinho	|	Miguel Freire"
	@ echo "	84001		|	84145"
	@ echo "---------------------------------------------------"
	@ echo ""

	$(CC) $(CFLAGS) $(LIB) $(FILES) -o $(TARGET)

	@ echo ""

clean:
	rm $(TARGET)
