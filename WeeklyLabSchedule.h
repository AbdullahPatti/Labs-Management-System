#ifndef WEEKLYLABSCHEDULE_H
#define WEEKLYLABSCHEDULE_H

#include "Schedule.h"
#include <vector>
#include <fstream>
#include "FileHandler.h"
using namespace std;

/**
 * WeeklyLabSchedule class represents a week's lab schedule collection
 * 
 * Responsibilities:
 * - Aggregate schedules for a specific week
 * - Provide week-based schedule organization
 * - Support weekly reporting and planning
 * 
 * Usage: Used for weekly schedule reports and planning
 */
class WeeklyLabSchedule {
private:
    int weekNumber;
    vector<Schedule*> schedules;
    
public:
    WeeklyLabSchedule() : weekNumber(0) {}
    
    /**
     * Creates a new weekly schedule container
     * @param week Week number (1-52)
     */
    WeeklyLabSchedule(int week) : weekNumber(week) {}
    
    int getWeekNumber() { return weekNumber; }
    void setWeekNumber(int week) { weekNumber = week; }
    
    /**
     * Adds a schedule to the weekly collection
     * @param schedule Pointer to Schedule object to add
     */
    void addSchedule(Schedule* schedule) { schedules.push_back(schedule); }
    
    vector<Schedule*> getSchedules() { return schedules; }

    // Binary file handling
    
    /**
     * Serializes weekly schedule data to binary file
     * @param file Output file stream for binary writing
     */
    void serialize(ofstream& file) {
        file.write((char*)&weekNumber, sizeof(weekNumber));
        int scheduleCount = schedules.size();
        file.write((char*)&scheduleCount, sizeof(scheduleCount));
        for (Schedule* schedule : schedules) {
            int scheduleId = schedule ? schedule->getScheduleId() : -1;
            file.write((char*)&scheduleId, sizeof(scheduleId));
        }
    }

    /**
     * Saves weekly schedule to binary file
     * @param filename Path to output file
     */
    void saveToFile(const string& filename) {
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            serialize(file);
            file.close();
        }
    }
};

#endif