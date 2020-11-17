#ifndef ALLEGRO_PINPOINT_MAIN_H
#define ALLEGRO_PINPOINT_MAIN_H

#include "city.h"

void initializeModules();
void redrawScreen();
void destroy();
void drawHUD();
void drawTimeBar();
void drawMapIcons();
double computeSecsLeft();
int computeScore(double distanceFromTarget);
void gameOver();
void gameWon();

#endif //ALLEGRO_PINPOINT_MAIN_H
