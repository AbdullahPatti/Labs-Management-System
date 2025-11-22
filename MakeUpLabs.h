#ifndef MAKEUPLABS_H
#define MAKEUPLABS_H
#include <string>
#include <fstream>
#include "FileHandler.h"
class LabSection;
class Schedule;
using namespace std;

class MakeupLabs {
    int          makeupId;
    LabSection*  labSection;
    string       requestDate;
    string       reason;
    Schedule*    makeupSchedule;
public:
    MakeupLabs() : makeupId(0), labSection(nullptr), requestDate(""), reason(""), makeupSchedule(nullptr) {}
    MakeupLabs(int mid, LabSection* sec, string d, string r)
        : makeupId(mid), labSection(sec), requestDate(d), reason(r), makeupSchedule(nullptr) {}

    int         getMakeupId()     const { return makeupId; }
    LabSection* getLabSection()   const { return labSection; }
    string      getRequestDate()  const { return requestDate; }
    string      getReason()       const { return reason; }
    Schedule*   getMakeupSchedule() const { return makeupSchedule; }
    void        setMakeupSchedule(Schedule* s) { makeupSchedule = s; }

    void serialize(ofstream& f) const {
        f.write((char*)&makeupId, sizeof(makeupId));
        int sid = labSection ? labSection->getSectionId() : -1;
        f.write((char*)&sid, sizeof(sid));
        FileHandler::writeString(f, requestDate);
        FileHandler::writeString(f, reason);
        int msid = makeupSchedule ? makeupSchedule->getScheduleId() : -1;
        f.write((char*)&msid, sizeof(msid));
    }
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }

    void deserialize(ifstream& f) {
    f.read((char*)&makeupId, sizeof(makeupId));
    int sid; f.read((char*)&sid, sizeof(sid));
    labSection = nullptr; // rebuilt later
    requestDate = FileHandler::readString(f);
    reason      = FileHandler::readString(f);
    int msid; f.read((char*)&msid, sizeof(msid));
    makeupSchedule = nullptr; // rebuilt later
}
};
#endif