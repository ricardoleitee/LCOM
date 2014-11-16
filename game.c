#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/com.h>

#include "game.h"
#include "keyboard.h"
#include "rtc.h"

#define DELAY_US    175000

int game_over = 0;

int irq_set = 0;

card a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
deck d1;

unsigned long scancode;
int j = 0;

int new_card(card *c, int width, int height, const char * card_paths[],
		int selected, int upcard, int value) {
	unsigned int i;

	c->width = width;
	c->height = height;
	c->selected = selected;
	c->upcard = upcard;
	c->value = value;

	for (i = 0; i < 11; i++) {
		c->card_paths[i] = card_paths[i];
	}
}

int new_deck(deck *d, int num_cards, card * cards[], int level, int posx[],
		int posy[]) {
	unsigned int i, j;

	d->num_cards = num_cards;
	d->level = level;

	for (j = 0; j < 11; j++) {
		d->cards[j] = cards[j];
	}

	for (i = 0; i < 10; i++) {
		d->posx[i] = posx[i];
		d->posy[i] = posy[i];
	}

}

int load_cards() {

	//Load all paths from images for each card and store them in char * arrays

	const char * card_path1[] = { "/usr/src/drivers/proj/images/image_b1.mg",
			"/usr/src/drivers/proj/images/image_b2.mg",
			"/usr/src/drivers/proj/images/image_b3.mg",
			"/usr/src/drivers/proj/images/image_b4.mg",
			"/usr/src/drivers/proj/images/image_b5.mg",
			"/usr/src/drivers/proj/images/image_b6.mg",
			"/usr/src/drivers/proj/images/image_1a.mg",
			"/usr/src/drivers/proj/images/image_1aa.mg",
			"/usr/src/drivers/proj/images/image_1aaa.mg",
			"/usr/src/drivers/proj/images/image_1b.mg",
			"/usr/src/drivers/proj/images/image_1c.mg" };

	const char * card_path2[] = { "/usr/src/drivers/proj/images/image_b1.mg",
			"/usr/src/drivers/proj/images/image_b2.mg",
			"/usr/src/drivers/proj/images/image_b3.mg",
			"/usr/src/drivers/proj/images/image_b4.mg",
			"/usr/src/drivers/proj/images/image_b5.mg",
			"/usr/src/drivers/proj/images/image_b6.mg",
			"/usr/src/drivers/proj/images/image_2a.mg",
			"/usr/src/drivers/proj/images/image_2aa.mg",
			"/usr/src/drivers/proj/images/image_2aaa.mg",
			"/usr/src/drivers/proj/images/image_2b.mg",
			"/usr/src/drivers/proj/images/image_2c.mg" };

	const char * card_path3[] = { "/usr/src/drivers/proj/images/image_b1.mg",
			"/usr/src/drivers/proj/images/image_b2.mg",
			"/usr/src/drivers/proj/images/image_b3.mg",
			"/usr/src/drivers/proj/images/image_b4.mg",
			"/usr/src/drivers/proj/images/image_b5.mg",
			"/usr/src/drivers/proj/images/image_b6.mg",
			"/usr/src/drivers/proj/images/image_3a.mg",
			"/usr/src/drivers/proj/images/image_3aa.mg",
			"/usr/src/drivers/proj/images/image_3aaa.mg",
			"/usr/src/drivers/proj/images/image_3b.mg",
			"/usr/src/drivers/proj/images/image_3c.mg" };

	const char * card_path4[] = { "/usr/src/drivers/proj/images/image_b1.mg",
			"/usr/src/drivers/proj/images/image_b2.mg",
			"/usr/src/drivers/proj/images/image_b3.mg",
			"/usr/src/drivers/proj/images/image_b4.mg",
			"/usr/src/drivers/proj/images/image_b5.mg",
			"/usr/src/drivers/proj/images/image_b6.mg",
			"/usr/src/drivers/proj/images/image_4a.mg",
			"/usr/src/drivers/proj/images/image_4aa.mg",
			"/usr/src/drivers/proj/images/image_4aaa.mg",
			"/usr/src/drivers/proj/images/image_4b.mg",
			"/usr/src/drivers/proj/images/image_4c.mg" };

	const char * card_path5[] = { "/usr/src/drivers/proj/images/image_b1.mg",
			"/usr/src/drivers/proj/images/image_b2.mg",
			"/usr/src/drivers/proj/images/image_b3.mg",
			"/usr/src/drivers/proj/images/image_b4.mg",
			"/usr/src/drivers/proj/images/image_b5.mg",
			"/usr/src/drivers/proj/images/image_b6.mg",
			"/usr/src/drivers/proj/images/image_5a.mg",
			"/usr/src/drivers/proj/images/image_5aa.mg",
			"/usr/src/drivers/proj/images/image_5aaa.mg",
			"/usr/src/drivers/proj/images/image_5b.mg",
			"/usr/src/drivers/proj/images/image_5c.mg" };

	//Create all cards
	new_card(&a1, 150, 220, card_path1, 0, 0, 1);
	new_card(&a2, 150, 220, card_path2, 0, 0, 2);
	new_card(&a3, 150, 220, card_path3, 0, 0, 3);
	new_card(&a4, 150, 220, card_path4, 0, 0, 4);
	new_card(&a5, 150, 220, card_path5, 0, 0, 5);
	new_card(&a6, 150, 220, card_path1, 0, 0, 1);
	new_card(&a7, 150, 220, card_path2, 0, 0, 2);
	new_card(&a8, 150, 220, card_path3, 0, 0, 3);
	new_card(&a9, 150, 220, card_path4, 0, 0, 4);
	new_card(&a10, 150, 220, card_path5, 0, 0, 5);

	card * cards[] = { &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, &a10 };

	//Create cards positions
	int xpos[10] = { 50, 240, 430, 620, 810, 50, 240, 430, 620, 810 };
	int ypos[10] = { 155, 155, 155, 155, 155, 415, 415, 415, 415, 415 };

	//Create new deck
	new_deck(&d1, 10, cards, 3, xpos, ypos);
}

