#ifndef OBJECTREGISTRY_H
#define OBJECTREGISTRY_H

#include <vector>
#include "Building.h"
#include "Room.h"
#include "Instructor.h"
#include "TA.h"
#include "LabSection.h"
#include "Schedule.h"
#include "TimeSheet.h"
#include "Attendant.h"
using namespace std;

class ObjectRegistry {
private:
    static ObjectRegistry* instance;
    vector<Building*> buildings;
    vector<Room*> rooms;
    vector<Instructor*> instructors;
    vector<TA*> tas;
    vector<LabSection*> labSections;
    vector<Schedule*> schedules;
    vector<TimeSheet*> timesheets;
    vector<Attendant*> attendants;

    ObjectRegistry() = default;

public:
    static ObjectRegistry& get() {
        if (!instance) instance = new ObjectRegistry();
        return *instance;
    }

    void add(Building* b) { if(b) buildings.push_back(b); }
    void add(Room* r) { if(r) rooms.push_back(r); }
    void add(Instructor* i) { if(i) instructors.push_back(i); }
    void add(TA* t) { if(t) tas.push_back(t); }
    void add(LabSection* ls) { if(ls) labSections.push_back(ls); }
    void add(Schedule* s) { if(s) schedules.push_back(s); }
    void add(TimeSheet* ts) { if(ts) timesheets.push_back(ts); }
    void add(Attendant* a) { if(a) attendants.push_back(a); }

    Building* findBuilding(int id) { for(auto b : buildings) if(b->getBuildingId() == id) return b; return nullptr; }
    Room* findRoom(int id) { for(auto r : rooms) if(r->getRoomId() == id) return r; return nullptr; }
    Instructor* findInstructor(int id) { for(auto i : instructors) if(i->getId() == id) return i; return nullptr; }
    TA* findTA(int id) { for(auto t : tas) if(t->getId() == id) return t; return nullptr; }
    LabSection* findLabSection(int id) { for(auto ls : labSections) if(ls->getSectionId() == id) return ls; return nullptr; }
    Schedule* findSchedule(int id) { for(auto s : schedules) if(s->getScheduleId() == id) return s; return nullptr; }
    TimeSheet* findTimeSheet(int id) { for(auto ts : timesheets) if(ts->getTimeSheetId() == id) return ts; return nullptr; }
    Attendant* findAttendant(int id) { for(auto a : attendants) if(a->getId() == id) return a; return nullptr; }

    void clearAll() {
        buildings.clear(); rooms.clear(); instructors.clear(); tas.clear();
        labSections.clear(); schedules.clear(); timesheets.clear(); attendants.clear();
    }

        vector<LabSection*>& getLabSections() { return labSections; }
    vector<Schedule*>& getSchedules() { return schedules; }
    vector<TimeSheet*>& getTimeSheets() { return timesheets; }
};

#endif