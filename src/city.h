#ifndef ALLEGRO_PINPOINT_CITY_H
#define ALLEGRO_PINPOINT_CITY_H

#include <string>

class City {

public:
    City(std::string name, int posX, int posY);
    City(std::string name, std::string country, int posX, int posY);
    std::string getName();
    int getPosX() const;
    int getPosY() const;
    float computeDistanceFrom(int posX, int posY);

private:
    std::string name;
    std::string country;
    int posX;
    int posY;
};


#endif //ALLEGRO_PINPOINT_CITY_H