int load_menu() {
	vg_init(0x117);
	vg_fill(0xff);
	vg_draw("/usr/src/drivers/proj/images/image_title.mg", 156, 413, 279, 107);
	vg_draw("/usr/src/drivers/proj/images/image_play.mg", 83, 460, 45, 350);
	vg_draw("/usr/src/drivers/proj/images/image_exit.mg", 90, 378, 45, 470);
	drawDate();
}


void drawDate()
{
	unsigned long day, month, year;
	getDate(&day,&month,&year);
	unsigned long i = (bcd2binary(day)) / 10;
	unsigned long ii = (bcd2binary(day)) % 10;
	unsigned long j = (bcd2binary(month)) / 10;
	unsigned long jj = (bcd2binary(month)) % 10;
	unsigned long k = (bcd2binary(year)) / 10;
	unsigned long kk = (bcd2binary(year)) % 10;
	printf("%d\n",i);
	if(i == 0)
		vg_draw("/usr/src/drivers/proj/images/image_0.mg", 22, 12, 850, 700);
	else if(i == 1)
		vg_draw("/usr/src/drivers/proj/images/image_1.mg", 22, 12, 850, 700);
	else if(i == 2)
		vg_draw("/usr/src/drivers/proj/images/image_2.mg", 22, 12, 850, 700);
	else if(i == 3)
		vg_draw("/usr/src/drivers/proj/images/image_3.mg", 22, 12, 850, 700);
	else if(i == 4)
		vg_draw("/usr/src/drivers/proj/images/image_4.mg", 22, 12, 850, 700);
	else if(i == 5)
		vg_draw("/usr/src/drivers/proj/images/image_5.mg", 22, 12, 850, 700);
	else if(i == 6)
		vg_draw("/usr/src/drivers/proj/images/image_6.mg", 22, 12, 850, 700);
	else if(i == 7)
		vg_draw("/usr/src/drivers/proj/images/image_7.mg", 22, 12, 850, 700);
	else if(i == 8)
		vg_draw("/usr/src/drivers/proj/images/image_8.mg", 22, 12, 850, 700);
	else if(i == 9)
		vg_draw("/usr/src/drivers/proj/images/image_9.mg", 22, 12, 850, 700);

	if(ii == 0)
		vg_draw("/usr/src/drivers/proj/images/image_0.mg", 22, 12, 862, 700);
	else if(ii == 1)
		vg_draw("/usr/src/drivers/proj/images/image_1.mg", 22, 12, 862, 700);
	else if(ii == 2)
		vg_draw("/usr/src/drivers/proj/images/image_2.mg", 22, 12, 862, 700);
	else if(ii == 3)
		vg_draw("/usr/src/drivers/proj/images/image_3.mg", 22, 12, 862, 700);
	else if(ii == 4)
		vg_draw("/usr/src/drivers/proj/images/image_4.mg", 22, 12, 862, 700);
	else if(ii == 5)
		vg_draw("/usr/src/drivers/proj/images/image_5.mg", 22, 12, 862, 700);
	else if(ii == 6)
		vg_draw("/usr/src/drivers/proj/images/image_6.mg", 22, 12, 862, 700);
	else if(ii == 7)
		vg_draw("/usr/src/drivers/proj/images/image_7.mg", 22, 12, 862, 700);
	else if(ii == 8)
		vg_draw("/usr/src/drivers/proj/images/image_8.mg", 22, 12, 862, 700);
	else if(ii == 9)
		vg_draw("/usr/src/drivers/proj/images/image_9.mg", 22, 12, 862, 700);

	vg_draw("/usr/src/drivers/proj/images/image_11.mg", 22, 12, 874, 700);

	if(j == 0)
		vg_draw("/usr/src/drivers/proj/images/image_0.mg", 22, 12, 886, 700);
	else if(j == 1)
		vg_draw("/usr/src/drivers/proj/images/image_1.mg", 22, 12, 886, 700);
	else if(j == 2)
		vg_draw("/usr/src/drivers/proj/images/image_2.mg", 22, 12, 886, 700);
	else if(j == 3)
		vg_draw("/usr/src/drivers/proj/images/image_3.mg", 22, 12, 886, 700);
	else if(j == 4)
		vg_draw("/usr/src/drivers/proj/images/image_4.mg", 22, 12, 886, 700);
	else if(j == 5)
		vg_draw("/usr/src/drivers/proj/images/image_5.mg", 22, 12, 886, 700);
	else if(j == 6)
		vg_draw("/usr/src/drivers/proj/images/image_6.mg", 22, 12, 886, 700);
	else if(j == 7)
		vg_draw("/usr/src/drivers/proj/images/image_7.mg", 22, 12, 886, 700);
	else if(j == 8)
		vg_draw("/usr/src/drivers/proj/images/image_8.mg", 22, 12, 886, 700);
	else if(j == 9)
		vg_draw("/usr/src/drivers/proj/images/image_9.mg", 22, 12, 886, 700);

	if(jj == 0)
		vg_draw("/usr/src/drivers/proj/images/image_0.mg", 22, 12, 898, 700);
	else if(jj == 1)
		vg_draw("/usr/src/drivers/proj/images/image_1.mg", 22, 12, 898, 700);
	else if(jj == 2)
		vg_draw("/usr/src/drivers/proj/images/image_2.mg", 22, 12, 898, 700);
	else if(jj == 3)
		vg_draw("/usr/src/drivers/proj/images/image_3.mg", 22, 12, 898, 700);
	else if(jj == 4)
		vg_draw("/usr/src/drivers/proj/images/image_4.mg", 22, 12, 898, 700);
	else if(jj == 5)
		vg_draw("/usr/src/drivers/proj/images/image_5.mg", 22, 12, 898, 700);
	else if(jj == 6)
		vg_draw("/usr/src/drivers/proj/images/image_6.mg", 22, 12, 898, 700);
	else if(jj == 7)
		vg_draw("/usr/src/drivers/proj/images/image_7.mg", 22, 12, 898, 700);
	else if(jj == 8)
		vg_draw("/usr/src/drivers/proj/images/image_8.mg", 22, 12, 898, 700);
	else if(jj == 9)
		vg_draw("/usr/src/drivers/proj/images/image_9.mg", 22, 12, 898, 700);

	vg_draw("/usr/src/drivers/proj/images/image_11.mg", 22, 12, 910, 700);

	if(k == 0)
		vg_draw("/usr/src/drivers/proj/images/image_0.mg", 22, 12, 922, 700);
	else if(k == 1)
		vg_draw("/usr/src/drivers/proj/images/image_1.mg", 22, 12, 922, 700);
	else if(k == 2)
		vg_draw("/usr/src/drivers/proj/images/image_2.mg", 22, 12, 922, 700);
	else if(k == 3)
		vg_draw("/usr/src/drivers/proj/images/image_3.mg", 22, 12, 922, 700);
	else if(k == 4)
		vg_draw("/usr/src/drivers/proj/images/image_4.mg", 22, 12, 922, 700);
	else if(k == 5)
		vg_draw("/usr/src/drivers/proj/images/image_5.mg", 22, 12, 922, 700);
	else if(k == 6)
		vg_draw("/usr/src/drivers/proj/images/image_6.mg", 22, 12, 922, 700);
	else if(k == 7)
		vg_draw("/usr/src/drivers/proj/images/image_7.mg", 22, 12, 922, 700);
	else if(k == 8)
		vg_draw("/usr/src/drivers/proj/images/image_8.mg", 22, 12, 922, 700);
	else if(k == 9)
		vg_draw("/usr/src/drivers/proj/images/image_9.mg", 22, 12, 922, 700);

	if(kk == 0)
		vg_draw("/usr/src/drivers/proj/images/image_0.mg", 22, 12, 934, 700);
	else if(kk == 1)
		vg_draw("/usr/src/drivers/proj/images/image_1.mg", 22, 12, 934, 700);
	else if(kk == 2)
		vg_draw("/usr/src/drivers/proj/images/image_2.mg", 22, 12, 934, 700);
	else if(kk == 3)
		vg_draw("/usr/src/drivers/proj/images/image_3.mg", 22, 12, 934, 700);
	else if(kk == 4)
		vg_draw("/usr/src/drivers/proj/images/image_4.mg", 22, 12, 934, 700);
	else if(kk == 5)
		vg_draw("/usr/src/drivers/proj/images/image_5.mg", 22, 12, 934, 700);
	else if(kk == 6)
		vg_draw("/usr/src/drivers/proj/images/image_6.mg", 22, 12, 934, 700);
	else if(kk == 7)
		vg_draw("/usr/src/drivers/proj/images/image_7.mg", 22, 12, 934, 700);
	else if(kk == 8)
		vg_draw("/usr/src/drivers/proj/images/image_8.mg", 22, 12, 934, 700);
	else if(kk == 9)
		vg_draw("/usr/src/drivers/proj/images/image_9.mg", 22, 12, 934, 700);
}

