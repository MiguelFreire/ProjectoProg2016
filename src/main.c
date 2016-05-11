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

	// structures
	Settings settings = {0};
	GameTable  table = createGameTable();
	House house = createHouse();
	PlayerList playerList = createPlayerList();
	Pile cardPile = createPile();
<<<<<<< Updated upstream
	
	phase = initGame(&table, &playerList, &cardPile, &house, &settings, argv[1]);

	
=======

	phase = initGame(&table, &playerList, &cardPile, &settings, argv[1]);


	// testing shit
	table.slots[0]->player.hand = pushToHand(table.slots[0]->player.hand, dealCard(&cardPile), &(table.slots[0]->player.numCards));
	table.slots[0]->player.hand = pushToHand(table.slots[0]->player.hand, dealCard(&cardPile), &(table.slots[0]->player.numCards));

	table.slots[1]->player.hand = pushToHand(table.slots[1]->player.hand, dealCard(&cardPile), &(table.slots[1]->player.numCards));
	table.slots[1]->player.hand = pushToHand(table.slots[1]->player.hand, dealCard(&cardPile), &(table.slots[1]->player.numCards));

	house.hand = pushToHand(house.hand, dealCard(&cardPile), &(house.numCards));
	house.hand = pushToHand(house.hand, dealCard(&cardPile), &(house.numCards));

	// end testing shit

>>>>>>> Stashed changes
	if(phase);
	// initialize graphics
	InitEverything(WINDOW_WIDTH,WINDOW_HEIGHT, &serif, imgs, &window, &renderer);
	LoadCards(cards);

	actionNewGame(&table, &cardPile);

	while(!quit){
		while(SDL_PollEvent(&event)){
			switch (event.type){

				// check for key press
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym){

						case SDLK_h: // hit
							actionHit(&table, &cardPile, PLAYER);
							break;
						case SDLK_s: // stand
							actionStand(&table);
							break;
						case SDLK_n: // new game
							if (phase == WAINTING_FOR_NEW_GAME){

							}
							break;
						case SDLK_q: // quit
							quit = true;
							break;
						case SDLK_d: // double

							break;
						case SDLK_r: // surrender

							break;
						case SDLK_b: // bet
						    actionBet(&table);
							if (phase == WAINTING_FOR_NEW_GAME){

							}
							break;
						case SDLK_a: // add player
						    if (phase == WAINTING_FOR_NEW_GAME){

						    }
						    break;
					}

					break;

				// check for mouse button press
				case SDL_MOUSEBUTTONDOWN:
					if (phase == ADDING_PLAYER && 
						event.button.button == SDL_BUTTON_LEFT){
						// check position to add player
						/*
						int mouseX, mouseY, slotClicked;
						if (slotClicked = hoveringSlot(mouseX, mouseY) >= 0){
							addPlayer(slotClicked, &playerList, &house);
						}
						*/
					}
					break;

				// check for quit cross press
				case SDL_QUIT:
					quit = true;

					break;
			}
		}

		// render game table
		RenderTable(serif, imgs, renderer, &table);
		// render the players cards
		RenderPlayerCards(cards, renderer, &table);
		// render house cards
		RenderHouseCards(cards, renderer, &house);
		// put to screen all changes above
		SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay(RENDER_DELAY);
	}

	// free everything and quit the program
	freeEverything(&playerList, &house, &cardPile, &settings);
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


	// create starting players
	for (int i = 0; i < settings->gameStg.numPlayers; i++){
		Player newPlayer = {0};

		// assign player setting to the player
		newPlayer.type = settings->playerStg[i].playerType;
		strcpy(newPlayer.name, settings->playerStg[i].name);
		newPlayer.money = settings->playerStg[i].seedMoney;
		newPlayer.bet = settings->playerStg[i].seedBet;


		// put player on the player list
		playerList->tail = createPlayer(playerList, newPlayer);

		// assign the player to a table slot
		table->slots[i] = playerList->tail;
	}
	listPlayers(playerList);

	// set the table pointer to the house
	table->house = house;

	pile->numDecks = settings->gameStg.numDecks;
	refillPile(pile);

	return START;
 }


void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, Settings *settings){
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
	// free settings
	freeSettingsStruct(settings);
}
