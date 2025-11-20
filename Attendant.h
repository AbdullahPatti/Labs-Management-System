#ifndef ATTENDANT_H
#define ATTENDANT_H
#include "Person.h"
#include "Building.h"
#include "TimeSheet.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Attendant : public Person {
private:
    Building* assignedBuilding;
    
public:
    Attendant() : Person(), assignedBuilding(nullptr) {}
    Attendant(int pid, string pname, string pemail, Building* building) 
        : Person(pid, pname, pemail), assignedBuilding(building) {}
    
    Building* getAssignedBuilding() { return assignedBuilding; }
    void setAssignedBuilding(Building* building) { assignedBuilding = building; }
    
    void fillTimeSheet(TimeSheet* timesheet, string startTime, string endTime) {
        timesheet->setActualStartTime(startTime);
        timesheet->setActualEndTime(endTime);
        cout << "Attendant " << name << " filled timesheet for lab." << endl;
    }

    // Binary file handling
    void serialize(ofstream& file) override {
        Person::serialize(file);
        int buildingId = assignedBuilding ? assignedBuilding->getBuildingId() : -1;
        file.write((char*)&buildingId, sizeof(buildingId));
    }

    void deserialize(ifstream& file) override {
        Person::deserialize(file);
        int buildingId;
        file.read((char*)&buildingId, sizeof(buildingId));
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