#ifndef _GAME_H_
#define _GAME_H_

#include "video_gr.h"
#include "structs.h"

int new_card(card *c,int width, int height, const char * card_paths[], int selected, int upcard, int value);
int new_deck(deck *d, int num_cards, card * cards[], int level, int posx[], int posy[]);
int load_cards();
int load_menu();
int game_handling(deck* d);
int game();
void drawDate();
void secondsToTime(int seconds, int *newMinutes, int *newSeconds);
void drawNum(int num, int x,int y);
void drawTime(int seconds);

#endif /* _GAME_H_ */
