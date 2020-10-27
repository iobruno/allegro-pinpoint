#ifndef ALLEGRO_PINPOINT_DATAPOINT_H
#define ALLEGRO_PINPOINT_DATAPOINT_H

#include <string>
#include <stack>
#include "city.h"

class Datapoint {

public:
    static Datapoint loadDataPointsFrom(std::string filepath);
    explicit Datapoint(std::stack<City*> cities);
    City* pickRandomCity();
    bool isEmpty();
    bool isNotEmpty();

private:
    std::stack<City*> cities;
};


#endif //ALLEGRO_PINPOINT_DATAPOINT_H
