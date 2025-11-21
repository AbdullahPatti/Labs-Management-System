#ifndef TIMESHEET_H
#define TIMESHEET_H
#include <fstream>
#include "FileHandler.h"
class LabSection;
using namespace std;

class TimeSheet {
    int         timeSheetId;
    LabSection* labSection;
    string      date;
    string      actualStart;
    string      actualEnd;
    bool        isMakeup;
public:
    TimeSheet() : timeSheetId(0), labSection(nullptr), date(""), actualStart(""), actualEnd(""), isMakeup(false) {}
    TimeSheet(int tid, LabSection* sec, string d, string s, string e, bool m)
        : timeSheetId(tid), labSection(sec), date(d), actualStart(s), actualEnd(e), isMakeup(m) {}

    int         getTimeSheetId()   const { return timeSheetId; }
    LabSection* getLabSection()    const { return labSection; }
    string      getDate()          const { return date; }
    string      getActualStartTime() const { return actualStart; }
    string      getActualEndTime()   const { return actualEnd; }
    bool        getIsMakeup()        const { return isMakeup; }

    void setActualStartTime(string s) { actualStart = s; }
    void setActualEndTime(string e)   { actualEnd = e; }

    void serialize(ofstream& f) const {
        f.write((char*)&timeSheetId, sizeof(timeSheetId));
        int sid = labSection ? labSection->getSectionId() : -1;
        f.write((char*)&sid, sizeof(sid));
        FileHandler::writeString(f, date);
        FileHandler::writeString(f, actualStart);
        FileHandler::writeString(f, actualEnd);
        f.write((char*)&isMakeup, sizeof(isMakeup));
    }
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }
};
#endif