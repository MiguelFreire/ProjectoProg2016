/**
 * Graphical interface functions implementation
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "config.h"
#include "graphicalInterface.h"

	/**********************
	 *Tirar isto depois
	 ************************/
	const char myName[] = "Andre Agostinho";
	const char myNumber[] = "IST425301";
	const char * playerNames[] = {"Player 1", "Player 2", "Player 3", "Player 4"};

void RenderTable(int _money[], TTF_Font *_font, SDL_Surface *_img[], 
    SDL_Renderer* _renderer, int currentPlayer)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    
    char name_money_str[MAX_BUFFER_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WINDOW_WIDTH); // seperates the left from the right part of the window
    int height;
   
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
    
    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = WINDOW_HEIGHT;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);
   
    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);
    
    // render the student name
    height += RenderText(separatorPos+3*MARGIN, height, myName, _font, &black, _renderer);
    
    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);
    
    

    // renders the areas for each player: names and money too !
    for ( int i = 0; i < TABLE_SLOTS; i++)
    {
        
        playerRect.x = i*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*WINDOW_HEIGHT);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*WINDOW_HEIGHT);

        // draw a rectangle in the current player area
	    if(i == currentPlayer){
            SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
            SDL_RenderDrawRect(_renderer, &playerRect);
        } 

        sprintf(name_money_str,"%s -- %d euros", playerNames[i], _money[i]);
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
    }
    
    // destroy everything
    SDL_DestroyTexture(table_texture);
}


void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);
    
    // destroy everything
    SDL_DestroyTexture(card_text);
}


void LoadCards(SDL_Surface **_cards)
{
    int i;
    char filename[MAX_BUFFER_SIZE];

     // loads all cards to an array
    for (i = 0 ; i < DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//img//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//img//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}


int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;
    
    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it 
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}


int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}



/******************************************************************
 * INTERFACE INITIALIZATION FUNCTIONS
 ******************************************************************/

void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
	InitSDL();
	InitFont();
	*_window = CreateWindow(width, height);
	*_renderer = CreateRenderer(width, height, *_window);
	
	// load the table texture
	_img[0] = IMG_Load(".//img//table_texture.png");
	if (_img[0] == NULL)
	{
		printf("Unable to load image: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	// load IST logo
	_img[1] = SDL_LoadBMP(".//img//ist_logo.bmp");
	if (_img[1] == NULL)
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	// this opens (loads) a font file and sets a size
	*_font = TTF_OpenFont(".//img//FreeSerif.ttf", 16);
	if(!*_font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
}


void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}


SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}


SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

    return renderer;
}