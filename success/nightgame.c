#include "nightgame.h"

void nightgame(void) {
	nightgame_init();

	int temp_px[PLAYER_MAX];
	int temp_py[PLAYER_MAX];

	system("cls");
	draw();
	gotoxy(N_ROW + 4, 0);
	print_status2();

	key_t key;

	while (1) {
		key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			if (player[0].is_alive != false)
			{
				move_manual2(key);
				checkNear();
			}
		}

		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				if (is_arrive(i) == 1)
				{
					continue;
				}

				if (player[i].is_alive == true) {
					move_random2(i);
					checkNear();
				}
			}
		}

		find_nearest();
		drawItem();
		draw();
		gotoxy(N_ROW + 4, 0);
		print_status2();
		Sleep(10);
		tick++;
	}
}