#ifndef MAKEUPLABS_H
#define MAKEUPLABS_H
#include "LabSection.h"
#include "Schedule.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class MakeupLabs {
private:
    int makeupId;
    LabSection* labSection;
    string requestDate;
    string reason;
    Schedule* makeupSchedule;
    
public:
    MakeupLabs() : makeupId(0), labSection(nullptr), requestDate(""), reason(""), makeupSchedule(nullptr) {}
    MakeupLabs(int mid, LabSection* section, string date, string preason) 
        : makeupId(mid), labSection(section), requestDate(date), reason(preason), makeupSchedule(nullptr) {}

    // Getters and setters...
    int getMakeupId() const { return makeupId; }
    LabSection* getLabSection() const { return labSection; }
    string getRequestDate() const { return requestDate; }
    string getReason() const { return reason; }
    Schedule* getMakeupSchedule() const { return makeupSchedule; }
    void setMakeupSchedule(Schedule* s) { makeupSchedule = s; }

    void serialize(ofstream& file) const {
        file.write((char*)&makeupId, sizeof(makeupId));
        int sectionId = labSection ? labSection->getSectionId() : -1;
        file.write((char*)&sectionId, sizeof(sectionId));
        FileHandler::writeString(file, requestDate);
        FileHandler::writeString(file, reason);
        int scheduleId = makeupSchedule ? makeupSchedule->getScheduleId() : -1;
        file.write((char*)&scheduleId, sizeof(scheduleId));
    }

    void deserialize(ifstream& file) {
        file.read((char*)&makeupId, sizeof(makeupId));
        int sectionId;
        file.read((char*)&sectionId, sizeof(sectionId));
        // Note: pointer reconstruction not done here — needs object registry in real system
        requestDate = FileHandler::readString(file);
        reason = FileHandler::readString(file);
        int scheduleId;
        file.read((char*)&scheduleId, sizeof(scheduleId));
        // makeupSchedule reconstruction skipped for simplicity
    }

    void saveToFile(const string& filename) const {
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