void secondsToTime(int seconds, int *newMinutes, int *newSeconds)
{
	*newMinutes = seconds / 60;
	*newSeconds = seconds % 60;
}

void drawNum(int num, int x,int y)
{
	switch (num)
	{
	case 0:
		vg_draw("/usr/src/drivers/proj/images/image_0.mg", 22, 12, x, y);
		break;
	case 1:
		vg_draw("/usr/src/drivers/proj/images/image_1.mg", 22, 12, x, y);
		break;
	case 2:
		vg_draw("/usr/src/drivers/proj/images/image_2.mg", 22, 12, x, y);
		break;
	case 3:
		vg_draw("/usr/src/drivers/proj/images/image_3.mg", 22, 12, x, y);
		break;
	case 4:
		vg_draw("/usr/src/drivers/proj/images/image_4.mg", 22, 12, x, y);
		break;
	case 5:
		vg_draw("/usr/src/drivers/proj/images/image_5.mg", 22, 12, x, y);
		break;
	case 6:
		vg_draw("/usr/src/drivers/proj/images/image_6.mg", 22, 12, x, y);
		break;
	case 7:
		vg_draw("/usr/src/drivers/proj/images/image_7.mg", 22, 12, x, y);
		break;
	case 8:
		vg_draw("/usr/src/drivers/proj/images/image_8.mg", 22, 12, x, y);
		break;
	case 9:
		vg_draw("/usr/src/drivers/proj/images/image_9.mg", 22, 12, x, y);
		break;
	}
}

