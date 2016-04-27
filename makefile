

CC=gcc

CFLAGS= -g -I/usr/local/include -Wall -pedantic -std=c99 

LIBL= -I/usr/include -lm -lSDL2 -lSDL2_ttf -lSDL2_image

LIBM= -I/usr/include -lm -lSDL2 -lSDL2_ttf -lSDL2_image

TARGET= blackjack

FILES= ./src/main.c ./src/graphicalInterface.c ./src/fileIO.c ./src/gameMechanics.c

linux:
	$(CC) $(CFLAGS) $(LIBL) $(FILES) -o $(TARGET)

mac:
	$(CC) $(CFLAGS) $(LIBM) $(FILES) -o $(TARGET)


clean:
	rm $(TARGET)
