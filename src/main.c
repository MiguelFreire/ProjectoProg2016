/**
 * BlackJack
 *
 * MEEC Prog 2015/2016 1º Semestre
 *
 * Authors: André Agostinho 84001, Miguel Freire 84145
 *
 * Programa elaborado para a entrega final do projeto da cadeira de
 * Programação
 */

////////////////////////////////////////////////////////////////////////////////
// 									MAIN.C									  //
// 																			  //
// Program flow control funtions											  //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "config.h"
#include "util.h"
#include "errorHandling.h"

#include "graphicalInterface.h"
#include "gameMechanics.h"
#include "players.h"
#include "cards.h"
#include "settings.h"
#include "stats.h"
#include "EA.h"
#include "main.h"

/**
 * @brief      Main program flow
 *
 * @param[in]  argc  Number of program arguments
 * @param      argv  Program arguments
 *
 * @return     Program exit status
 *
 * argc should be 3 and argv should contain:
 * 		*[0] the name of the program
 * 		*[1] the name of the game config file
 * 		*[2] the name of the EA matrixes file
 *
 * The program is controled using the vaiable quit to determine when the program
 * should quit and the variable phase to determine what is the program is
 * currently doing.
 *
 * The variable event is used with SDL_PollEvent to colect user input trough
 * keyboard and mouse
 */
int main(int argc, char *argv[]){
	// Graphical interface variables
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	TTF_Font *serif = NULL;
	SDL_Surface *cards[DECK_SIZE + 1], *imgs[2];


	// Program flow control variables
	SDL_Event event;
	bool quit = false;
	GamePhase phase;

	// Structures
	Settings settings = {{0}};
	GameTable  table = createGameTable();
	House house = createHouse();
	PlayerList playerList = createPlayerList();
	Pile cardPile = createPile();

	// EA
	int **softMatrix = NULL;
	int **hardMatrix = NULL;
	readEAMatrix(argv[2], &softMatrix, &hardMatrix);
	int EASpeed = SPEED_LEVELS/2 - 1;
	int EADelay = increaseEASpeed(&EASpeed);


	// INITIALIZATION SEQUENCE

	// game initialization
	phase = initGame(&table, &playerList, &cardPile, &house, &settings, argv[1]);

	// initialize graphics
	InitEverything(WINDOW_WIDTH,WINDOW_HEIGHT, &serif, imgs, &window, &renderer);
	LoadCards(cards);

	// render for the first time
	// render game table
	RenderTable(serif, imgs, renderer, &table, phase, EASpeed);
	// put to screen all changes above
	SDL_RenderPresent(renderer);

	clearTerminal();

	// inform the user to press n for new game
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "New Game",
	"Press 'n' to start a new game", window);

	phase = WAITING_FOR_NEW_GAME;

	// FLOW CONTROL
	while(!quit){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				// check for key press
				case SDL_KEYDOWN:

					phase = handleKeyPress(window, &event, &table, &cardPile,
						phase, &quit, &EASpeed, &EADelay);
					break;

				// check for mouse button press
				case SDL_MOUSEBUTTONDOWN:

					phase = handleMousePress(window, &event, &table, &playerList, phase);
					break;

				// check for quit cross press
				case SDL_QUIT:
					quit = true;
					break;
			}
		}

		// special phases
		if (phase == EA_PLAYING || phase == HOUSE_TURN || phase == COLECTING_BETS){

			renderEverything(serif, imgs, renderer, cards, &table,
				phase, EASpeed);

			// let EA player make a decision
			if (phase == EA_PLAYING){
				phase = EAMakeDecision(window, softMatrix, hardMatrix, &table,
					&cardPile);
				SDL_Delay(EADelay);

			// let house make a decision
			} else if (phase == HOUSE_TURN){
				house.state = HOUSE_PLAYING;
				phase = houseTurn(&table, &house, &cardPile);
				// set current player to the first player to start collecting bets
				if (phase == COLECTING_BETS){
					table.currentPlayer = 0;
				}

			// colect bets
			} else if (phase == COLECTING_BETS){
				phase = colectBets(&table, &house);
			}
		}

		renderEverything(serif, imgs, renderer, cards, &table,
				phase, EASpeed);

        SDL_Delay(RENDER_DELAY);
	}

	// QUIT SEQUENCE

	// write stats
	writeStats(playerList);
	// free everything and quit the program
	freeEverything(&playerList, &house, &cardPile, &settings, softMatrix, hardMatrix);
	UnLoadCards(cards);
	TTF_CloseFont(serif);
	SDL_FreeSurface(imgs[0]);
	SDL_FreeSurface(imgs[1]);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	logPlay("\b", "Exited sucessfully");
	return EXIT_SUCCESS;

}




////////////////////////////////////////////////////////////////////////////////
//						GAME FLOW CONTROL FUNTIONS							  //
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief      Initializes program variables
 *
 * @param      table       ptr to the table structure
 * @param      playerList  ptr to the player list structure
 * @param      pile        ptr to the card pile structure
 * @param      house       ptr to the house structure
 * @param      settings    ptr to the settings structure
 * @param      argv1       argv[1] - config file name
 *
 * @return     new game phase (always START)
 */
