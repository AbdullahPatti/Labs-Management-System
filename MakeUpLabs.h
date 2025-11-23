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
    // Needed for restoring links after loading
    int storedSectionId = -1;
    int storedScheduleId = -1;

    MakeupLabs()
        : makeupId(0),
          labSection(nullptr),
          requestDate(""),
          reason(""),
          makeupSchedule(nullptr),
          storedSectionId(-1),
          storedScheduleId(-1)
    {}

    MakeupLabs(int mid, LabSection* sec, string d, string r)
        : makeupId(mid),
          labSection(sec),
          requestDate(d),
          reason(r),
          makeupSchedule(nullptr),
          storedSectionId(sec ? sec->getSectionId() : -1),
          storedScheduleId(-1)
    {}

    int getMakeupId() const { return makeupId; }
    LabSection* getLabSection() const { return labSection; }
    string getRequestDate() const { return requestDate; }
    string getReason() const { return reason; }
    Schedule* getMakeupSchedule() const { return makeupSchedule; }

    void setMakeupSchedule(Schedule* s) { 
        makeupSchedule = s; 
        storedScheduleId = s ? s->getScheduleId() : -1;
    }

    void _setLabSection(LabSection* sec) { labSection = sec; }

    void serialize(ofstream& f) const {
        f.write((char*)&makeupId, sizeof(makeupId));

        int sid = labSection ? labSection->getSectionId() : -1;
        f.write((char*)&sid, sizeof(sid));

        FileHandler::writeString(f, requestDate);
        FileHandler::writeString(f, reason);

        int schid = makeupSchedule ? makeupSchedule->getScheduleId() : -1;
        f.write((char*)&schid, sizeof(schid));
    }

    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }

    void deserialize(ifstream& f) {
        f.read((char*)&makeupId, sizeof(makeupId));

        f.read((char*)&storedSectionId, sizeof(storedSectionId));
        labSection = nullptr;

        requestDate = FileHandler::readString(f);
        reason      = FileHandler::readString(f);

        f.read((char*)&storedScheduleId, sizeof(storedScheduleId));
        makeupSchedule = nullptr;
    }
};

#endif
