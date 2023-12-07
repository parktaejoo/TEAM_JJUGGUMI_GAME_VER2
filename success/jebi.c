#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"


char arr[PLAYER_MAX];
int pos = 0, cnt, rf, r = 1, dir, pn = 0;


void select_jebi(int p) {
    char a[20];
    if (rf == pos) {
        sprintf(a, "%d 생존자 %d 실패!", n_alive, p);
        n_alive -= 1;
        player[p].is_alive = false;
    }
    else {
        sprintf(a, "%d 생존자 %d 통과!", n_alive, p);
        arr[pos] = ' ';
        for (int i = 0; i < n_player; i++) {
            if (arr[i] == '?') {
                arr[i] = '@';
                pos = i;
                break;
            }
        }
        gotoxy(3, 2);
        for (int i = 0; i < n_alive; i++) {
            printf("%c ", arr[i]);
        }
    }
    dialog(a);
    map_init(7, 40);
    display();
    pn += 1;
    if (pn < n_player && player[pn].is_alive == false) {
        pn += 1;
    }
    if (pn >= n_player) {
        for (int i = 0; i < n_player; i++) {
            if (player[i].is_alive == true) {
                pn = i;
                break;
            }
        }
    }
}

void jebi_init() {
    system("cls");
    map_init(7, 40);
    draw();
    rf = randint(0, n_alive - 1);
    for (int i = 0; i < n_player; i++) {
        arr[i] = '?';
    }
    arr[0] = '@';
    gotoxy(3, 2);
    for (int i = 0; i < n_alive; i++) {
        printf("%c ", arr[i]);
    }
    gotoxy(7, 0);
    printf("round %d", r);
    display();
}
void player_turn(int dir, int p) {
    int t = 0;
    if (dir == 1) {
        dir = -1;
    }
    else if (dir == 2) {
        dir = 1;
    }
    if (dir == 0) {
        select_jebi(p);
        return;
    }
    else if (pos + dir < 0 || pos + dir >= n_alive) {
        return;
    }
    if (arr[pos + dir] == ' ') {
        while (1) {
            if (dir < 0) {
                for (int i = pos; i >= 0; i--) {
                    if (arr[i] == '?') {
                        arr[i] = '@';
                        arr[pos] = '?';
                        pos = i;
                        gotoxy(3, 2);
                        for (int i = 0; i < n_alive; i++) {
                            printf("%c ", arr[i]);
                        }
                        return;
                    }
                }
                t++;
            }
            else if (dir > 0) {
                for (int i = pos; i < n_player; i++) {
                    if (arr[i] == '?') {
                        arr[i] = '@';
                        arr[pos] = '?';
                        pos = i;
                        gotoxy(3, 2);
                        for (int i = 0; i < n_alive; i++) {
                            printf("%c ", arr[i]);
                        }
                        return;
                    }
                }
                t++;
            }
            if (t == 2) {
                t = 0;
                return;
            }
        }
    }
    arr[dir + pos] = '@';
    arr[pos] = '?';
    pos += dir;
    gotoxy(3, 2);
    for (int i = 0; i < n_alive; i++) {
        printf("%c ", arr[i]);
    }
}
void computer_turn() {
    for (int i = 1; i < n_player; i++) {
        gotoxy(7, 7);
        printf(", turn: player %d", pn);
        if (player[i].is_alive == true) {
            while (1) {
                int computer = randint(0, 2);
                Sleep(1000);
                player_turn(computer, i);
                Sleep(1000);
                if (computer == 0) {
                    break;
                }
            }
            if (player[i].is_alive == false) {
                break;
            }
        }
    }
}

void jebi() {
    jebi_init();
    while (1) {
        int temp = n_alive;
        if (player[0].is_alive == true && pn == 0) {
            gotoxy(7, 7);
            printf(", 차례: player %d", pn);
            key_t key = get_key();
            if (key == K_QUIT) {
                break;
            }
            else if (key != K_UNDEFINED) {
                switch (key) {
                case K_SPACE: dir = 0; break;
                case K_LEFT:  dir = 1; break;
                case K_RIGHT: dir = 2; break;
                default: break;
                }
                player_turn(dir, 0);
            }
        }
        else {
            computer_turn();
        }
        if (n_alive == 1) {
            for (int i = 0; i < n_player; i++) {
                if (player[i].is_alive == true) {
                    system("cls");
                    printf("%d 플레이어가 승리했습니다.", i);
                }
            }
            break;
        }
        if (temp != n_alive) {
            pn = 0;
            if (player[0].is_alive == false) {
                for (int i = 0; i < n_player; i++) {
                    if (player[i].is_alive == true) {
                        pn = i;
                        break;
                    }
                }
            }
            pos = 0;
            r += 1;
            jebi_init();
        }
    }
}