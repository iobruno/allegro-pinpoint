#include "datapoint.h"


Datapoint Datapoint::loadDataPointsFrom(std::string filepath) {
    return Datapoint();
}

City *Datapoint::pickRandomCity() {
    City *city = new City("RJ", "Brazil", 673, 834);
    fprintf(stdout, "Selected City: %s (%d, %d)\n", city->getName().c_str(), city->getPosX(), city->getPosY());
    return city ;
}