GamePhase initGame (GameTable *table, PlayerList *playerList, Pile *pile,
	House *house, Settings *settings, char *argv1){
	// seed random number generator
	srand(time(NULL));


	*settings = readSettings(argv1);

	// create starting players from config file
	for (int i = 0; i < settings->gameStg.numPlayers; i++){
		Player newPlayer = {0};

		// assign player setting to the player
		newPlayer.type = settings->playerStg[i].playerType;
		strcpy(newPlayer.name, settings->playerStg[i].name);
		newPlayer.money = settings->playerStg[i].seedMoney;
		newPlayer.bet = settings->playerStg[i].seedBet;
		newPlayer.betMultiplier = 1;


		// put player on the player list
		playerList->tail = createPlayer(playerList, newPlayer);

		// assign the player to a table slot
		assignPlayerToSlot(playerList->tail, i, table);
	}


	// set the table pointer to the house
	table->house = house;

	table->currentPlayer = -1; // no player is playing

	// compute and store table slots dimensions
	calcSlotDim(table);

	// fill the card pile
	pile->numDecks = settings->gameStg.numDecks;
	refillPile(pile);

	return START;
 }



/**
 * @brief      Handles keybooard keys
 *
 * @param      event    ptr to the SDL event
 * @param      table    ptr to the game table
 * @param      pile     ptr to the card pile
 * @param[in]  phase    current game phase
 * @param      quit     ptr to the quit control variable
 * @param      EASpeed  ptr to the EA speed variable
 * @param      EADelay  ptr to the EA delay variable
 *
 * @return     the new game phase
 */
GamePhase handleKeyPress(SDL_Window *window, SDL_Event *event, GameTable *table,
	Pile *pile, GamePhase phase, bool *quit, int *EASpeed, int *EADelay){

	switch( event->key.keysym.sym){

		case SDLK_h: // hit
		    if (phase == PLAYERS_PLAYING){
		    	phase = actionHit(table, pile);
		    }
			break;
		case SDLK_s: // stand
		    if (phase == PLAYERS_PLAYING){
		    	phase = actionStand(table);
		    }
			break;
		case SDLK_n: // new game
			if (phase == WAITING_FOR_NEW_GAME){
				phase = actionNewGame(window, table, pile);
			}
			break;
		case SDLK_q: // quit
		    if (phase == WAITING_FOR_NEW_GAME){
				*quit = true;
			}
			break;
		case SDLK_d: // double
		    if (phase == PLAYERS_PLAYING){
		    	phase = actionDouble(window, table, pile, 0);
		    }
			break;
		case SDLK_r: // surrender
			if (phase == PLAYERS_PLAYING){
				phase = actionSurrender(window, table);
			}
			break;
		case SDLK_b: // bet

			if (phase == WAITING_FOR_NEW_GAME){
				// warn user that input is needed at the terminal
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Input Needed",
				"Please check the terminal to provide some input", window);

				actionBet(table);

			}
			break;
		case SDLK_a: // add player
		    if (phase == WAITING_FOR_NEW_GAME){
		    	bool emptySlots = false;
				for (int i = 0; i < TABLE_SLOTS; i++){
					if (slotIsEmpty(table->slots[i])){
						emptySlots = true;
					}
				}
				if (emptySlots){
					phase = ADDING_PLAYER;
					// inform the user to click an empty slot
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Adding Player",
					"Please click an empty slot to add a player", window);

				} else {
					phase = WAITING_FOR_NEW_GAME;
					// inform the user that there are no empty slots
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Adding Player",
					"There are no empty slots for new players", window);
				}
		    }
		    break;
		case SDLK_UP: // increase EA speed
			*EADelay = increaseEASpeed(EASpeed);
			break;
		case SDLK_DOWN:	// decrease EA speed
			*EADelay = decreaseEASpeed(EASpeed);
			break;
	}

	return phase;
}


/**
 * @brief      Handles mouse button presses
 *
 * @param      event       ptr to the SDL event structure
 * @param      table       ptr to the table
 * @param      playerList  ptr to the player list
 * @param[in]  phase       current game phase
 *
 * @return     the new game phase
 */
GamePhase handleMousePress(SDL_Window *window, SDL_Event *event, GameTable *table, PlayerList *playerList, GamePhase phase){
	if (phase == ADDING_PLAYER &&
		event->button.button == SDL_BUTTON_LEFT){
		// check position to add player
		int mouseX, mouseY, slotClicked;
		SDL_GetMouseState(&mouseX, &mouseY);
		slotClicked = mouseIsOverSlot(table, mouseX, mouseY);
		if (slotClicked >= 0){
			if (slotIsEmpty(table->slots[slotClicked])){
				// warn user that input is needed at the terminal
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Input Needed",
				"Please check the terminal to provide some input", window);

				phase = actionAddPlayer(slotClicked, playerList, table);
			}
		}
	}
	return phase;
}
