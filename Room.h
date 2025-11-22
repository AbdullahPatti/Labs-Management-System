#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <fstream>
#include "FileHandler.h"
class Building;   // forward
using namespace std;

class Room {
    int       roomId;
    string    roomNumber;
    int       capacity;
    Building* building;
public:
    Room() : roomId(0), roomNumber(""), capacity(0), building(nullptr) {}
    Room(int rid, string rnum, int cap, Building* b)
        : roomId(rid), roomNumber(rnum), capacity(cap), building(b) {}

    int       getRoomId()    const { return roomId; }
    string    getRoomNumber()const { return roomNumber; }
    int       getCapacity()  const { return capacity; }
    Building* getBuilding()  const { return building; }

    void serialize(ofstream& f) const {
        f.write((char*)&roomId, sizeof(roomId));
        FileHandler::writeString(f, roomNumber);
        f.write((char*)&capacity, sizeof(capacity));
        int bid = building ? building->getBuildingId() : -1;
        f.write((char*)&bid, sizeof(bid));
    }
    
    // persistence helper
void _setBuilding(Building* b) { building = b; }

    void deserialize(ifstream& f) {
        f.read((char*)&roomId, sizeof(roomId));
        roomNumber = FileHandler::readString(f);
        f.read((char*)&capacity, sizeof(capacity));
        int bid; f.read((char*)&bid, sizeof(bid));
        // building pointer rebuilt later via id
    }
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }
};
#endif