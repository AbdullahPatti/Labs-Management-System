#ifndef LABFULLSEMREPORT_H
#define LABFULLSEMREPORT_H

#include <vector>
#include <fstream>
#include "FileHandler.h"
class LabSection;
class TimeSheet;
using namespace std;

// Lab semester report generator - calculates contact hours and leaves
class LabFullSemReport {
    LabSection*      labSection;
    vector<TimeSheet*> sheets;
    int totalContactHours;
    int totalLeaves;
    
public:
    LabFullSemReport() : labSection(nullptr), totalContactHours(0), totalLeaves(0) {}
    LabFullSemReport(LabSection* sec) : labSection(sec), totalContactHours(0), totalLeaves(0) {}

    // Add timesheet for analysis
    void addTimeSheet(TimeSheet* ts) { sheets.push_back(ts); }

    // Calculate total contact hours from timesheets
    void calculateTotalContactHours() {
        totalContactHours = 0;
        for (auto* ts : sheets) {
            // Skip timesheets with missing time data
            if (!ts->getActualStartTime().empty() && !ts->getActualEndTime().empty()) {
                // Parse time strings and calculate duration
                size_t p1 = ts->getActualStartTime().find(':');
                size_t p2 = ts->getActualEndTime().find(':');
                if (p1 != string::npos && p2 != string::npos) {
                    // Extract hours and minutes
                    int sh = stoi(ts->getActualStartTime().substr(0, p1));
                    int sm = stoi(ts->getActualStartTime().substr(p1 + 1));
                    int eh = stoi(ts->getActualEndTime().substr(0, p2));
                    int em = stoi(ts->getActualEndTime().substr(p2 + 1));
                    // Calculate duration in minutes
                    int dur = (eh * 60 + em) - (sh * 60 + sm);
                    totalContactHours += dur / 60; // Convert to hours
                }
            }
        }
    }

    // Calculate total leaves from incomplete timesheets
    void calculateTotalLeaves() {
        totalLeaves = 0;
        for (auto* ts : sheets)
            if (ts->getActualStartTime().empty() || ts->getActualEndTime().empty())
                ++totalLeaves;
    }

    int getTotalContactHours() const { return totalContactHours; }
    int getTotalLeaves()       const { return totalLeaves; }

    // Serialize report data to binary file
    void serialize(ofstream& f) const {
        int sid = labSection ? labSection->getSectionId() : -1;
        f.write((char*)&sid, sizeof(sid));
        size_t sz = sheets.size();
        f.write((char*)&sz, sizeof(sz));
        for (auto* ts : sheets) {
            int tid = ts ? ts->getTimeSheetId() : -1;
            f.write((char*)&tid, sizeof(tid));
        }
        f.write((char*)&totalContactHours, sizeof(totalContactHours));
        f.write((char*)&totalLeaves, sizeof(totalLeaves));
    }

    // Save report to file
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }
};

#endif