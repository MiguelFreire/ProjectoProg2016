

CC=gcc

CFLAGS= -g -I/usr/local/include -Wall -pedantic -std=c99

LIBL= -I/usr/include -lm -lSDL2 -lSDL2_ttf -lSDL2_image

LIBM= -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image


TARGET= blackjack

FILES= ./src/*.c

linux:
	@ echo "Compiling blackjack"
	@ echo ""

	$(CC) $(CFLAGS) $(LIBL) $(FILES) -o $(TARGET)

#Check for compiling failure
	@ if [ $? -eq 0 ]; then \
		echo ""; \
		echo "Done compiling"; \
	else \
		echo "!!!!!!!!!Compiling Errors!!!!!!!!!!!"; \
	fi;

mac:
	@ echo "Compiling blackjack"
	@ echo ""

	$(CC) $(CFLAGS) $(LIBM) $(FILES) -o $(TARGET)

clean:
	rm $(TARGET)
