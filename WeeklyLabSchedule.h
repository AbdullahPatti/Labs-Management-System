#ifndef WEEKLYLABSCHEDULE_H
#define WEEKLYLABSCHEDULE_H
#include "Schedule.h"
#include <vector>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class WeeklyLabSchedule {
private:
    int weekNumber;
    vector<Schedule*> schedules;
    
public:
    WeeklyLabSchedule() : weekNumber(0) {}
    WeeklyLabSchedule(int week) : weekNumber(week) {}
    
    int getWeekNumber() { return weekNumber; }
    void setWeekNumber(int week) { weekNumber = week; }
    void addSchedule(Schedule* schedule) { schedules.push_back(schedule); }
    vector<Schedule*> getSchedules() { return schedules; }

    // Binary file handling
    void serialize(ofstream& file) {
        file.write((char*)&weekNumber, sizeof(weekNumber));
        int scheduleCount = schedules.size();
        file.write((char*)&scheduleCount, sizeof(scheduleCount));
        for (Schedule* schedule : schedules) {
            int scheduleId = schedule ? schedule->getScheduleId() : -1;
            file.write((char*)&scheduleId, sizeof(scheduleId));
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