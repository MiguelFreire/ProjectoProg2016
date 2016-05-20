////////////////////////////////////////////////////////////////////////////////
//                             GRAPHICALINTERFACE.C                           //
//                                                                            //
// Funtions used to control de program interface                              //
////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "config.h"
#include "gameMechanics.h"
#include "graphicalInterface.h"
#include "players.h"
#include "main.h"


/**
 * @brief      Computes table slots dimensions and stores them on game table
 *             structure
 *
 * @param      table  ptr to the game table structure
 */
void calcSlotDim(GameTable *table){
    int separatorPos = (int)(0.95f*WINDOW_WIDTH);

    for (int i = 0; i < TABLE_SLOTS; i++){
        table->slotDim[i].x = i*(separatorPos/4)+5;
        table->slotDim[i].y = (int) (0.55f*WINDOW_HEIGHT);
        table->slotDim[i].w = separatorPos/4-10;
        table->slotDim[i].h = (int) (0.42f*WINDOW_HEIGHT);
    }
}

/**
 * @brief      Tests if the mouse is over one of the table slots
 *
 * @param      table   ptr to the game table strcture
 * @param[in]  mouseX  mouse x position
 * @param[in]  mouseY  mouse y position
 *
 * @return     the numberm of the slot being hovered or -1 if the mouse isn't
 *             over any table slot
 */
int mouseIsOverSlot(GameTable *table, int mouseX, int mouseY){
    SlotDim slot;
    for (int i = 0; i < TABLE_SLOTS; i++){
        slot = table->slotDim[i];
        if (mouseX > slot.x && mouseX < slot.x + slot.w
            && mouseY > slot.y && mouseY < slot.y + slot.h){

            return i;
        }
    }
    return (-1);
}

/**
 * @brief      Render player and house state overlays
 *
 * @param      font      ptr to the text font
 * @param      renderer  ptr to the renderer
 * @param      table     ptr to the game table structure
 * @param[in]  phase     current phase of the game
 */
