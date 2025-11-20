#ifndef BUILDING_H
#define BUILDING_H
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Building {
private:
    int buildingId;
    string name;
    string location;
    
public:
    Building() : buildingId(0), name(""), location("") {}
    Building(int bid, string bname, string blocation) 
        : buildingId(bid), name(bname), location(blocation) {}
    
    int getBuildingId() { return buildingId; }
    void setBuildingId(int bid) { buildingId = bid; }
    string getName() { return name; }
    void setName(string bname) { name = bname; }
    string getLocation() { return location; }
    void setLocation(string blocation) { location = blocation; }

    // Binary file handling
    void serialize(ofstream& file) {
        file.write((char*)&buildingId, sizeof(buildingId));
        FileHandler::writeString(file, name);
        FileHandler::writeString(file, location);
    }

    void deserialize(ifstream& file) {
        file.read((char*)&buildingId, sizeof(buildingId));
        name = FileHandler::readString(file);
        location = FileHandler::readString(file);
    }

    void saveToFile(const string& filename) {
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            serialize(file);
            file.close();
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (file.is_open()) {
            deserialize(file);
            file.close();
        }
    }
};

#endif