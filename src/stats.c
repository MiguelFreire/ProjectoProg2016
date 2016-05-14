
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "players.h"
#include "gameMechanics.h"
#include "stats.h"

void writeStats(PlayersList list) {
    FILE *file = fopen(STATS_FILE_NAME, "r");
    if(file == NULL) fireFileNotFoundError(STATS_FILE_NAME);

    PlayerNode *node = list.head;

    char name[MAX_NAME_SIZE+1];
    Playertype type = 0;
    int gameStats[3] = {0};
    int playerMoney = 0;
    int houseMoney = 0;

    while(node != NULL) {


        node = node->next;
    }

}
