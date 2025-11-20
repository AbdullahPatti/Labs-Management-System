#ifndef TIMESHEET_H
#define TIMESHEET_H
#include "LabSection.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class TimeSheet {
private:
    int timeSheetId;
    LabSection* labSection;
    string date;
    string actualStartTime;
    string actualEndTime;
    bool isMakeup;
    
public:
    TimeSheet() : timeSheetId(0), labSection(nullptr), date(""), actualStartTime(""), actualEndTime(""), isMakeup(false) {}
    TimeSheet(int tid, LabSection* section, string pdate, string start, string end, bool makeup) 
        : timeSheetId(tid), labSection(section), date(pdate), actualStartTime(start), actualEndTime(end), isMakeup(makeup) {}
    
    int getTimeSheetId() { return timeSheetId; }
    void setTimeSheetId(int tid) { timeSheetId = tid; }
    LabSection* getLabSection() { return labSection; }
    void setLabSection(LabSection* section) { labSection = section; }
    string getDate() { return date; }
    void setDate(string pdate) { date = pdate; }
    string getActualStartTime() { return actualStartTime; }
    void setActualStartTime(string start) { actualStartTime = start; }
    string getActualEndTime() { return actualEndTime; }
    void setActualEndTime(string end) { actualEndTime = end; }
    bool getIsMakeup() { return isMakeup; }
    void setIsMakeup(bool makeup) { isMakeup = makeup; }

    // Binary file handling
    void serialize(ofstream& file) {
        file.write((char*)&timeSheetId, sizeof(timeSheetId));
        int sectionId = labSection ? labSection->getSectionId() : -1;
        file.write((char*)&sectionId, sizeof(sectionId));
        FileHandler::writeString(file, date);
        FileHandler::writeString(file, actualStartTime);
        FileHandler::writeString(file, actualEndTime);
        file.write((char*)&isMakeup, sizeof(isMakeup));
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