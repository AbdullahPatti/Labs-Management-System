#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <fstream>
#include "FileHandler.h"
class Building;   // forward declaration
using namespace std;

/**
 * Room class represents physical rooms within buildings
 * 
 * Responsibilities:
 * - Store room capacity and identification data
 * - Link to parent building for location context
 * - Host lab sections and sessions
 * 
 * Relationships:
 * - Contains reference to Building (many-to-one)
 * - Hosts LabSection objects (one-to-many)
 */
class Room {
    int       roomId;
    string    roomNumber;
    int       capacity;
    Building* building;
    
public:
    Room() : roomId(0), roomNumber(""), capacity(0), building(nullptr) {}
    
    /**
     * Creates a new room record
     * @param rid Unique room identifier
     * @param rnum Room number/identifier string
     * @param cap Maximum room capacity
     * @param b Pointer to parent Building object
     */
    Room(int rid, string rnum, int cap, Building* b)
        : roomId(rid), roomNumber(rnum), capacity(cap), building(b) {}

    // Getters
    int       getRoomId()    const { return roomId; }
    string    getRoomNumber()const { return roomNumber; }
    int       getCapacity()  const { return capacity; }
    Building* getBuilding()  const { return building; }

    /**
     * Serializes room data to binary file
     * @param f Output file stream for binary writing
     */
    void serialize(ofstream& f) const {
        f.write((char*)&roomId, sizeof(roomId));
        FileHandler::writeString(f, roomNumber);
        f.write((char*)&capacity, sizeof(capacity));
        int bid = building ? building->getBuildingId() : -1;
        f.write((char*)&bid, sizeof(bid));
    }
    
    /**
     * Sets building reference during deserialization
     * Used in link reconstruction phase
     * @param b Pointer to Building object
     */
    void _setBuilding(Building* b) { building = b; }

    /**
     * Deserializes room data from binary file
     * @param f Input file stream for binary reading
     */
    void deserialize(ifstream& f) {
        f.read((char*)&roomId, sizeof(roomId));
        roomNumber = FileHandler::readString(f);
        f.read((char*)&capacity, sizeof(capacity));
        int bid; f.read((char*)&bid, sizeof(bid));
        // building pointer rebuilt later via id during link reconstruction
    }

    /**
     * Saves room data to binary file
     * @param filename Path to output file
     */
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }
};

#endif