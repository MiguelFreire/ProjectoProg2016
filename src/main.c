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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "main.h"
#include "config.h"
#include "gameMechanics.h"
#include "graphicalInterface.h"
#include "players.h"
#include "cards.h"
#include "util.h"
#include "settings.h"
#include "errorHandling.h"
#include "stats.h"
#include "EA.h"

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


	int EASpeed = SPEED_LEVELS/2;
	int EADelay = increaseEADelay(&EASpeed);

	// game initialization
	phase = initGame(&table, &playerList, &cardPile, &house, &settings, argv[1]);

	// initialize graphics
	InitEverything(WINDOW_WIDTH,WINDOW_HEIGHT, &serif, imgs, &window, &renderer);
	LoadCards(cards);

	phase = WAITING_FOR_NEW_GAME;

	// render for the first time
	// render game table
	RenderTable(serif, imgs, renderer, &table, phase, EASpeed);
	// put to screen all changes above
	SDL_RenderPresent(renderer);

	clearTerminal();

	// inform the user to press n for new game
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "New Game",
	"Press 'n' to start a new game", window);

	while(!quit){
		while(SDL_PollEvent(&event)){
			switch (event.type){

				// check for key press
				case SDL_KEYDOWN:
					phase = handleKeyPress(&event, &table, &cardPile, 
						phase, &quit, &EASpeed, &EADelay);
					break;

				// check for mouse button press
				case SDL_MOUSEBUTTONDOWN:

					phase = handleMousePress(&event, &table, &playerList, phase);

					break;

				// check for quit cross press
				case SDL_QUIT:
					quit = true;

					break;
			}
		}

		if (phase == EA_PLAYING || phase == HOUSE_TURN || phase == COLECTING_BETS){
			// render game table
			RenderTable(serif, imgs, renderer, &table, phase, EASpeed);
			// render the players cards
			RenderPlayerCards(cards, renderer, &table);
			// render house cards
			RenderHouseCards(cards, renderer, &house);
			// render info orverlays
			renderStates(serif, renderer, &table, phase);
			// put to screen all changes above
			SDL_RenderPresent(renderer);
	        // add a delay
	        SDL_Delay(RENDER_DELAY/2);

			// Let EA player make a decision
			if (phase == EA_PLAYING){
				printf ("EA playing\n");
				EAAction action = actionDecoder(softMatrix, hardMatrix, &table);
				printf ("action: %d\n", action);
				switch (action){
					case aHIT:
						phase = actionHit(&table, &cardPile);
						break;
					case aDOUBLES:
						phase = actionDouble(&table, &cardPile, aSTAND);
						break;
					case aDOUBLEH:
						phase = actionDouble(&table, &cardPile, aHIT);
						break;
					case aSURRENDER:
						phase = actionSurrender(&table);
						break;
					case aSTAND:
						phase = actionStand(&table);
						break;
					default:
						phase = actionStand(&table);
				}
				SDL_Delay(EADelay);
				printf ("EA done\n");

			} else if (phase == HOUSE_TURN){
				printf("house playing\n");
				house.state = HOUSE_PLAYING;
				phase = houseTurn(&table, &house, &cardPile);
				if (phase == COLECTING_BETS){
					table.currentPlayer = 0;
					printf ("house going to colect\n");
				}
			} else if (phase == COLECTING_BETS){
				printf("clecting bets\n");
				phase = colectBets(&table, &house);
			}
		}

		// render game table
		RenderTable(serif, imgs, renderer, &table, phase, EASpeed);
		// render the players cards
		RenderPlayerCards(cards, renderer, &table);
		// render house cards
		RenderHouseCards(cards, renderer, &house);
		// render info orverlays
		renderStates(serif, renderer, &table, phase);
		// put to screen all changes above
		SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay(RENDER_DELAY);
	}

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
	printf("Exited sucessfully\n");
	return EXIT_SUCCESS;

}


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
		table->slots[i] = playerList->tail;
	}
	listPlayers(playerList);



	// set the table pointer to the house
	table->house = house;

	table->currentPlayer = -1; // no player is playing

	// compute and store table slots dimensions
	calcSlotDim(table);

	// fill the pile
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
GamePhase handleKeyPress(SDL_Event *event, GameTable *table, Pile *pile, GamePhase phase, bool *quit, int *EASpeed, int *EADelay){

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
				phase = actionNewGame(table, pile);
			}
			break;
		case SDLK_q: // quit
		    if (phase == WAITING_FOR_NEW_GAME){
				*quit = true;
			}
			break;
		case SDLK_d: // double
		    if (phase == PLAYERS_PLAYING){
		    	phase = actionDouble(table, pile, 0);
		    }
			break;
		case SDLK_r: // surrender
			if (phase == PLAYERS_PLAYING){
				phase = actionSurrender(table);
			}
			break;
		case SDLK_b: // bet

			if (phase == WAITING_FOR_NEW_GAME){
				// warn user that input is needed at the terminal
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Input Needed",
				"Please check the terminal to provide some input", NULL);
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
					"Please click an empty slot to add a player", NULL);
		    		printf("Adding player\n");
				} else {
					phase = WAITING_FOR_NEW_GAME;
					// inform the user that there are no empty slots
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Adding Player",
					"There are no empty slots for new players", NULL);
					printf("There are no empty slots\n");
				}
		    }
		    break;
		case SDLK_UP: // increase EA delay
			*EADelay = increaseEADelay(EASpeed);
			break;
		case SDLK_DOWN:	// decrease EA delay
			*EADelay = decreaseEADelay(EASpeed);
			break;
	}

	return phase;
}

GamePhase handleMousePress(SDL_Event *event, GameTable *table, PlayerList *playerList, GamePhase phase){
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
				"Please check the terminal to provide some input", NULL);
				
				phase = actionAddPlayer(slotClicked, playerList, table);
			}
		}
	}
	return phase;
}

void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, Settings *settings, int **softMatrix, int **hardMatrix){
	CardNode *tmpCard = NULL;
	// free players
	while (playerList->head != NULL){
		// remove player frees the player hand and the player
		playerList->head = removePlayer(playerList);
	}
	// free house hand
	tmpCard = house->hand;
	while (tmpCard != NULL){
		tmpCard = popHand(tmpCard, NULL, &house->numCards);
	}
	// free card pile

	while (cardPile->pileTop != NULL){
		cardPile->pileTop = removeCardFromTop(cardPile);
	}
	//free EA matrixes
	freeMatrixes(softMatrix, hardMatrix);

	// free settings
	freeSettingsStruct(settings);
}
