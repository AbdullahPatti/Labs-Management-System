#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <fstream>
#include "FileHandler.h"
class LabSection;
using namespace std;

class Schedule {
    int          scheduleId;
    LabSection*  labSection;
    string       dayOfWeek;
    string       startTime;
    string       endTime;
public:
    Schedule() : scheduleId(0), labSection(nullptr), dayOfWeek(""), startTime(""), endTime("") {}
    Schedule(int sid, LabSection* sec, string d, string s, string e)
        : scheduleId(sid), labSection(sec), dayOfWeek(d), startTime(s), endTime(e) {}

    int         getScheduleId()    const { return scheduleId; }
    LabSection* getLabSection()    const { return labSection; }
    string      getDayOfWeek()     const { return dayOfWeek; }
    string      getExpectedStartTime() const { return startTime; }
    string      getExpectedEndTime()   const { return endTime; }

    void serialize(ofstream& f) const {
        f.write((char*)&scheduleId, sizeof(scheduleId));
        int sid = labSection ? labSection->getSectionId() : -1;
        f.write((char*)&sid, sizeof(sid));
        FileHandler::writeString(f, dayOfWeek);
        FileHandler::writeString(f, startTime);
        FileHandler::writeString(f, endTime);
    }
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }

    void deserialize(ifstream& f) {
    f.read((char*)&scheduleId, sizeof(scheduleId));
    int sid; f.read((char*)&sid, sizeof(sid));
    labSection = nullptr; // rebuilt later
    dayOfWeek  = FileHandler::readString(f);
    startTime  = FileHandler::readString(f);
    endTime    = FileHandler::readString(f);
}
};
#endif