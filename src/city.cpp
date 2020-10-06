#include "city.h"
#include <cmath>

using namespace std;

City::City(string name, int posX, int posY) {
    this->name = name;
    this->posX = posX;
    this->posY = posY;
}

City::City(string name, string country, int posX, int posY): City(name, posX, posY) {
    this->country = country;
}

int City::getPosX() const {
    return this->posX;
}

int City::getPosY() const {
    return this->posY;
}

string City::getName() {
    return this->name;
}

double City::computeDistanceFrom(int x, int y) const {
    double dist = pow((x - this->getPosX()), 2) + pow((y - this->getPosY()), 2);
    return sqrt(dist);
}
