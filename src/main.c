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

	//Settings and player structs
	// if((Players *player = (player *) malloc(sizeof(player))) == NULL) fireNotEnoughMemoryError("Player struct");


    // Program flow control variables
	SDL_Event event;
	bool quit = 0;
	GamePhase phase;

	GameTable  table = createGameTable();
	PlayerList playerList = createPlayerList();
	Pile cardPile = createPile();
	Settings settings = readSettings(argv[1]);

	for (int i = 0; i < settings.gameStg.numPlayers; i++){
		Player newPlayer = {0};
		newPlayer.type = settings.playerStg[i].playerType;
		strcpy(newPlayer.name, settings.playerStg[i].name);
		newPlayer.money = settings.playerStg[i].seedMoney;
		newPlayer.bet = settings.playerStg[i].seedBet;
		playerList.tail = createPlayer(playerList.tail, newPlayer);
		if(playerList.head == NULL) playerList.head = playerList.tail;
		playerList.totalPlayers ++;
	}
	listPlayers(playerList);
	phase = initGame(&table, &settings, &cardPile);

	if(phase);
	// initialize graphics
    InitEverything(WINDOW_WIDTH,WINDOW_HEIGHT, &serif, imgs, &window, &renderer);
    LoadCards(cards);

	while(!quit){
		while(SDL_PollEvent(&event)){
			switch (event.type){

				// check for key press
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym){

						case SDLK_h: // hit

							break;
						case SDLK_s: // stand

							break;
						case SDLK_n: // new game

							break;
						case SDLK_q: // quit
							quit = 1;
							break;
						case SDLK_d: // double

							break;
						case SDLK_r: // surrender

							break;
						case SDLK_b: // bet

							break;
					}

					break;

				// check for mouse button press
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT){
						// check position to add player
					}
					break;

				// check for quit cross press
				case SDL_QUIT:
					quit = 1;

					break;
			}
		}
	}

}


GamePhase initGame (GameTable *table, Settings *settings, Pile *pile){
	// seed random number generator
 	srand(time(NULL));

 	refillPile(pile, settings->gameStg.numDecks);

 	

 	

 	return START;

 }
