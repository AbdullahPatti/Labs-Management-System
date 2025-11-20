#ifndef ROOM_H
#define ROOM_H
#include "Building.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Room {
private:
    int roomId;
    string roomNumber;
    int capacity;
    Building* building;
    
public:
    Room() : roomId(0), roomNumber(""), capacity(0), building(nullptr) {}
    Room(int rid, string rnumber, int rcapacity, Building* b) 
        : roomId(rid), roomNumber(rnumber), capacity(rcapacity), building(b) {}
    
    int getRoomId() { return roomId; }
    void setRoomId(int rid) { roomId = rid; }
    string getRoomNumber() { return roomNumber; }
    void setRoomNumber(string rnumber) { roomNumber = rnumber; }
    int getCapacity() { return capacity; }
    void setCapacity(int rcapacity) { capacity = rcapacity; }
    Building* getBuilding() { return building; }
    void setBuilding(Building* b) { building = b; }

    // Binary file handling
    void serialize(ofstream& file) {
        file.write((char*)&roomId, sizeof(roomId));
        FileHandler::writeString(file, roomNumber);
        file.write((char*)&capacity, sizeof(capacity));
        int buildingId = building ? building->getBuildingId() : -1;
        file.write((char*)&buildingId, sizeof(buildingId));
    }

    void saveToFile(const string& filename) {
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            serialize(file);
            file.close();
        }
    }
};

#endif