void renderStates(TTF_Font *font, SDL_Renderer* renderer, GameTable *table, int phase){
    SDL_Rect stateRect, textRect;
    int textX, textY;
    SDL_Color white = { 255, 255, 255 }; // white
    SDL_Color black = {0,0,0}; // black
    char moneyStr[MAX_BUFFER_SIZE];
    Player *player;

    // PLAYER STATES
    for (int i = 0; i < TABLE_SLOTS; i++){
        // big rectangle dimensions
        stateRect.x = table->slotDim[i].x + 5;
        stateRect.y = table->slotDim[i].y + 5;
        stateRect.w = table->slotDim[i].w - 10;
        stateRect.h = table->slotDim[i].h - 10;

        // smaller text rectangle dimensions
        textRect.x = stateRect.x + stateRect.w/8;
        textRect.y = stateRect.y + stateRect.h/3;
        textRect.w = stateRect.w*3/4;
        textRect.h = stateRect.h/3;

        if(!slotIsEmpty(table->slots[i])){
            player = &table->slots[i]->player;

            switch (player->state){
                case BUSTED: // red
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // state text
                    textX = textRect.x + textRect.w/3.5;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "BUSTED", font, &white, renderer);
                    // money text
                    sprintf(moneyStr, "-%d€", (int)(player->bet * player->betMultiplier));
                    RenderText(textX + textRect.w/8, textY, moneyStr, font, &white, renderer);
                    break;
                case LOST: // red
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // state text
                    textX = textRect.x + textRect.w/2.8;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "LOST", font, &white, renderer);
                    // money text
                    sprintf(moneyStr, "-%d€", (int)(player->bet * player->betMultiplier));
                    RenderText(textX + textRect.w/15, textY, moneyStr, font, &white, renderer);
                    break;
                case BLACKJACK: // green
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // state text
                    textX = textRect.x + textRect.w/8;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "BLACKJACK", font, &white, renderer);
                    // money text
                    sprintf(moneyStr, "+%d€", (int)(player->bet * BLACKJACK_MULTIPLIER));
                    RenderText(textX + textRect.w/4, textY, moneyStr, font, &white, renderer);
                    break;
                case WON: // green
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // state text
                    textX = textRect.x + textRect.w/2.5;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "WIN", font, &white, renderer);
                    // money text
                    sprintf(moneyStr, "+%d€", (int)(player->bet * player->betMultiplier));
                    RenderText(textX, textY, moneyStr, font, &white, renderer);
                    break;
                case TIED: // yellow
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 200, 200, 40, 120);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 200, 180, 45, 240);
                    SDL_RenderFillRect(renderer, &textRect);
                    // state text
                    textX = textRect.x + textRect.w/2.7;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "TIED", font, &white, renderer);
                    // money text
                    sprintf(moneyStr, "+0€");
                    RenderText(textX + textRect.w/20, textY, moneyStr, font, &white, renderer);
                    break;
                case SURRENDERED: // blue
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 20, 40, 255, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 20, 40, 255, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // state text
                    textX = textRect.x + textRect.w/10;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "SURRENDERED", font, &white, renderer);
                    // money text
                    sprintf(moneyStr, "-%d€", player->bet/2);
                    RenderText(textX + textRect.w/3, textY, moneyStr, font, &white, renderer);
                    break;
                case BROKE: // black
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // state text
                    textX = textRect.x + textRect.w/4;
                    textY = textRect.y + textRect.h/8;
                    RenderText(textX, textY, "BROKE", font, &white, renderer);

                default:
                    break;
            }
        } else {
            if (phase == ADDING_PLAYER){ // white
                // big rectangle
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderFillRect(renderer, &stateRect);
                // small rectangle
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 220);
                SDL_RenderFillRect(renderer, &textRect);
                // text
                textX = textRect.x + textRect.w/10;
                textY = textRect.y + textRect.h/4;
                textY += RenderText(textX, textY, "ADD PLAYER", font, &black, renderer);
            }
        }
    }


    // HOUSE STATE
    if (phase != PLAYERS_PLAYING){
        textRect.x = WINDOW_WIDTH/10;
        textRect.y = WINDOW_HEIGHT/3;
        textRect.w = WINDOW_WIDTH/7;
        textRect.h = WINDOW_HEIGHT/12;

        switch(table->house->state) {
            case HOUSE_BLACKJACK:
                // rectangle
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &textRect);
                // state text
                textX = textRect.x + textRect.w/8;
                textY = textRect.y + textRect.h/8;
                RenderText(textX, textY, "BLACKJACK", font, &white, renderer);
                break;
            case HOUSE_BUSTED:
                // rectangle
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &textRect);
                // state text
                textX = textRect.x + textRect.w/4;
                textY = textRect.y + textRect.h/8;
                RenderText(textX, textY, "BUSTED", font, &white, renderer);
                break;
             default:
                break;
        }
    }



}

/**
 * @brief Renders the whole game area
 * 
 * @param      _font      ptr to the text font
 * @param      _img       surfaces where the table background and IST logo were
 *                        loaded
 * @param      _renderer  renderer to handle all rendering in a window
 * @param      table      ptr to the game table structure
 * @param[in]  phase      current game phase
 * @param[in]  EASpeed    current EA speed
 * 
 * Draws the table where the game will be played, namely:
 * - some texture for the background
 * - the right part with the IST logo and the student name and number
 * - squares to define the playing positions of each player
 * - names, type and the available money for each player
 * - player bet amount and hand value
 * - house hand value
 *
 */
