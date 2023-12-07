#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>
#define PLAYER_MAX 10
#define ITEM_MAX 10

typedef struct {
    char name[100];
    int intel_buf, str_buf, stamina_buf;
} ITEM;

typedef struct {
    int id;
    char name[100];
    int intel, str, stamina;
    bool is_alive;
    bool hasitem;
    ITEM item;
} PLAYER;

PLAYER player[PLAYER_MAX];
ITEM item[ITEM_MAX];
ITEM nullItem;
int n_player, n_alive, n_item;
int tick;

void sample(void);
void mugunghwa(void);
void nightgame(void);
int randint(int low, int high);

#endif
