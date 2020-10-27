#ifndef ALLEGRO_PINPOINT_MAIN_H
#define ALLEGRO_PINPOINT_MAIN_H

#include "city.h"

void startUp();
void redrawScreen();
void destroy();
void drawHUD();
double computeSecsLeft();
int computeScore(double distanceFromTarget);
void gameOver();
void gameWon();
void displaySelectedCity(City* city);

#endif //ALLEGRO_PINPOINT_MAIN_H