void RenderTable(TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, GameTable *table, int phase, int EASpeed){
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white

    char strBuffer[MAX_BUFFER_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WINDOW_WIDTH); // seperates the left from the right part of the window
    int height;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    // render table texture
    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = WINDOW_HEIGHT;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);



    // render side area
    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);


    height += 50;

    // render player names and money
    height += RenderText(separatorPos+3*MARGIN, height, "==================", _font, &black, _renderer);
    for (int i = 0; i < TABLE_SLOTS; i++){
        if (!slotIsEmpty(table->slots[i])){

            sprintf(strBuffer, "%s(%s) - %d€", 
                table->slots[i]->player.name,
                table->slots[i]->player.type == 0 ? "HU" : "EA",
                table->slots[i]->player.money);
            height += RenderText(separatorPos+3*MARGIN, height, strBuffer, _font, &black, _renderer);
        }
    }
    RenderText(separatorPos+3*MARGIN, height, "==================", _font, &black, _renderer);

    // render delay level
    char delayStr[MAX_BUFFER_SIZE];
    sprintf(delayStr, "EA speed: %d", EASpeed);
    RenderText(separatorPos+3*MARGIN, WINDOW_HEIGHT - 40, delayStr, _font, &black, _renderer);

    // render player areas
    for ( int i = 0; i < TABLE_SLOTS; i++)
    {
            playerRect.x = table->slotDim[i].x;
            playerRect.y = table->slotDim[i].y;
            playerRect.w = table->slotDim[i].w;
            playerRect.h = table->slotDim[i].h;
            height = playerRect.y - 30;

            // Draw the player area
            SDL_SetRenderDrawColor(_renderer, 80, 255, 40, 50);
            SDL_RenderFillRect(_renderer, &playerRect);

        if(!slotIsEmpty(table->slots[i])) { // check if there is a player in that slot

            // draw a rectangle around the current player area
            if(i == table->currentPlayer){
                SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(_renderer, &playerRect);
            }

            sprintf(strBuffer,"bet: %d € -- %d points",
                (int)(table->slots[i]->player.bet * table->slots[i]->player.betMultiplier),
                table->slots[i]->player.handValue );
            height -= RenderText(playerRect.x+20, height, strBuffer, _font, &white, _renderer) - 10;

            sprintf(strBuffer,"%s", table->slots[i]->player.name);
            RenderText(playerRect.x+20, height, strBuffer, _font, &white, _renderer);
        }

    }

    // render house point counting
    char housePointsStr[MAX_BUFFER_SIZE];

    if (table->house->state == HOUSE_WAITING){
        RenderText(WINDOW_WIDTH/9, WINDOW_HEIGHT/4, "House points: ??", _font, &white, _renderer);
    } else {
        sprintf(housePointsStr, "House points: %d", table->house->handValue);
        RenderText(WINDOW_WIDTH/9, WINDOW_HEIGHT/4, housePointsStr, _font, &white, _renderer);
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
}


/**
 * @brief      Renders players cards on the table
 *
 * @param      _cards     vector with all loaded card images
 * @param      _renderer  renderer to handle all rendering in a window
 * @param      table      ptr to the game table structure
 */
void RenderPlayerCards(SDL_Surface **_cards, SDL_Renderer* _renderer, GameTable *table)
{
    int pos, x, y, slot, card;
    Player *curPlayer;

    // for every card of every player
    for ( slot = 0; slot < TABLE_SLOTS; slot++)
    {
        if (table->slots[slot] == NULL) continue; // pass empty slots

        curPlayer = &table->slots[slot]->player;

        for ( card = 0; card < curPlayer->numCards; card++)
        {
            // draw all cards of the player: calculate its position: only 4 positions are available !
            pos = card % 4;
            x = (int) slot*((0.95f*WINDOW_WIDTH)/4+5)+(card/4)*12+15;
            y = (int) (0.55f*WINDOW_HEIGHT)+10;
            if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
            if ( pos == 2 || pos == 3) y += CARD_HEIGHT + 10;
            // render it !
            RenderCard(x, y, peekHand(curPlayer->hand, curPlayer->numCards - card), _cards, _renderer);
        }
    }
}


