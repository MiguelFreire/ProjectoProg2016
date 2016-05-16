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
#include "fileIO.h"
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
	/**********************
	 * READ SETTINGS BLOCK
	 * Settings *stg = readSettings();
	 * DONT FORGET TO FREE THE SETTINGS STRUCT!!!!!
	 ***********************/
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
	Settings settings = {0};
	GameTable  table = createGameTable();
	House house = createHouse();
	PlayerList playerList = createPlayerList();
	Pile cardPile = createPile();

	// EA
	int **softMatrix = readSoftEAMatrix();
	int **hardMatrix = readHardEAMatrix();
	int EADelayLevel = 3;

	phase = initGame(&table, &playerList, &cardPile, &house, &settings, argv[1]);

	// initialize graphics
	InitEverything(WINDOW_WIDTH,WINDOW_HEIGHT, &serif, imgs, &window, &renderer);
	LoadCards(cards);

	phase = WAITING_FOR_NEW_GAME;
	// render for the first time
	// render game table
	RenderTable(serif, imgs, renderer, &table, phase, EADelayLevel);
	// put to screen all changes above
	SDL_RenderPresent(renderer);
	// add a delay
    SDL_Delay(2 * RENDER_DELAY);

    // clear the terminal
	printf("\033[2J\033[1;1H");
	
	// inform the user to press n for new game
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "New Game", 
	"Press 'n' to start a new game", window);

	while(!quit){
		if (phase != PLAYERS_PLAYING || table.slots[table.currentPlayer]->player.type == HUMAN){
			while(SDL_PollEvent(&event)){
				switch (event.type){

					// check for key press
					case SDL_KEYDOWN:

						switch( event.key.keysym.sym){

							case SDLK_h: // hit
							    if (phase == PLAYERS_PLAYING){
							    	phase = actionHit(&table, &cardPile, PLAYER);
							    }
								break;
							case SDLK_s: // stand
							    if (phase == PLAYERS_PLAYING){
							    	phase = actionStand(&table);
							    }
								break;
							case SDLK_n: // new game
								if (phase == WAITING_FOR_NEW_GAME){
									phase = actionNewGame(&table, &cardPile);
								}
								break;
							case SDLK_q: // quit
							    if (phase == WAITING_FOR_NEW_GAME){
									quit = true;
								}
								break;
							case SDLK_d: // double
							    if (phase == PLAYERS_PLAYING){
							    	phase = actionDouble(&table, &cardPile);
							    }
								break;
							case SDLK_r: // surrender
								if (phase == PLAYERS_PLAYING){
									phase = actionSurrender(&table);
								}
								break;
							case SDLK_b: // bet

								if (phase == WAITING_FOR_NEW_GAME){
									// warn user that input is needed at the terminal
									SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Input Needed", 
									"Please check the terminal to provide some input", window);
									actionBet(&table);

								}
								break;
							case SDLK_a: // add player
							    if (phase == WAITING_FOR_NEW_GAME){
							    	bool emptySlots = false;
									for (int i = 0; i < TABLE_SLOTS; i++){
										if (slotIsEmpty(table.slots[i])){
											emptySlots = true;
										}
									}
									if (emptySlots){
										phase = ADDING_PLAYER;
										// inform the user to click an empty slot
										SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Adding Player", 
										"Please click an empty slot to add a player", window);
							    		printf("Adding player\n");
									} else {
										phase = WAITING_FOR_NEW_GAME;
										// inform the user that there are no empty slots
										SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Adding Player", 
										"There are no empty slots for new players", window);
										printf("There are no empty slots\n");
									}
							    }
							    break;
							case SDLK_UP: // increase EA delay
								EADelayLevel = increaseEADelay(EADelayLevel);
								break;
							case SDLK_DOWN:	// decrease EA delay
								EADelayLevel = decreaseEADelay(EADelayLevel);
								break;
						}

						break;

					// check for mouse button press
					case SDL_MOUSEBUTTONDOWN:
						if (phase == ADDING_PLAYER &&
							event.button.button == SDL_BUTTON_LEFT){
							// check position to add player
							int mouseX, mouseY, slotClicked;
							SDL_GetMouseState(&mouseX, &mouseY);
							slotClicked = mouseIsOverSlot(&table, mouseX, mouseY);
							if (slotClicked >= 0){
								if (slotIsEmpty(table.slots[slotClicked])){
									// warn user that input is needed at the terminal
									SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Input Needed", 
									"Please check the terminal to provide some input", window);
									printf("\033[2J\033[1;1H");
									printf("Adding player at slot %d\n", slotClicked + 1);
									phase = actionAddPlayer(slotClicked, &playerList, &table);
								}

							}

						}
						break;

					// check for quit cross press
					case SDL_QUIT:
						quit = true;

						break;
				}
			}
		} else {
			printf ("EA playing\n");
			EAAction action = actionDecoder(softMatrix, hardMatrix, &table);
			printf ("action: %d\n", action);
			switch (action){
				case aHIT:
					phase = actionHit(&table, &cardPile, PLAYER);
					break;
				case aDOUBLE:
					phase = actionDouble(&table, &cardPile);
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
			SDL_Delay((EADelayLevel - 1)/2.0 * 1000);
			printf ("EA done\n");
		}

		if (phase == HOUSE_TURN){
			printf("house playing\n");
			phase = houseTurn(&table, &house, &cardPile);
			if (phase == COLECTING_BETS){
				table.currentPlayer = 0;
				printf ("house going to colect\n");
			}
		}

		if (phase == COLECTING_BETS){
			printf("clecting bets\n");
			phase = colectBets(&table, &house);
		}

		// render game table
		RenderTable(serif, imgs, renderer, &table, phase, EADelayLevel);
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
	//writeStats(playerList);
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
