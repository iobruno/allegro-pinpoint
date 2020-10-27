#include "datapoint.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <utility>

using namespace std;

Datapoint Datapoint::loadDataPointsFrom(std::string filepath) {
    ifstream ip(filepath);
    string cityName, countryName, posX, posY;
    stack<City*> cities;

    if (!ip.is_open())
        fprintf(stdout, "Could not load file from %s", filepath.c_str());

    while (ip.good()) {
        getline(ip, cityName, ',');
        getline(ip, countryName, ',');
        getline(ip, posX, ',');
        getline(ip, posY, '\n');
        City *city = new City(cityName, countryName, stoi(posX), stoi(posY));
        cities.push(city);
    }

    ip.close();
    return Datapoint(cities);
}

Datapoint::Datapoint(std::stack<City *> cities) {
    this->cities = std::move(cities);
}

City *Datapoint::pickRandomCity() {
    if (this->isNotEmpty()) {
        City *city = this->cities.top();
        this->cities.pop();
        return city;
    }
    return nullptr;
}

bool Datapoint::isEmpty() {
    return this->cities.empty();
}

bool Datapoint::isNotEmpty() {
    return !isEmpty();
}