/**
 * @brief      Renders the cards of the house
 *
 * @param      _cards     vector with all loaded card images
 * @param      _renderer  renderer to handle all rendering in a window
 * @param      house      ptr to the house structure
 */
void RenderHouseCards(SDL_Surface **_cards, SDL_Renderer* _renderer, House *house)
{
    int card, x, y;
    int div = WINDOW_WIDTH/CARD_WIDTH;

    // drawing all house cards
    for ( card = 0; card < house->numCards; card++)
    {
        // players still playing ? draw a card face down
        if (card == 1 && house->numCards == 2 && house->state == HOUSE_WAITING)
        {
            Card cardDown = {0};
            x = (div/2-house->numCards/2+card)*CARD_WIDTH + 15;
            y = (int) (0.26f*WINDOW_HEIGHT);
            cardDown.suit = BACK;
            RenderCard(x, y, cardDown, _cards, _renderer);
        } else {
            // calculate its position
            x = (div/2-house->numCards/2+card)*CARD_WIDTH + 15;
            y = (int) (0.26f*WINDOW_HEIGHT);
            // render it !
            RenderCard(x, y, peekHand(house->hand, house->numCards - card), _cards, _renderer);
        }

    }
}


/**
 * @brief      Draws one card at a certain position of the window, based on the
 * card rank and suit
 *
 * @param      _x         X coordinate of the card position in the window
 * @param      _y         Y coordinate of the card position in the window
 * @param      card       card structure to render
 * @param      _cards     vector with all loaded card images
 * @param      _renderer  renderer to handle all rendering in a window
 */
void RenderCard(int _x, int _y, Card card ,SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;
    int cardNumber; // 1-53

    // calculate card id for card array acessing
    if (card.suit == BACK) // card back
        cardNumber = DECK_SIZE + 1;
    else // regular cards
        cardNumber = (card.suit * 13) + (card.rank);

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[cardNumber - 1]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);

    // destroy everything
    SDL_DestroyTexture(card_text);
}


/**
 * LoadCards: Loads all images of the cards
 *
 * @param      _cards  vector with all loaded card images
 */
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


/**
 * UnLoadCards: unloads all card images of the memory
 *
 * @param      _array_of_cards  vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}


/**
 * @brief      Renders the IST Logo on the window screen
 *
 * @param      x          X coordinate of the Logo
 * @param      y          Y coordinate of the Logo
 * @param      _logoIST   surface with the IST logo image to render
 * @param      _renderer  renderer to handle all rendering in a window
 *
 * @return     height of the logo
 */
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


/**
 * RenderText function: Renders the IST Logo on the window screen
 *
 * @param      x          X coordinate of the text
 * @param      y          Y coordinate of the text
 * @param      text       string where the text is written
 * @param      _font      TTF font used to render the text
 * @param      _color     color of the text
 * @param      _renderer  renderer to handle all rendering in a window
 *
 * @return     height of the text
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderUTF8_Blended(_font,text,*_color);
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

/**
 * InitEverything: Initializes the SDL2 library and all graphical components:
 * font, window, renderer
 *
 * @param      width      width in px of the window
 * @param      height     height in px of the window
 * @param      _font      ptr to the text font
 * @param      _img       surface to be created with the table background and
 *                        IST logo
 * @param      _window    represents the window of the application
 * @param      _renderer  renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, 
    SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
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


/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * CreateWindow: Creates a window for the application
 *
 * @param      width   width in px of the window
 * @param      height  height in px of the window
 *
 * @return     pointer to the window created
 */
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


/**
 * CreateRenderer: Creates a renderer for the application
 *
 * @param      width    width in px of the window
 * @param      height   height in px of the window
 * @param      _window  represents the window for which the renderer is
 *                      associated
 *
 * @return     pointer to the renderer created
 */
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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return renderer;
}
