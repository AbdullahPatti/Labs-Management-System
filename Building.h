#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

/**
 * Building class represents physical campus buildings
 * 
 * Responsibilities:
 * - Store building location and identification data
 * - Serve as container for rooms and labs
 * - Provide building information for scheduling
 * 
 * Usage: Referenced by Room objects for location context
 */
class Building {
    int    buildingId;
    string name;
    string location;
    
public:
    Building() : buildingId(0), name(""), location("") {}
    
    /**
     * Creates a new building record
     * @param bid Unique building identifier
     * @param bname Name of the building
     * @param bloc Physical location on campus
     */
    Building(int bid, string bname, string bloc)
        : buildingId(bid), name(bname), location(bloc) {}

    // Getters
    int    getBuildingId() const { return buildingId; }
    string getName()       const { return name; }
    string getLocation()   const { return location; }

    /**
     * Serializes building data to binary file
     * @param f Output file stream for binary writing
     */
    void serialize(ofstream& f) const {
        f.write((char*)&buildingId, sizeof(buildingId));
        FileHandler::writeString(f, name);
        FileHandler::writeString(f, location);
    }

    /**
     * Deserializes building data from binary file
     * @param f Input file stream for binary reading
     */
    void deserialize(ifstream& f) {
        f.read((char*)&buildingId, sizeof(buildingId));
        name     = FileHandler::readString(f);
        location = FileHandler::readString(f);
    }

    /**
     * Saves building data to binary file
     * @param filename Path to output file
     */
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }
};

#endif