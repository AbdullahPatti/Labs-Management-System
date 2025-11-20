#ifndef LABFULLSEMREPORT_H
#define LABFULLSEMREPORT_H
#include "LabSection.h"
#include "TimeSheet.h"
#include <vector>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class LabFullSemReport {
private:
    LabSection* labSection;
    vector<TimeSheet*> timeSheets;
    int totalContactHours;
    int totalLeaves;
    
public:
    LabFullSemReport() : labSection(nullptr), totalContactHours(0), totalLeaves(0) {}
    LabFullSemReport(LabSection* section) : labSection(section), totalContactHours(0), totalLeaves(0) {}
    
    LabSection* getLabSection() { return labSection; }
    void setLabSection(LabSection* section) { labSection = section; }
    void addTimeSheet(TimeSheet* timesheet) { timeSheets.push_back(timesheet); }
    vector<TimeSheet*> getTimeSheets() { return timeSheets; }
    int getTotalContactHours() { return totalContactHours; }
    void calculateTotalContactHours() { totalContactHours = timeSheets.size() * 2; }
    int getTotalLeaves() { return totalLeaves; }
    void calculateTotalLeaves() { totalLeaves = 0; }

    // Binary file handling
    void serialize(ofstream& file) {
        int sectionId = labSection ? labSection->getSectionId() : -1;
        file.write((char*)&sectionId, sizeof(sectionId));
        int timesheetCount = timeSheets.size();
        file.write((char*)&timesheetCount, sizeof(timesheetCount));
        for (TimeSheet* timesheet : timeSheets) {
            int timesheetId = timesheet ? timesheet->getTimeSheetId() : -1;
            file.write((char*)&timesheetId, sizeof(timesheetId));
        }
        file.write((char*)&totalContactHours, sizeof(totalContactHours));
        file.write((char*)&totalLeaves, sizeof(totalLeaves));
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