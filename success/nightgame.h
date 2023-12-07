#pragma once

#include "jjuggumi.h"
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define NIGHTGAME_MAP_ROW 9
#define NIGHTGAME_MAP_COL 23

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];
int tx[PLAYER_MAX], ty[PLAYER_MAX];
int ix[ITEM_MAX], iy[ITEM_MAX];

void nightgame_init(void) {
	map_init(NIGHTGAME_MAP_ROW, NIGHTGAME_MAP_COL);
	int x, y;

	for (int i = 0; i < n_player; i++) {
		if (!player[i].is_alive)
			continue;
		do {
			px[i] = randint(1, NIGHTGAME_MAP_ROW - 2);
			py[i] = randint(1, NIGHTGAME_MAP_COL - 2);
			period[i] = randint(50, 100);
		} while (back_buf[px[i]][py[i]] != ' ');
		back_buf[px[i]][py[i]] = '0' + i;
	}

	for (int i = 0; i < n_item; i++) {
		do {
			ix[i] = randint(1, NIGHTGAME_MAP_ROW - 2);
			iy[i] = randint(1, NIGHTGAME_MAP_COL - 2);
		} while (back_buf[ix[i]][iy[i]] != ' ');
		back_buf[ix[i]][iy[i]] = 'I';
	}

	tick = 0;
}

void drawItem() {
	for (int i = 0; i < n_item; i++)
		if (ix[i] != -1 && back_buf[ix[i]][iy[i]] == ' ')
			back_buf[ix[i]][iy[i]] = 'I';
}

int isItem(int nx, int ny) {
	for (int i = 0; i < n_item; i++)
		if (ix[i] == nx && iy[i] == ny)
			return i;
	return -1;
}

void swapItem(int playerIndex, int itemIndex) {
	int p = playerIndex;

	for (int i = 0; i < n_item; i++) {
		if (!strcmp(item[i].name, player[p].item.name)) {
			ix[i] = ix[itemIndex];
			iy[i] = iy[itemIndex];
			ix[itemIndex] = -1;
			iy[itemIndex] = -1;
		}
	}
	player[p].item = item[itemIndex];
}

void checkItem(int playerIndex, int nx, int ny) {
	int p = playerIndex;
	int itemIndex = isItem(nx, ny);

	if (itemIndex != -1) {
		if (!player[p].hasitem) {
			player[p].hasitem = true;
			player[p].item = item[itemIndex];
			ix[itemIndex] = -1;
			iy[itemIndex] = -1;
		}
		else {
			if (p == 0) {
				int k = 5;
				for (int tick = 60000; tick >= 0; tick--) {
					gotoxy(13, 0);
					if (tick % 10000 == 0) {
						printf("[%d] 아이템 교환 : C ", k);
						k--;
					}
					int key = get_key();
					if (key == K_CONFIRM) {
						swapItem(0, itemIndex);
						dialog("아이템 교환 완료", NIGHTGAME_MAP_ROW, NIGHTGAME_MAP_COL);
						break;
					}
				}
				gotoxy(13, 0);
				printf("                         ");
			}
			else {
				if (randint(0, 1) == 0)
					swapItem(p, itemIndex);
			}
		}
	}
}


bool placable2(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		(back_buf[row][col] != ' ' && back_buf[row][col] != 'I')) {
		return false;
	}
	return true;
}


void move_tail(int player, int nx, int ny) {
	int p = player;
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void move_manual2(key_t key) {
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];

	if (!placable2(nx, ny)) {
		return;
	}

	checkItem(0, nx, ny);
	move_tail(0, nx, ny);

	if (player[0].is_alive == true) {
		player[0].stamina = max(0, player[0].stamina - 1);
	}
}

void move_random2(int playerIndex) {
	int p = playerIndex;
	int nx, ny;
	int move_x;
	int move_y;

	if (tx[p] != -1) {
		if (px[p] < tx[p])
			move_x = 1;
		else if (px[p] == tx[p])
			move_x = 0;
		else
			move_x = -1;

		if (py[p] < ty[p])
			move_y = 1;
		else if (py[p] == ty[p])
			move_y = 0;
		else
			move_y = -1;

		nx = px[p] + move_x;
		ny = py[p] + move_y;

		if (!placable2(nx, ny))
			return;
	}
	else {
		do {
			move_x = randint(0, 3);
			move_y = randint(0, 3);

			if (move_x == 0)
				move_x = -1;
			else if (move_x == 1)
				move_x = 1;
			else
				move_x = 0;

			if (move_y == 0)
				move_y = -1;
			else if (move_y == 1)
				move_y = 1;
			else
				move_y = 0;

			nx = px[p] + move_x;
			ny = py[p] + move_y;

		} while (!placable2(nx, ny));
	}

	checkItem(p, nx, ny);
	move_tail(p, nx, ny);

	if (player[p].is_alive == true) {
		player[p].stamina = max(0, player[p].stamina - 1);
	}
}


void find_nearest() {
	int dist = 1000;

	for (int n = 1; n < n_player; n++) {
		for (int i = 0; i < n_player; i++) {
			if (n == i)
				continue;
			if (!player[i].hasitem)
				continue;

			int d = (px[n] - px[i]) * (px[n] - px[i]) + (py[n] - py[i]) * (py[n] - py[i]);
			if (d != 0 && d < dist) {
				tx[n] = px[i];
				ty[n] = py[i];
			}
		}

		for (int i = 0; i < n_item; i++) {
			int d = (px[n] - ix[i]) * (px[n] - ix[i]) + (py[n] - ix[i]) * (py[n] - iy[i]);
			if (d != 0 && d < dist) {
				tx[n] = ix[i];
				ty[n] = iy[i];
			}
		}
	}
}

