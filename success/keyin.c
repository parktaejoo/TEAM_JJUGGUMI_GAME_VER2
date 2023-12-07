#include <conio.h>
#include <stdbool.h>
#include "keyin.h"

bool is_valid(key_t key) {
	static key_t key_list[] = {
		K_UP,
		K_DOWN,
		K_LEFT,
		K_RIGHT,
		K_CONFIRM,
		K_STEAL,
		K_CONCIL,
		K_LPULL,
		K_RPULL,
		K_LLD,
		K_RLD,
		K_CHOICE,
		K_QUIT
	};

	for (int i = 0; i < N_KEY; i++) {
		if (key_list[i] == key) {
			return true;
		}
	}
	return false;
}

key_t get_key(void) {
	if (!_kbhit()) {
		return K_UNDEFINED;
	}

	int key = _getch();
	if (key == K_ARROW) {
		key = _getch();
	}

	if (is_valid(key)) {
		return key;
	}
	else {
		return K_UNDEFINED;
	}
}