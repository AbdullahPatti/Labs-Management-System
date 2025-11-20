#ifndef LABSECTION_H
#define LABSECTION_H
#include "Instructor.h"
#include "TA.h"
#include "Room.h"
#include <string>
#include <vector>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class LabSection {
private:
    int sectionId;
    string courseCode;
    string sectionName;
    Instructor* instructor;
    vector<TA*> tas;
    Room* room;
    
public:
    LabSection() : sectionId(0), courseCode(""), sectionName(""), instructor(nullptr), room(nullptr) {}
    LabSection(int sid, string scode, string sname, Instructor* instr, Room* r) 
        : sectionId(sid), courseCode(scode), sectionName(sname), instructor(instr), room(r) {}
    
    int getSectionId() { return sectionId; }
    void setSectionId(int sid) { sectionId = sid; }
    string getCourseCode() { return courseCode; }
    void setCourseCode(string scode) { courseCode = scode; }
    string getSectionName() { return sectionName; }
    void setSectionName(string sname) { sectionName = sname; }
    Instructor* getInstructor() { return instructor; }
    void setInstructor(Instructor* instr) { instructor = instr; }
    Room* getRoom() { return room; }
    void setRoom(Room* r) { room = r; }
    
    void addTA(TA* ta) { tas.push_back(ta); }
    void removeTA(TA* ta) {
        for (auto it = tas.begin(); it != tas.end(); ++it) {
            if (*it == ta) {
                tas.erase(it);
                break;
            }
        }
    }
    vector<TA*> getTAs() { return tas; }

    // Binary file handling
    void serialize(ofstream& file) {
        file.write((char*)&sectionId, sizeof(sectionId));
        FileHandler::writeString(file, courseCode);
        FileHandler::writeString(file, sectionName);
        int instructorId = instructor ? instructor->getId() : -1;
        file.write((char*)&instructorId, sizeof(instructorId));
        int roomId = room ? room->getRoomId() : -1;
        file.write((char*)&roomId, sizeof(roomId));
        int taCount = tas.size();
        file.write((char*)&taCount, sizeof(taCount));
        for (TA* ta : tas) {
            int taId = ta ? ta->getId() : -1;
            file.write((char*)&taId, sizeof(taId));
        }
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