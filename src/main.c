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

#include "config.h"
#include "fileIO.h"
#include "gameMechanics.h"
#include "graphicalInterface.h"
#include "players.h"
#include "cards.h"

int main(int argc, char *argv[]){
	SDL_Event event;
	bool quit = 0;

	while( !quit ){
		while(SDL_PollEvent(&event)){
			switch (event.type){

				// check for key press
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym){

						case SLD_h: // hit
						
						case SLD_s: // stand
							
						case SLD_n: // new game
							
						case SLD_q: // quit
							quit = 1
						case SDL_d: // double
							
						case SDL_r: // surrender
							
						case SDL_b: // bet
							
					}

				// check for mouse button press
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT){
						// check position to add player
					}

				// check for quit cross press
				case SDL_QUIT:
					quit = 1;
			}
		}
	}
	
}