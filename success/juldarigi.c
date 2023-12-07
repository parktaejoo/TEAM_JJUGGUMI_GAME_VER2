#include "juldarigi.h"

void juldarigi(void) {
	juldarigi_init();

	system("cls");
	draw();
	gotoxy(N_ROW + 4, 0);
	update_str();
	print_juldarigi_status();

	key_t key;
	str = calculate_str();

	bool gameover = false;
	while (!gameover) {
		tick++;

		if (back_buf[1][center - 1] != ' ' && back_buf[1][center + 1] != ' ') {
			key = get_key();
		}
		if (key == K_QUIT) {
			break;
		}
		else if (key == K_LPULL) {
			str--;
		}
		else if (key == K_RPULL) {
			str++;
		}
		else if (key == K_LLD) {
			char message[] = "¿ÞÂÊ ÆÀ µå·¯´¯±â ~";
			dialog(message, JULDARIGI_MAP_ROW, JULDARIGI_MAP_COL);

			lld = true;
			calculate_lying_down_str(0);
			str = calculate_str();
			update_str();

			if (str < 0) {
				gameover = move_left();
				gameover = move_left();
			}
			else {
				gameover = move_right();
				gameover = move_right();
			}

			for (int i = 0; i < n_player; i++)
				if (i % 2 == 0) {
					if (player[i].stamina > 30)
						player[i].stamina -= 30;
					else
						player[i].stamina = 0;
				}
			end_lying_down_str(0);
		}
		else if (key == K_RLD) {
			char message[] = "¿À¸¥ÂÊ ÆÀ µå·¯´¯±â ~";
			dialog(message, JULDARIGI_MAP_ROW, JULDARIGI_MAP_COL);

			rld = true;
			calculate_lying_down_str(1);
			str = calculate_str();
			update_str();

			if (str < 0) {
				gameover = move_left();
				gameover = move_left();
			}
			else {
				gameover = move_right();
				gameover = move_right();
			}

			for (int i = 0; i < n_player; i++)
				if (i % 2 == 1) {
					if (player[i].stamina > 30)
						player[i].stamina -= 30;
					else
						player[i].stamina = 0;
				}
			end_lying_down_str(1);
		}


		if (tick % 100 == 0) {
			if (lld || rld) {
				draw();
				gotoxy(N_ROW + 4, 0);
				update_str();
				print_juldarigi_status();

				Sleep(10);
				lld = false;
				rld = false;
				continue;
			}

			if (str < 0) {
				gameover = move_left();
			}
			else {
				gameover = move_right();
			}
			str = calculate_str();
		}
		;
		draw();
		gotoxy(N_ROW + 4, 0);
		update_str();
		print_juldarigi_status();

		Sleep(10);
	}
}