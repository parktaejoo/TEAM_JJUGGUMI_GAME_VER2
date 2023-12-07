// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
void intro(void);
void ending(void);

int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1;
	}

	fscanf_s(fp, "%d", &n_player);
	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->stamina = 100;
		p->is_alive = true;
		p->hasitem = false;
	}

	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}

	fclose(fp);
	return 0;
}

void intro(void) {
	system("cls");
	printf(" _____  _   _ ______  _   _  _____  _   _  _____ \n");
	printf("/  ___|| | | || ___ \\| | | ||_   _|| | | ||  ___|\n");
	printf("\\ `--. | | | || |_/ /| | | |  | |  | | | || |__  \n");
	printf(" `--. \\| | | ||    / | | | |  | |  | | | ||  __| \n");
	printf("/\\__/ /| |_| || |\\ \\ \\ \\_/ / _| |_ \\ \\_/ /| |___ \n");
	printf("\\____/  \\___/ \\_| \\_| \\___/  \\___/  \\___/ \\____/ \n");
	for (int i = 0; i < 3; i++) {
		gotoxy(0, 50 + (i * 4)); printf(" _ ");
		gotoxy(1, 50 + (i * 4)); printf("| |");
		gotoxy(2, 50 + (i * 4)); printf("| |");
		gotoxy(3, 50 + (i * 4)); printf("| |");
		gotoxy(4, 50 + (i * 4)); printf("|_|");
		gotoxy(5, 50 + (i * 4)); printf("(_)");
		Sleep(1000);
	}
}

void ending(void) {
	int count = 0;
	for (int i = 0; i < n_player; i++) {
		if (player[i].is_alive) {
			count++;
		}
	}
	for (int i = 0; i < n_player; i++) {
		if (count == 1) {
			if (player[i].is_alive) {
				gotoxy(5, 5); printf("---------------------");
				gotoxy(6, 5); printf("|우승자는 %d번입니다!|", i);
				gotoxy(7, 5); printf("---------------------");
			}
		}
		else {
			gotoxy(5, 5); printf(" ---------------------");
			gotoxy(6, 5); printf("| 우승자가 없습니다! |");
			gotoxy(7, 5); printf(" ---------------------");
		}
	}
	for (int i = 0; i < 13 + n_player; i++) {
		printf("\n");
	}
}

int main(void) {
	jjuggumi_init();
	intro();
	sample();
	mugunghwa();
	nightgame();
	juldarigi();
	jebi();
	ending();
	return 0;
}