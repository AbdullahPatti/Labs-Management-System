#ifndef LABSECTION_H
#define LABSECTION_H
#include <vector>
#include <fstream>
#include "FileHandler.h"
class Instructor;
class TA;
class Room;
using namespace std;

class LabSection {
    int              sectionId;
    string           courseCode;
    string           sectionName;
    Instructor*      instructor;
    vector<TA*>      tas;
    Room*            room;
public:
    LabSection() : sectionId(0), courseCode(""), sectionName(""), instructor(nullptr), room(nullptr) {}
    LabSection(int sid, string ccode, string sname, Instructor* instr, Room* r)
        : sectionId(sid), courseCode(ccode), sectionName(sname), instructor(instr), room(r) {}

    int         getSectionId()   const { return sectionId; }
    string      getCourseCode()  const { return courseCode; }
    string      getSectionName() const { return sectionName; }
    Instructor* getInstructor()  const { return instructor; }
    Room*       getRoom()        const { return room; }
    vector<TA*> getTAs()         const { return tas; }
    void addTA(TA* t) { tas.push_back(t); }

    void serialize(ofstream& f) const {
        f.write((char*)&sectionId, sizeof(sectionId));
        FileHandler::writeString(f, courseCode);
        FileHandler::writeString(f, sectionName);
        int iid = instructor ? instructor->getId() : -1;
        f.write((char*)&iid, sizeof(iid));
        int rid = room ? room->getRoomId() : -1;
        f.write((char*)&rid, sizeof(rid));
        size_t sz = tas.size();
        f.write((char*)&sz, sizeof(sz));
        for (TA* t : tas) {
            int tid = t ? t->getId() : -1;
            f.write((char*)&tid, sizeof(tid));
        }
    }
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }
};
#endif