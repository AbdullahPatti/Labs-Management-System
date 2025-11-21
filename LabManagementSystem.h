#ifndef LABMANAGEMENTSYSTEM_H
#define LABMANAGEMENTSYSTEM_H
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

// forward declarations
class Building;
class Room;
class Instructor;
class TA;
class LabSection;
class Schedule;
class TimeSheet;
class MakeupLabs;
class Attendant;
class HOD;
class AcademicOfficer;

class LabManagementSystem {
    vector<Building*>     buildings;
    vector<Room*>         rooms;
    vector<Instructor*>   instructors;
    vector<TA*>           tas;
    vector<LabSection*>   labSections;
    vector<Schedule*>     schedules;
    vector<TimeSheet*>    timeSheets;
    vector<Attendant*>    attendants;
    vector<MakeupLabs*>   makeupLabs;
    HOD*                  hod = nullptr;
    AcademicOfficer*      academicOfficer = nullptr;

public:
    /* getters – const */
    const vector<Building*>&   getBuildings()     const { return buildings; }
    const vector<Room*>&       getRooms()         const { return rooms; }
    const vector<Instructor*>& getInstructors()   const { return instructors; }
    const vector<TA*>&         getTAs()           const { return tas; }
    const vector<LabSection*>& getLabSections()   const { return labSections; }
    const vector<Schedule*>&   getSchedules()     const { return schedules; }
    const vector<TimeSheet*>&  getTimeSheets()    const { return timeSheets; }
    const vector<Attendant*>&  getAttendants()    const { return attendants; }
    const vector<MakeupLabs*>& getMakeupLabs()    const { return makeupLabs; }
    HOD*                  getHOD()                const { return hod; }
    AcademicOfficer*      getAcademicOfficer()    const { return academicOfficer; }

    /* setters */
    void setHOD(HOD* h) { hod = h; }
    void setAcademicOfficer(AcademicOfficer* ao) { academicOfficer = ao; }

    /* adders */
    void addBuilding(Building* b)   { buildings.push_back(b); }
    void addRoom(Room* r)           { rooms.push_back(r); }
    void addInstructor(Instructor* i){ instructors.push_back(i); }
    void addTA(TA* t)               { tas.push_back(t); }
    void addLabSection(LabSection* ls){ labSections.push_back(ls); }
    void addSchedule(Schedule* s)   { schedules.push_back(s); }
    void addTimeSheet(TimeSheet* ts){ timeSheets.push_back(ts); }
    void addAttendant(Attendant* a) { attendants.push_back(a); }
    void addMakeupLab(MakeupLabs* m){ makeupLabs.push_back(m); }

    /* find helpers */
    LabSection* findLabSection(int sid) {
        for (auto* ls : labSections) if (ls->getSectionId() == sid) return ls;
        return nullptr;
    }
    Schedule* findSchedule(int schid) {
        for (auto* s : schedules) if (s->getScheduleId() == schid) return s;
        return nullptr;
    }

    /* core business functions – implemented AFTER all headers */
    void generateWeeklyScheduleReport(int weekNo);
    void generateWeeklyTimeSheetReport(const string& weekStart);
    void generateLabSemesterReport(int labSecId);
    void requestMakeupLab(int insId, int secId, const string& date, const string& reason);
    void scheduleMakeupLab(int mid, const string& day, const string& st, const string& et);
    void fillTimeSheet(int tsid, const string& st, const string& et);
};
#endif