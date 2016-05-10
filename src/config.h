/**
 * Program Configurations including max size for certain parameters
 */

#ifndef CONFIG_H
#define	CONFIG_H

#define WINDOW_WIDTH	900      // window width
#define WINDOW_HEIGHT	525     // window height
#define CARD_WIDTH	67         // card width
#define CARD_HEIGHT	97        // card height
#define WINDOW_POSX	200       // initial position of the window: x
#define WINDOW_POSY	100       // initial position of the window: y
#define EXTRASPACE	150
#define MARGIN	5

#define RENDER_DELAY 200	// delay between renders (ms)

#define DECK_SIZE	52	// num of card in a deck
#define CARD_RANKS	13	// num of different card ranks

#define MIN_DECKS 4 // min number of decks to use
#define MAX_DECKS 8 // max number of decks to use

#define MIN_PLAYERS 1 // min players to start the game with
#define TABLE_SLOTS	4	// num of table slots for players (max players)

#define MAX_NAME_SIZE 8	// max player name size
#define MAX_BUFFER_SIZE 100	// max size for some buffers

#define MAX_PLAYER_TYPE_SIZE 2	// max size for player type chars buffer

#define SETTINGS_FILE_NAME "settings.txt"

#define AUTHOR_NAME1	"Andre Agostinho"
#define AUTHOR_NAME2	"Miguel Freire"
#define AUTHOR_NUMBER1	"IST425301"
#define AUTHOR_NUMBER2	"IST425301"

#endif /* end include guard */
