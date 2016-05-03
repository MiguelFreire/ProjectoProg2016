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
#include <stdbool.h>
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
    SDL_Surface *cards[DECK_SIZE+1], *imgs[2];
    
    // Program flow control variables
	SDL_Event event;
	bool quit = 0;
	gamePhase phase;
	gameTable table;

	phase = initGame(&table);
	if(phase);
	// initialize graphics
    InitEverything(WINDOW_WIDTH,WINDOW_HEIGHT, &serif, imgs, &window, &renderer);
    LoadCards(cards);

	while( !quit ){
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


gamePhase initGame (gameTable *table){

 	table->currentPlayer = 0;
 	for (int i = 0; i < TABLE_SLOTS; i++){
 		table->players[i] = NULL;
 	}

 	return START;
 	
 }