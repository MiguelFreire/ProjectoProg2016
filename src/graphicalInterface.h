/**
 * Graphical interface funtions
 */

#ifndef GRAPHICAL_INTERFACE_H
#define	GRAPHICAL_INTERFACE_H

#include <stdbool.h>

void RenderTable(TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, GameTable table);

void RenderHouseCards(SDL_Surface **_cards, SDL_Renderer* _renderer, House *house);

void RenderPlayerCards(SDL_Surface **_cards, SDL_Renderer* _renderer, GameTable table);

void RenderCard(int _x, int _y, Card card ,SDL_Surface **_cards, SDL_Renderer* _renderer);

void LoadCards(SDL_Surface **_cards);

void UnLoadCards(SDL_Surface **_array_of_cards);

int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer);

int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer);


void InitEverything(
	int width, int height, TTF_Font **_font, SDL_Surface *_img[],
	SDL_Window** _window, SDL_Renderer** _renderer);

void InitSDL();

void InitFont();

SDL_Window* CreateWindow(int width, int height);

SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window);

#endif /* end include guard */
