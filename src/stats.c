////////////////////////////////////////////////////////////////////////////////
//                                STATS.C                                     //
//                                                                            //
// Game stats related functions                                               //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "players.h"
#include "gameMechanics.h"
#include "errorHandling.h"
#include "stats.h"

/**
 * @brief      Writes the game stats to file
 *
 * @param      *list - a PlayerList struct
 *
 * @return     void
 */
void writeStats(PlayerList list) {
    FILE *file = fopen(STATS_FILE_NAME, "w");
    if(file == NULL) fireFileNotFoundError(STATS_FILE_NAME);

    PlayerNode *node = list.head;

    char name[MAX_NAME_SIZE+1];
    char type[6];
    int gameStats[3] = {0};
    int playerMoney = 0;
    int houseGains = 0;
    while(node != NULL) {
        strcpy(name, node->player.name);

        if(node->player.type == HUMAN) strcpy(type, "HUMAN");
        else if(node->player.type == CPU) strcpy(type, "EA");

        gameStats[0] = node->player.stats.won;  // Games Won
        gameStats[1] = node->player.stats.lost; // Games Loss
        gameStats[2] = node->player.stats.tied; // Games tied

        playerMoney = node->player.money;

        houseGains =  node->player.stats.houseGains;

        fprintf(file, "Player Name: %s\n"
                      "Player Type: %s\n"
                      "Games Won/Tied/Lost: %d/%d/%d\n"
                      "Final money: %d\n"
                      "House gains: %d\n\n",
                       name, type, gameStats[0], gameStats[2], gameStats[1],
                       playerMoney, houseGains);

        node = node->next;
    }

    fclose(file);
}
