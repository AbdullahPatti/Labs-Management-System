#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "LabSection.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Schedule {
private:
    int scheduleId;
    LabSection* labSection;
    string dayOfWeek;
    string expectedStartTime;
    string expectedEndTime;
    
public:
    Schedule() : scheduleId(0), labSection(nullptr), dayOfWeek(""), expectedStartTime(""), expectedEndTime("") {}
    Schedule(int sid, LabSection* section, string day, string start, string end) 
        : scheduleId(sid), labSection(section), dayOfWeek(day), expectedStartTime(start), expectedEndTime(end) {}
    
    int getScheduleId() { return scheduleId; }
    void setScheduleId(int sid) { scheduleId = sid; }
    LabSection* getLabSection() { return labSection; }
    void setLabSection(LabSection* section) { labSection = section; }
    string getDayOfWeek() { return dayOfWeek; }
    void setDayOfWeek(string day) { dayOfWeek = day; }
    string getExpectedStartTime() { return expectedStartTime; }
    void setExpectedStartTime(string start) { expectedStartTime = start; }
    string getExpectedEndTime() { return expectedEndTime; }
    void setExpectedEndTime(string end) { expectedEndTime = end; }

    // Binary file handling
    void serialize(ofstream& file) {
        file.write((char*)&scheduleId, sizeof(scheduleId));
        int sectionId = labSection ? labSection->getSectionId() : -1;
        file.write((char*)&sectionId, sizeof(sectionId));
        FileHandler::writeString(file, dayOfWeek);
        FileHandler::writeString(file, expectedStartTime);
        FileHandler::writeString(file, expectedEndTime);
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