/**
 * Graphical interface functions implementation
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "config.h"
#include "gameMechanics.h"
#include "graphicalInterface.h"
#include "players.h"
#include "main.h"


void calcSlotDim(GameTable *table){
    int separatorPos = (int)(0.95f*WINDOW_WIDTH);

    for (int i = 0; i < TABLE_SLOTS; i++){
        table->slotDim[i].x = i*(separatorPos/4)+5;
        table->slotDim[i].y = (int) (0.55f*WINDOW_HEIGHT);
        table->slotDim[i].w = separatorPos/4-10;
        table->slotDim[i].h = (int) (0.42f*WINDOW_HEIGHT);
    }
}

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

void renderStates(TTF_Font *font, SDL_Renderer* renderer, GameTable *table, int phase){
    SDL_Rect stateRect, textRect;
    int textX, textY;
    SDL_Color white = { 255, 255, 255 }; // white
    SDL_Color black = {0,0,0}; // black
    char moneyStr[MAX_BUFFER_SIZE];
    Player *player;

    for (int i = 0; i < TABLE_SLOTS; i++){
        stateRect.x = table->slotDim[i].x + 5;
        stateRect.y = table->slotDim[i].y + 5;
        stateRect.w = table->slotDim[i].w - 10;
        stateRect.h = table->slotDim[i].h - 10;

        textRect.x = stateRect.x + stateRect.w/8;
        textRect.y = stateRect.y + stateRect.h/3;
        textRect.w = stateRect.w*3/4;
        textRect.h = stateRect.h/3;

        if(!slotIsEmpty(table->slots[i])){
            player = &table->slots[i]->player;

            switch (player->state){
                case BUSTED:
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // text
                    textX = textRect.x + textRect.w/4;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "BUSTED", font, &white, renderer);
                    sprintf(moneyStr, "-%d€", (int)(player->bet * player->betMultiplier));
                    RenderText(textX + textRect.w/8, textY, moneyStr, font, &white, renderer);
                    break;
                case BLACKJACK:
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // text
                    textX = textRect.x + textRect.w/10;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "BLACKJACK", font, &white, renderer);
                    sprintf(moneyStr, "+%d€", (int)(player->bet * player->betMultiplier));
                    RenderText(textX + textRect.w/3, textY, moneyStr, font, &white, renderer);
                    break;
                case SURRENDERED:
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // text
                    textX = textRect.x + textRect.w/10;
                    textY = textRect.y + textRect.h/8;
                    textY += RenderText(textX, textY, "SURRENDERED", font, &white, renderer);
                    sprintf(moneyStr, "-%d€", player->bet/2);
                    RenderText(textX + textRect.w/3, textY, moneyStr, font, &white, renderer);
                    break;
                case BROKE:
                    // big rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
                    SDL_RenderFillRect(renderer, &stateRect);
                    // small rectangle
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
                    SDL_RenderFillRect(renderer, &textRect);
                    // text
                    textX = textRect.x + textRect.w/4;
                    textY = textRect.y + textRect.h/8;
                    RenderText(textX, textY, "BROKE", font, &white, renderer);
                default:
                    break;
            }
        } else {
            if (phase == ADDING_PLAYER){
                // big rectangle
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderFillRect(renderer, &stateRect);
                // small rectangle
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 220);
                SDL_RenderFillRect(renderer, &textRect);
                // text
                textX = textRect.x + textRect.w/10;
                textY = textRect.y + textRect.h/8;
                textY += RenderText(textX, textY, "ADD PLAYER", font, &black, renderer);
            }
        }
    }
}

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, GameTable *table, int phase, int delayLevel){
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white

    char nameMoneyStr[MAX_BUFFER_SIZE];
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
    // render phase
    char phaseStr[10];
    sprintf(phaseStr, "Phase %d", phase);
    height += RenderText(separatorPos+3*MARGIN, height, phaseStr, _font, &black, _renderer);

    // render delay level
    char delayStr[10];
    sprintf(delayStr, "Delay %d", delayLevel);
    height += RenderText(separatorPos+3*MARGIN, height, delayStr, _font, &black, _renderer);

    // render player names and money
    height += RenderText(separatorPos+3*MARGIN, height, "==============", _font, &black, _renderer);
    for (int i = 0; i < TABLE_SLOTS; i++){
        if (!slotIsEmpty(table->slots[i])){
            sprintf(nameMoneyStr, "%s - %d€", table->slots[i]->player.name, table->slots[i]->player.money);
            height += RenderText(separatorPos+3*MARGIN, height, nameMoneyStr, _font, &black, _renderer);
        }
    }
    RenderText(separatorPos+3*MARGIN, height, "==============", _font, &black, _renderer);



    // render player areas
    for ( int i = 0; i < TABLE_SLOTS; i++)
    {
            playerRect.x = table->slotDim[i].x;
            playerRect.y = table->slotDim[i].y;
            playerRect.w = table->slotDim[i].w;
            playerRect.h = table->slotDim[i].h;

            // Draw the player area
            SDL_SetRenderDrawColor(_renderer, 80, 255, 40, 50);
            SDL_RenderFillRect(_renderer, &playerRect);

        if(!slotIsEmpty(table->slots[i])) { // check if there is a player in that slot

            // draw a rectangle around the current player area
            if(i == table->currentPlayer){
                SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(_renderer, &playerRect);
            }

            sprintf(nameMoneyStr,"%s -- %d bet -- %d points",
                table->slots[i]->player.name,
                (int)(table->slots[i]->player.bet * table->slots[i]->player.betMultiplier),
                table->slots[i]->player.handValue);
            RenderText(playerRect.x+20, playerRect.y-30, nameMoneyStr, _font, &white, _renderer);
        }

    }


    // destroy everything
    SDL_DestroyTexture(table_texture);
}


/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
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
 * RenderHouseCards: Renders cards of the house
 *
 * @param      _house           vector with the house cards
 * @param      _pos_house_hand  position of the vector _house with valid card
 *                              IDs
 * @param      _cards           vector with all loaded card images
 * @param      _renderer        renderer to handle all rendering in a window
 * @param[in]  gameHasEnded     flag to know if it is time for the house to play
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
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
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
 * \param _cards vector with all loaded card images
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
 * \param _cards vector with all loaded card images
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
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
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
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
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
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
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
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
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
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
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
