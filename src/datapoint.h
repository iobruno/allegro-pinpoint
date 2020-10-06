#ifndef ALLEGRO_PINPOINT_DATAPOINT_H
#define ALLEGRO_PINPOINT_DATAPOINT_H

#include <string>
#include <stack>
#include "city.h"

class Datapoint {

public:
    explicit Datapoint(std::stack<City*> cities);
    static Datapoint loadDataPointsFrom(std::string filepath);
    City *pickRandomCity();

private:
    std::stack<City*> cities;
};


#endif //ALLEGRO_PINPOINT_DATAPOINT_H
