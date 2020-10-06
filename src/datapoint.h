//
// Created by Bruno Oliveira on 06/10/20.
//

#ifndef ALLEGRO_PINPOINT_DATAPOINT_H
#define ALLEGRO_PINPOINT_DATAPOINT_H

#include <string>
#include "city.h"

class Datapoint {

public:
    static Datapoint loadDataPointsFrom(std::string filepath);
    City *pickRandomCity();
};


#endif //ALLEGRO_PINPOINT_DATAPOINT_H