void drawTime(int seconds)
{
	int newSeconds;
	int newMinutes;

	secondsToTime(seconds,&newSeconds,&newMinutes);

	drawNum(newSeconds/10, 926, 35);
	drawNum(newSeconds%10, 938, 35);
	vg_draw("/usr/src/drivers/proj/images/image_10.mg", 22, 8, 950, 35);
	drawNum(newMinutes/10, 958, 35);
	drawNum(newMinutes%10, 970, 35);
}

int game_handling(deck* d) {
	int i;
	int turned = 0;

	if (scancode == 0x9e) {
		for (i = 0; i < 11; i++) {
			vg_draw(d->cards[j]->card_paths[i], d->cards[j]->height,
					d->cards[j]->width, d->posx[j], d->posy[j]);
			tickdelay(micros_to_ticks(DELAY_US));
		}
		d->cards[j]->selected = 1;
	}

	else if (scancode == 0x83 || scancode == 0x81) {
		game_over = 1;
		vg_exit();
	}

	//Press right
	else if (scancode == 0xcd) {
		if (d->cards[j]->selected == 0) {
			vg_draw(d->cards[j]->card_paths[0], d->cards[j]->height,
					d->cards[j]->width, d->posx[j], d->posy[j]);
		}
		j++;

		do {
			if (j == d->num_cards) {
				j = 0;
			}
			if (d->cards[j]->selected == 1)
				j++;
		} while (d->cards[j]->selected == 1);

		vg_draw("/usr/src/drivers/proj/images/image_selected.mg",
				d->cards[j]->height, d->cards[j]->width, d->posx[j],
				d->posy[j]);
	}

	//Press left
	else if (scancode == 0xcb) {
		if (d->cards[j]->selected == 0) {
			vg_draw(d->cards[j]->card_paths[0], d->cards[j]->height,
					d->cards[j]->width, d->posx[j], d->posy[j]);
		}
		j--;

		do {
			if (j == -1) {
				j = d->num_cards - 1;
			}
			if (d->cards[j]->selected == 1)
				j--;
		} while (d->cards[j]->selected == 1);

		vg_draw("/usr/src/drivers/proj/images/image_selected.mg",
				d->cards[j]->height, d->cards[j]->width, d->posx[j],
				d->posy[j]);
	}

	//Press up
	else if (scancode == 0xc8) {
		if (j >= d->num_cards / 2) {
			if (d->cards[j]->selected == 0) {
				vg_draw(d->cards[j]->card_paths[0], d->cards[j]->height,
						d->cards[j]->width, d->posx[j], d->posy[j]);
			}
			j -= 5;

			if (d->cards[j]->selected == 0) {
				vg_draw("/usr/src/drivers/proj/images/image_selected.mg",
						d->cards[j]->height, d->cards[j]->width, d->posx[j],
						d->posy[j]);
			}
		}
	}

	//Press down
	else if (scancode == 0xd0) {
		if (j < d->num_cards / 2) {
			if (d->cards[j]->selected == 0) {
			vg_draw(d->cards[j]->card_paths[0], d->cards[j]->height,
					d->cards[j]->width, d->posx[j], d->posy[j]);
			}
			j += 5;

			if (d->cards[j]->selected == 0) {
				vg_draw("/usr/src/drivers/proj/images/image_selected.mg",
						d->cards[j]->height, d->cards[j]->width, d->posx[j],
						d->posy[j]);
			}
		}
	}

}

int game() {

	int menu = 0;

	load_cards();
	load_menu();

	if ((irq_set = keyboard_subscribe_int()) == 1) {
		vg_exit();
		return 1;
	}

	while (menu == 0) {
		if (keyboard_int_handler(irq_set) == 0) {
			if (scancode == 0x82) {
				menu = 1;
				vg_fill(0xff);
				vg_draw("/usr/src/drivers/proj/images/image_nivel3.mg", 64, 167,
						428, 36);
				vg_draw_all(&d1, 134, 180);
				vg_draw("/usr/src/drivers/proj/images/image_selected.mg",
						d1.cards[0]->height, d1.cards[0]->width, d1.posx[0],
						d1.posy[0]);
			} else {
				if (scancode == 0x83) {
					vg_exit();
					return 0;
				}
			}
		}
	}

	while (game_over == 0) {
		if (keyboard_int_handler(irq_set) == 0) {
			game_handling(&d1);
		}
	}

	if (keyboard_unsubscribe_int() != 0) {
		return 1;
	}

	sleep(5);
	vg_exit();
	return 0;
}