void aware(int playerIndex1, int playerIndex2) {
	int p1 = playerIndex1;
	int p2 = playerIndex2;

	int nx1 = px[p1], nx2 = px[p2];
	int ny1 = py[p1], ny2 = py[p2];

	if (nx1 + 1 == nx2) {
		if (nx1 > 0) nx1--;
		if (nx2 < NIGHTGAME_MAP_ROW - 2) nx2++;
	}
	else if (nx1 - 1 == nx2) {
		if (nx2 > 0) nx2--;
		if (nx1 < NIGHTGAME_MAP_ROW - 2) nx1++;
	}
	else if (ny1 + 1 == ny2) {
		if (ny1 > 0) ny1--;
		if (ny2 < NIGHTGAME_MAP_COL - 2) ny2++;
	}
	else if (ny1 - 1 == ny2) {
		if (ny2 > 0) ny2--;
		if (ny1 < NIGHTGAME_MAP_COL - 2) ny1++;
	}

	move_tail(p1, nx1, ny1);
	move_tail(p2, nx2, ny2);
}

void interact(int playerIndex1, int playerIndex2) {
	int p1 = playerIndex1;
	int p2 = playerIndex2;

	key_t key;
	if (p1 == 0 || p2 == 0) {
		int k = 5;
		for (int tick = 60000; tick >= 0; tick--) {
			gotoxy(13, 0);
			if (tick % 10000 == 0) {
				printf("[%d] 아이템 강탈 : a,회유하기 : s", k);
				k--;
			}
			key = get_key();
		}
		gotoxy(13, 0);
		printf("                                        ");
	}
	else {
		if (player[p1].hasitem && !player[p2].hasitem) {
			int temp = p1;
			p1 = p2;
			p2 = temp;
		}
		if (randint(0, 1) == 0)
			key = K_STEAL;
		else
			key = K_CONCIL;
	}

	if (key == K_STEAL) {
		if (player[p1].stamina == 0 || !player[p2].hasitem)
			return;
		if (player[p1].str > player[p2].str) {
			char message[] = "0 stole item from 0.";
			message[0] = p1 + '0'; message[18] = p2 + '0';

			ITEM tmp = player[p1].item;
			player[p1].item = player[p2].item;
			player[p2].item = tmp;

			if (player[p1].stamina >= 40)
				player[p1].stamina -= 40;
			else
				player[p1].stamina = 0;
			dialog(message, NIGHTGAME_MAP_ROW, NIGHTGAME_MAP_COL);
		}
		else {
			char message[] = "0 강탈 실패.";
			message[0] = p1 - '0';
			if (player[p1].stamina >= 60)
				player[p1].stamina -= 60;
			else
				player[p1].stamina = 0;
			dialog(message, NIGHTGAME_MAP_ROW, NIGHTGAME_MAP_COL);
		}
	}
	else if (key == K_CONCIL) {
		if (player[p1].stamina == 0 || !player[p2].hasitem)
			return;
		if (player[p1].str > player[p2].str) {
			char message[] = "0 conciliate item from 0.";
			message[0] = p1 + '0'; message[23] = p2 + '0';

			ITEM tmp = player[p1].item;
			player[p1].item = player[p2].item;
			player[p2].item = tmp;

			if (player[p1].stamina >= 20)
				player[p1].stamina -= 20;
			else
				player[p1].stamina = 0;
			dialog(message, NIGHTGAME_MAP_ROW, NIGHTGAME_MAP_COL);
		}
		else {
			char message[] = "0 회유 실패";
			message[0] = p1 - '0';
			if (player[p1].stamina >= 40)
				player[p1].stamina -= 40;
			else
				player[p1].stamina = 0;
			dialog(message, NIGHTGAME_MAP_ROW, NIGHTGAME_MAP_COL);
		}
	}

	aware(p1, p2);
}

bool checkNear() {
	for (int n = 0; n < n_player; n++) {
		for (int i = 0; i < n_player; i++) {
			if (n == i)
				continue;
			int d = (px[n] - px[i]) * (px[n] - px[i]) + (py[n] - py[i]) * (py[n] - py[i]);
			if (d == 1)
				interact(n, i);
		}
	}
}

void print_status2(void) {
	gotoxy(14, 0);
	printf("no. 남은 생존자들: %d\n", n_alive);
	printf("                  intl    str  stm\n");

	for (int p = 0; p < n_player; p++) {
		printf("player %2d: %5s %2d(+%d) %2d(+%d) %3d%%\n",
			p,
			player[p].is_alive ? "alive" : "DEAD",
			player[p].intel, player[p].item.intel_buf,
			player[p].str, player[p].item.str_buf,
			player[p].stamina
		);
	}
}

int is_arrive(int p) {
	if (px[p] == 2 && py[p] == 1)
	{
		return 1;
	}
	else if (px[p] == 3 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 4 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 5 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 6 && py[p] == 1)
	{
		return 1;
	}

	return 0;
}