#ifndef BUILDING_H
#define BUILDING_H
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Building {
    int    buildingId;
    string name;
    string location;
public:
    Building() : buildingId(0), name(""), location("") {}
    Building(int bid, string bname, string bloc)
        : buildingId(bid), name(bname), location(bloc) {}

    int    getBuildingId() const { return buildingId; }
    string getName()       const { return name; }
    string getLocation()   const { return location; }

    void serialize(ofstream& f) const {
        f.write((char*)&buildingId, sizeof(buildingId));
        FileHandler::writeString(f, name);
        FileHandler::writeString(f, location);
    }
    void deserialize(ifstream& f) {
        f.read((char*)&buildingId, sizeof(buildingId));
        name     = FileHandler::readString(f);
        location = FileHandler::readString(f);
    }
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }
};
#endif