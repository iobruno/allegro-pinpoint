#include "datapoint.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <utility>

using namespace std;

Datapoint::Datapoint(std::stack<City *> cities) {
    this->cities = std::move(cities);
}

City *Datapoint::pickRandomCity() {
    return this->cities.top();
}

Datapoint Datapoint::loadDataPointsFrom(std::string filepath) {

    ifstream ip(filepath);

    if (!ip.is_open())
        fprintf(stdout, "Could not load file from %s", filepath.c_str());

    string cityName, countryName;
    string posX, posY;

    stack<City*> cities;

    while (ip.good()) {
        getline(ip, cityName, ',');
        getline(ip, countryName, ',');
        getline(ip, posX, ',');
        getline(ip, posY, '\n');
        City *city = new City(cityName, countryName, stoi(posX), stoi(posY));
        cities.push(city);
    }

    return Datapoint(cities);
}
