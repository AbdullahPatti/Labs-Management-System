/******************************************************************************************
 *  University Lab Management System - COMPLETE IMPLEMENTATION
 *
 *  Compile:  g++ -std=c++17 Main.cpp -o LabSys
 *  Run:      ./LabSys
 ******************************************************************************************/
#ifdef _WIN32
#   include <direct.h>
#   define MKDIR(dir) _mkdir(dir)
#else
#   include <sys/stat.h>
#   define MKDIR(dir) mkdir(dir, 0755)
#endif

#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include "Person.h"
#include "Instructor.h"
#include "TA.h"
#include "Building.h"
#include "Room.h"
#include "LabSection.h"
#include "Schedule.h"
#include "TimeSheet.h"
#include "Attendant.h"
#include "HOD.h"
#include "AcademicOfficer.h"
#include "MakeUpLabs.h"
#include "LabFullSemReport.h"
#include "LabManagementSystem.h"
#include "FileHandler.h"

using namespace std;

/* ----------------  AcademicOfficer method definitions  ---------------- */
void AcademicOfficer::populateLabData() {
    if (!system) { cout << "System not linked.\n"; return; }
    cout << "Populating default schedules & skeleton timesheets...\n";
    int schId = 10000, tsId = 20000;
    for (auto* sec : system->getLabSections()) {
        auto* sch = new Schedule(++schId, sec, "Monday", "14:00", "16:00");
        system->addSchedule(sch);
        auto* ts = new TimeSheet(++tsId, sec, "2025-11-24", "", "", false);
        system->addTimeSheet(ts);
    }
}

void AcademicOfficer::scheduleMakeupLab(int mid, const string& day, const string& st, const string& et) {
    if (!system) { cout << "System not linked.\n"; return; }
    system->scheduleMakeupLab(mid, day, st, et);
}

/* ----------------  HOD method definitions  ---------------- */
void HOD::generateWeeklyScheduleReport(int weekNo) {
    if (!system) { cout << "System not linked.\n"; return; }
    system->generateWeeklyScheduleReport(weekNo);
}

void HOD::generateWeeklyTimeSheetReport(const string& ws) {
    if (!system) { cout << "System not linked.\n"; return; }
    system->generateWeeklyTimeSheetReport(ws);
}

void HOD::generateLabSemesterReport(int lsid) {
    if (!system) { cout << "System not linked.\n"; return; }
    system->generateLabSemesterReport(lsid);
}

/* --------------  LabManagementSystem method definitions  -------------- */
void LabManagementSystem::generateWeeklyScheduleReport(int weekNo) {
    cout << "\n========== WEEKLY SCHEDULE - WEEK " << weekNo << " ==========\n";
    cout << left << setw(12) << "Course" << setw(12) << "Section" << setw(10) << "Day"
         << setw(12) << "Start" << setw(12) << "End" << setw(15) << "Instructor" << "\n";
    for (auto* s : schedules) {
        auto* sec = s->getLabSection();
        if (!sec) continue;
        cout << setw(12) << sec->getCourseCode()
             << setw(12) << sec->getSectionName()
             << setw(10) << s->getDayOfWeek()
             << setw(12) << s->getExpectedStartTime()
             << setw(12) << s->getExpectedEndTime()
             << setw(15) << (sec->getInstructor() ? sec->getInstructor()->getName() : "N/A") << "\n";
    }
    cout << string(70, '=') << "\n";
}

void LabManagementSystem::generateWeeklyTimeSheetReport(const string& weekStart) {
    cout << "\n========== TIMESHEET WEEK STARTING " << weekStart << " ==========\n";
    for (auto* ts : timeSheets) {
        auto* sec = ts->getLabSection();
        if (!sec) continue;
        cout << sec->getCourseCode() << "-" << sec->getSectionName()
             << "  " << ts->getDate()
             << "  scheduled: " << ts->getActualStartTime() << "-" << ts->getActualEndTime()
             << "  makeup: " << (ts->getIsMakeup() ? "YES" : "NO") << "\n";
    }
    cout << string(70, '=') << "\n";
}

void LabManagementSystem::generateLabSemesterReport(int labSecId) {
    auto* sec = findLabSection(labSecId);
    if (!sec) { cout << "Lab section not found.\n"; return; }
    cout << "\n========== SEMESTER REPORT  " << sec->getCourseCode()
         << " - " << sec->getSectionName() << " ==========\n";
    LabFullSemReport rep(sec);
    for (auto* ts : timeSheets)
        if (ts->getLabSection() == sec) rep.addTimeSheet(ts);
    rep.calculateTotalContactHours();
    rep.calculateTotalLeaves();
    cout << "Total contact hours: " << rep.getTotalContactHours() << "\n";
    cout << "Total leaves: " << rep.getTotalLeaves() << "\n";
    cout << string(70, '=') << "\n";
}

void LabManagementSystem::requestMakeupLab(int insId, int secId, const string& date, const string& reason) {
    Instructor* instr = nullptr;
    for (auto* i : instructors) if (i->getId() == insId) { instr = i; break; }
    if (!instr) { cout << "Instructor not found.\n"; return; }
    auto* sec = findLabSection(secId);
    if (!sec) { cout << "Section not found.\n"; return; }
    if (sec->getInstructor() != instr) { cout << "Instructor not assigned to this section.\n"; return; }
    static int mid = 7000;
    auto* m = new MakeupLabs(++mid, sec, date, reason);
    addMakeupLab(m);
    cout << "Makeup request created ID=" << mid << "\n";
}

void LabManagementSystem::scheduleMakeupLab(int mid, const string& day, const string& st, const string& et) {
    MakeupLabs* m = nullptr;
    for (auto* x : makeupLabs) if (x->getMakeupId() == mid) { m = x; break; }
    if (!m) { cout << "Makeup request not found.\n"; return; }
    
    auto* sec = m->getLabSection();
    
    // Check for conflicts
    bool conflict = false;
    for (auto* sch : schedules) {
        if (sch->getDayOfWeek() == day && sch->getLabSection()->getRoom() == sec->getRoom()) {
            if (sch->getExpectedStartTime() == st) {
                cout << "Conflict detected: Room " << sec->getRoom()->getRoomNumber() 
                     << " is already booked at this time.\n";
                conflict = true;
                break;
            }
        }
    }
    
    if (conflict) {
        cout << "Cannot schedule makeup lab due to conflict.\n";
        return;
    }
    
    static int schId = 8000, tsId = 9000;
    auto* sch = new Schedule(++schId, sec, day, st, et);
    addSchedule(sch);
    m->setMakeupSchedule(sch);
    auto* ts = new TimeSheet(++tsId, sec, m->getRequestDate(), "", "", true);
    addTimeSheet(ts);
    cout << "Makeup lab scheduled successfully.\n";
}

void LabManagementSystem::fillTimeSheet(int tsid, const string& st, const string& et) {
    for (auto* ts : timeSheets)
        if (ts->getTimeSheetId() == tsid) {
            ts->setActualStartTime(st);
            ts->setActualEndTime(et);
            cout << "Timesheet filled.\n";
            return;
        }
    cout << "Timesheet not found.\n";
}

/* ---------- Console helpers ---------- */
void clearScr() { 
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScr() { 
    cout << "\nPress ENTER to continue..."; 
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
    cin.get();
}

template<typename T>
T read(string prompt, T low, T high) {
    T x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= low && x <= high) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cin.clear(); 
        cin.ignore(256,'\n');
        cout << "Invalid input. Try again.\n";
    }
}

/* ---------- Global singleton ---------- */
LabManagementSystem LMS;

/* ============================================================
                      BINARY PERSISTENCE
   ============================================================ */
static const string BASE_DIR = "data/";

template<typename T>
void saveVector(const vector<T*>& vec, const string& fname) {
    ofstream ofs(BASE_DIR + fname, ios::binary);
    if (!ofs) return;
    size_t sz = vec.size();
    ofs.write((char*)&sz, sizeof(sz));
    for (auto* p : vec) p->serialize(ofs);
}

template<typename T>
void loadVector(vector<T*>& vec, const string& fname) {
    ifstream ifs(BASE_DIR + fname, ios::binary);
    if (!ifs) return;
    size_t sz; ifs.read((char*)&sz, sizeof(sz));
    vec.clear(); vec.reserve(sz);
    for (size_t i = 0; i < sz; ++i) {
        auto* t = new T(); t->deserialize(ifs); vec.push_back(t);
    }
}

void rebuildLinks() {
    auto readInt = [](ifstream& f) -> int {
        int v; f.read((char*)&v, sizeof(v)); return v;
    };

    auto& buildings = const_cast<vector<Building*>&>(LMS.getBuildings());
    auto& rooms = const_cast<vector<Room*>&>(LMS.getRooms());
    auto& sections = const_cast<vector<LabSection*>&>(LMS.getLabSections());
    auto& instructors = const_cast<vector<Instructor*>&>(LMS.getInstructors());
    auto& tas = const_cast<vector<TA*>&>(LMS.getTAs());
    auto& attendants = const_cast<vector<Attendant*>&>(LMS.getAttendants());

    for (auto* r : rooms) {
        ifstream ifs(BASE_DIR + "buildings.dat", ios::binary);
        size_t sz; ifs.read((char*)&sz, sizeof(sz));
        for (size_t i = 0; i < sz; ++i) {
            int bid = readInt(ifs);
            string name = FileHandler::readString(ifs);
            string loc = FileHandler::readString(ifs);
            for (auto* b : buildings)
                if (b->getBuildingId() == bid) { r->_setBuilding(b); break; }
        }
    }

    for (auto* ls : sections) {
        ifstream ifs(BASE_DIR + "labsections.dat", ios::binary);
        size_t sz; ifs.read((char*)&sz, sizeof(sz));
        for (size_t i = 0; i < sz; ++i) {
            int sid = readInt(ifs);
            string cc = FileHandler::readString(ifs);
            string sn = FileHandler::readString(ifs);
            int iid = readInt(ifs);
            int rid = readInt(ifs);
            size_t taCnt; ifs.read((char*)&taCnt, sizeof(taCnt));

            if (sid != ls->getSectionId()) {
                for (size_t j = 0; j < taCnt; ++j) readInt(ifs);
                continue;
            }
            
            for (auto* in : instructors)
                if (in->getId() == iid) { ls->_setInstructor(in); break; }
            for (auto* ro : rooms)
                if (ro->getRoomId() == rid) { ls->_setRoom(ro); break; }
            ls->_clearTAs();
            for (size_t j = 0; j < taCnt; ++j) {
                int tid = readInt(ifs);
                for (auto* ta : tas)
                    if (ta->getId() == tid) { ls->_addTA(ta); break; }
            }
            break;
        }
    }

    for (auto* a : attendants) {
        ifstream ifs(BASE_DIR + "attendants.dat", ios::binary);
        size_t sz; ifs.read((char*)&sz, sizeof(sz));
        for (size_t i = 0; i < sz; ++i) {
            int aid = readInt(ifs);
            string nm = FileHandler::readString(ifs);
            string em = FileHandler::readString(ifs);
            int bid = readInt(ifs);
            if (aid == a->getId()) {
                for (auto* b : buildings)
                    if (b->getBuildingId() == bid) { a->_setBuilding(b); break; }
                break;
            }
        }
    }

    if (LMS.getHOD()) const_cast<HOD*>(LMS.getHOD())->setSystem(&LMS);
    if (LMS.getAcademicOfficer()) const_cast<AcademicOfficer*>(LMS.getAcademicOfficer())->setSystem(&LMS);
}

void savePersistent() {
    cout << "Saving persistent state...\n";
    MKDIR(BASE_DIR.c_str());
    
    saveVector(LMS.getBuildings(), "buildings.dat");
    saveVector(LMS.getRooms(), "rooms.dat");
    saveVector(LMS.getInstructors(), "instructors.dat");
    saveVector(LMS.getTAs(), "tas.dat");
    saveVector(LMS.getLabSections(), "labsections.dat");
    saveVector(LMS.getSchedules(), "schedules.dat");
    saveVector(LMS.getTimeSheets(), "timesheets.dat");
    saveVector(LMS.getAttendants(), "attendants.dat");
    saveVector(LMS.getMakeupLabs(), "makeuplabs.dat");
    
    ofstream ofs(BASE_DIR + "officers.dat", ios::binary);
    size_t sz = 2;
    ofs.write((char*)&sz, sizeof(sz));
    if (LMS.getHOD()) LMS.getHOD()->serialize(ofs);
    if (LMS.getAcademicOfficer()) LMS.getAcademicOfficer()->serialize(ofs);
}

void loadPersistent() {
    cout << "Loading persistent state...\n";
    loadVector<Building>(const_cast<vector<Building*>&>(LMS.getBuildings()), "buildings.dat");
    loadVector<Room>(const_cast<vector<Room*>&>(LMS.getRooms()), "rooms.dat");
    loadVector<Instructor>(const_cast<vector<Instructor*>&>(LMS.getInstructors()), "instructors.dat");
    loadVector<TA>(const_cast<vector<TA*>&>(LMS.getTAs()), "tas.dat");
    loadVector<LabSection>(const_cast<vector<LabSection*>&>(LMS.getLabSections()), "labsections.dat");
    loadVector<Schedule>(const_cast<vector<Schedule*>&>(LMS.getSchedules()), "schedules.dat");
    loadVector<TimeSheet>(const_cast<vector<TimeSheet*>&>(LMS.getTimeSheets()), "timesheets.dat");
    loadVector<Attendant>(const_cast<vector<Attendant*>&>(LMS.getAttendants()), "attendants.dat");
    loadVector<MakeupLabs>(const_cast<vector<MakeupLabs*>&>(LMS.getMakeupLabs()), "makeuplabs.dat");
    
    ifstream ifs(BASE_DIR + "officers.dat", ios::binary);
    if (ifs) {
        size_t sz; ifs.read((char*)&sz, sizeof(sz));
        if (sz >= 1) {
            HOD* h = new HOD(); h->deserialize(ifs); LMS.setHOD(h);
        }
        if (sz >= 2) {
            AcademicOfficer* ao = new AcademicOfficer(); ao->deserialize(ifs); LMS.setAcademicOfficer(ao);
        }
    }
    rebuildLinks();
}

void seedData() {
    cout << "Seeding master data...\n";

    Building* csB = new Building(1,"Computer-Science Block","Main Campus");
    Building* eeB = new Building(2,"Electrical Block","North Campus");
    LMS.addBuilding(csB); LMS.addBuilding(eeB);

    Room* r1 = new Room(101,"CS-Lab-1",35,csB);
    Room* r2 = new Room(102,"CS-Lab-2",30,csB);
    Room* r3 = new Room(201,"EE-Lab-1",40,eeB);
    LMS.addRoom(r1); LMS.addRoom(r2); LMS.addRoom(r3);

    Instructor* i1 = new Instructor(1001,"Dr. Ahmed Khan","ahmed@uni.edu","CS","CS-101");
    Instructor* i2 = new Instructor(1002,"Dr. Fatima Ali","fatima@uni.edu","CS","CS-102");
    Instructor* i3 = new Instructor(1003,"Dr. Hassan Malik","hassan@uni.edu","EE","EE-201");
    LMS.addInstructor(i1); LMS.addInstructor(i2); LMS.addInstructor(i3);

    TA* t1 = new TA(2001,"Sara Ali","sara@student.edu","BSCS001","BSCS");
    TA* t2 = new TA(2002,"Bilal Ahmed","bilal@student.edu","BSCS002","BSCS");
    TA* t3 = new TA(2003,"Ayesha Khan","ayesha@student.edu","BSCS003","BSCS");
    TA* t4 = new TA(2004,"Usman Malik","usman@student.edu","BSEE001","BSEE");
    LMS.addTA(t1); LMS.addTA(t2); LMS.addTA(t3); LMS.addTA(t4);

    LabSection* ls1 = new LabSection(1,"CS-101L","Prog-Fundamentals",i1,r1);
    LabSection* ls2 = new LabSection(2,"CS-201L","Data-Structures",i2,r2);
    LabSection* ls3 = new LabSection(3,"EE-301L","Digital-Logic",i3,r3);
    ls1->addTA(t1); ls1->addTA(t2); ls2->addTA(t3); ls3->addTA(t4);
    LMS.addLabSection(ls1); LMS.addLabSection(ls2); LMS.addLabSection(ls3);

    int sid=3001, tid=4001;
    auto add=[&](LabSection* sec, string day, string s, string e){
        Schedule* sch = new Schedule(sid++,sec,day,s,e);
        LMS.addSchedule(sch);
        TimeSheet* ts = new TimeSheet(tid++,sec,"2025-11-24","", "", false);
        LMS.addTimeSheet(ts);
    };
    add(ls1,"Monday","14:00","16:00");
    add(ls2,"Tuesday","10:00","12:00");
    add(ls3,"Wednesday","14:00","16:00");
    add(ls1,"Thursday","14:00","16:00");

    AcademicOfficer* ao = new AcademicOfficer(5001,"Academic Admin","admin@uni.edu","Academic");
    ao->setSystem(&LMS); LMS.setAcademicOfficer(ao);

    HOD* hod = new HOD(4001,"Prof. Dr. Naveed","naveed@uni.edu","CS");
    hod->setSystem(&LMS); LMS.setHOD(hod);

    Attendant* at1 = new Attendant(6001,"Usman Attendant","usman@uni.edu",csB);
    Attendant* at2 = new Attendant(6002,"Ali Attendant","ali@uni.edu",eeB);
    LMS.addAttendant(at1); LMS.addAttendant(at2);

    cout << "Master data seeded.\n";
}

/* ============================================================
                          MENU FUNCTIONS
   ============================================================ */

void addBuildingMenu() {
    clearScr();
    cout << "========== ADD NEW BUILDING ==========\n";
    int id = read<int>("Building ID: ", 1, 9999);
    cin.ignore();
    string name, location;
    cout << "Building Name: "; getline(cin, name);
    cout << "Location: "; getline(cin, location);
    
    Building* b = new Building(id, name, location);
    LMS.addBuilding(b);
    cout << "Building added successfully.\n";
    pauseScr();
}

void addRoomMenu() {
    clearScr();
    cout << "========== ADD NEW ROOM ==========\n";
    
    cout << "\nAvailable Buildings:\n";
    for (auto* b : LMS.getBuildings())
        cout << "  ID: " << b->getBuildingId() << " - " << b->getName() << "\n";
    
    int rid = read<int>("\nRoom ID: ", 1, 9999);
    cin.ignore();
    string rnum;
    cout << "Room Number: "; getline(cin, rnum);
    int cap = read<int>("Capacity: ", 1, 200);
    int bid = read<int>("Building ID: ", 1, 9999);
    
    Building* building = nullptr;
    for (auto* b : LMS.getBuildings())
        if (b->getBuildingId() == bid) { building = b; break; }
    
    if (!building) {
        cout << "Building not found.\n";
    } else {
        Room* r = new Room(rid, rnum, cap, building);
        LMS.addRoom(r);
        cout << "Room added successfully.\n";
    }
    pauseScr();
}

void addInstructorMenu() {
    clearScr();
    cout << "========== ADD NEW INSTRUCTOR ==========\n";
    int id = read<int>("Instructor ID: ", 1000, 9999);
    cin.ignore();
    string name, email, dept, office;
    cout << "Name: "; getline(cin, name);
    cout << "Email: "; getline(cin, email);
    cout << "Department: "; getline(cin, dept);
    cout << "Office: "; getline(cin, office);
    
    Instructor* ins = new Instructor(id, name, email, dept, office);
    LMS.addInstructor(ins);
    cout << "Instructor added successfully.\n";
    pauseScr();
}

void addTAMenu() {
    clearScr();
    cout << "========== ADD NEW TA ==========\n";
    int id = read<int>("TA ID: ", 2000, 9999);
    cin.ignore();
    string name, email, sid, prog;
    cout << "Name: "; getline(cin, name);
    cout << "Email: "; getline(cin, email);
    cout << "Student ID: "; getline(cin, sid);
    cout << "Program: "; getline(cin, prog);
    
    TA* ta = new TA(id, name, email, sid, prog);
    LMS.addTA(ta);
    cout << "TA added successfully.\n";
    pauseScr();
}

void addLabSectionMenu() {
    clearScr();
    cout << "========== ADD NEW LAB SECTION ==========\n";
    
    cout << "\nAvailable Instructors:\n";
    for (auto* i : LMS.getInstructors())
        cout << "  ID: " << i->getId() << " - " << i->getName() << "\n";
    
    cout << "\nAvailable Rooms:\n";
    for (auto* r : LMS.getRooms())
        cout << "  ID: " << r->getRoomId() << " - " << r->getRoomNumber() << "\n";
    
    int sid = read<int>("\nSection ID: ", 1, 99);
    cin.ignore();
    string ccode, sname;
    cout << "Course Code: "; getline(cin, ccode);
    cout << "Section Name: "; getline(cin, sname);
    int iid = read<int>("Instructor ID: ", 1000, 9999);
    int rid = read<int>("Room ID: ", 1, 9999);
    
    Instructor* inst = nullptr;
    for (auto* i : LMS.getInstructors())
        if (i->getId() == iid) { inst = i; break; }
    
    Room* room = nullptr;
    for (auto* r : LMS.getRooms())
        if (r->getRoomId() == rid) { room = r; break; }
    
    if (!inst || !room) {
        cout << "Instructor or Room not found.\n";
    } else {
        LabSection* ls = new LabSection(sid, ccode, sname, inst, room);
        LMS.addLabSection(ls);
        cout << "Lab Section added successfully.\n";
    }
    pauseScr();
}

void addAttendantMenu() {
    clearScr();
    cout << "========== ADD NEW ATTENDANT ==========\n";
    
    cout << "\nAvailable Buildings:\n";
    for (auto* b : LMS.getBuildings())
        cout << "  ID: " << b->getBuildingId() << " - " << b->getName() << "\n";
    
    int id = read<int>("\nAttendant ID: ", 6000, 9999);
    cin.ignore();
    string name, email;
    cout << "Name: "; getline(cin, name);
    cout << "Email: "; getline(cin, email);
    int bid = read<int>("Building ID: ", 1, 9999);
    
    Building* building = nullptr;
    for (auto* b : LMS.getBuildings())
        if (b->getBuildingId() == bid) { building = b; break; }
    
    if (!building) {
        cout << "Building not found.\n";
    } else {
        Attendant* att = new Attendant(id, name, email, building);
        LMS.addAttendant(att);
        cout << "Attendant added successfully.\n";
    }
    pauseScr();
}

void viewAllDataMenu() {
    while(true) {
        clearScr();
        cout << "========== VIEW ALL DATA ==========\n"
             << "1. View Buildings\n"
             << "2. View Rooms\n"
             << "3. View Instructors\n"
             << "4. View TAs\n"
             << "5. View Lab Sections\n"
             << "6. View Schedules\n"
             << "7. View Time Sheets\n"
             << "8. View Attendants\n"
             << "9. View Makeup Requests\n"
             << "0. Back\n"
             << "Choice: ";
        int c; cin >> c; cin.ignore();
        
        if(c == 0) break;
        clearScr();
        
        switch(c) {
        case 1:
            cout << "========== BUILDINGS ==========\n";
            for (auto* b : LMS.getBuildings())
                cout << "ID: " << b->getBuildingId() << " | Name: " << b->getName() 
                     << " | Location: " << b->getLocation() << "\n";
            break;
        case 2:
            cout << "========== ROOMS ==========\n";
            for (auto* r : LMS.getRooms())
                cout << "ID: " << r->getRoomId() << " | Number: " << r->getRoomNumber() 
                     << " | Capacity: " << r->getCapacity() << "\n";
            break;
        case 3:
            cout << "========== INSTRUCTORS ==========\n";
            for (auto* i : LMS.getInstructors())
                cout << "ID: " << i->getId() << " | Name: " << i->getName() 
                     << " | Dept: " << i->getDepartment() << "\n";
            break;
        case 4:
            cout << "========== TAs ==========\n";
            for (auto* t : LMS.getTAs())
                cout << "ID: " << t->getId() << " | Name: " << t->getName() 
                     << " | Program: " << t->getProgram() << "\n";
            break;
        case 5:
            cout << "========== LAB SECTIONS ==========\n";
            for (auto* ls : LMS.getLabSections())
                cout << "ID: " << ls->getSectionId() << " | Course: " << ls->getCourseCode() 
                     << " | Section: " << ls->getSectionName() << "\n";
            break;
        case 6:
            cout << "========== SCHEDULES ==========\n";
            for (auto* s : LMS.getSchedules()) {
                auto* sec = s->getLabSection();
                if (sec)
                    cout << "ID: " << s->getScheduleId() << " | Section: " << sec->getSectionName()
                         << " | Day: " << s->getDayOfWeek() << " | Time: " 
                         << s->getExpectedStartTime() << "-" << s->getExpectedEndTime() << "\n";
            }
            break;
        case 7:
            cout << "========== TIME SHEETS ==========\n";
            for (auto* ts : LMS.getTimeSheets()) {
                auto* sec = ts->getLabSection();
                if (sec)
                    cout << "ID: " << ts->getTimeSheetId() << " | Section: " << sec->getSectionName()
                         << " | Date: " << ts->getDate() << " | Makeup: " 
                         << (ts->getIsMakeup() ? "YES" : "NO") << "\n";
            }
            break;
        case 8:
            cout << "========== ATTENDANTS ==========\n";
            for (auto* a : LMS.getAttendants()) {
                auto* b = a->getAssignedBuilding();
                cout << "ID: " << a->getId() << " | Name: " << a->getName() 
                     << " | Building: " << (b ? b->getName() : "N/A") << "\n";
            }
            break;
        case 9:
            cout << "========== MAKEUP REQUESTS ==========\n";
            for (auto* m : LMS.getMakeupLabs()) {
                auto* sec = m->getLabSection();
                cout << "ID: " << m->getMakeupId() << " | Section: " 
                     << (sec ? sec->getSectionName() : "N/A")
                     << " | Date: " << m->getRequestDate() << " | Reason: " 
                     << m->getReason() << "\n";
            }
            break;
        default:
            cout << "Invalid choice.\n";
        }
        pauseScr();
    }
}

void viewMakeupRequestsMenu() {
    clearScr();
    cout << "========== PENDING MAKEUP REQUESTS ==========\n";
    bool found = false;
    for (auto* m : LMS.getMakeupLabs()) {
        if (!m->getMakeupSchedule()) {
            found = true;
            auto* sec = m->getLabSection();
            cout << "Request ID: " << m->getMakeupId() << "\n";
            cout << "  Section: " << (sec ? sec->getSectionName() : "N/A") << "\n";
            cout << "  Course: " << (sec ? sec->getCourseCode() : "N/A") << "\n";
            cout << "  Instructor: " << (sec && sec->getInstructor() ? sec->getInstructor()->getName() : "N/A") << "\n";
            cout << "  Date: " << m->getRequestDate() << "\n";
            cout << "  Reason: " << m->getReason() << "\n";
            cout << "  Room: " << (sec && sec->getRoom() ? sec->getRoom()->getRoomNumber() : "N/A") << "\n";
            cout << string(50, '-') << "\n";
        }
    }
    if (!found) cout << "No pending makeup requests.\n";
    pauseScr();
}

void fillTimeSheetMenu() {
    clearScr();
    cout << "========== ATTENDANT - FILL TIMESHEET ==========\n";
    
    cout << "\nAvailable Time Sheets:\n";
    for (auto* ts : LMS.getTimeSheets()) {
        auto* sec = ts->getLabSection();
        if (sec)
            cout << "  ID: " << ts->getTimeSheetId() << " | Section: " 
                 << sec->getSectionName() << " | Date: " << ts->getDate() << "\n";
    }
    
    int tsId = read<int>("\nTimeSheet ID: ", 1, 999999);
    auto* ts = [&]()->TimeSheet*{
        for(auto* t : LMS.getTimeSheets()) 
            if(t->getTimeSheetId() == tsId) return t;
        return nullptr;
    }();
    
    if(!ts) { 
        cout << "TimeSheet not found.\n"; 
        pauseScr(); 
        return; 
    }
    
    cin.ignore();
    string s, e;
    cout << "Actual start (HH:MM): "; getline(cin, s);
    cout << "Actual end   (HH:MM): "; getline(cin, e);
    LMS.fillTimeSheet(tsId, s, e);
    pauseScr();
}

void makeupRequestMenu() {
    clearScr();
    cout << "========== INSTRUCTOR - REQUEST MAKEUP ==========\n";
    
    cout << "\nAvailable Instructors:\n";
    for (auto* i : LMS.getInstructors())
        cout << "  ID: " << i->getId() << " - " << i->getName() << "\n";
    
    cout << "\nAvailable Lab Sections:\n";
    for (auto* ls : LMS.getLabSections())
        cout << "  ID: " << ls->getSectionId() << " - " << ls->getCourseCode() 
             << " - " << ls->getSectionName() << "\n";
    
    int insId = read<int>("\nYour Instructor ID: ", 1000, 9999);
    int secId = read<int>("Lab Section ID: ", 1, 99);
    cin.ignore();
    string date, reason;
    cout << "Date (YYYY-MM-DD): "; getline(cin, date);
    cout << "Reason: "; getline(cin, reason);
    LMS.requestMakeupLab(insId, secId, date, reason);
    pauseScr();
}

void scheduleMakeupMenu() {
    clearScr();
    cout << "========== ACADEMIC OFFICER - SCHEDULE MAKEUP ==========\n";
    
    cout << "\nPending Makeup Requests:\n";
    bool found = false;
    for (auto* m : LMS.getMakeupLabs()) {
        if (!m->getMakeupSchedule()) {
            found = true;
            auto* sec = m->getLabSection();
            cout << "  ID: " << m->getMakeupId() << " | Section: " 
                 << (sec ? sec->getSectionName() : "N/A")
                 << " | Date: " << m->getRequestDate() << "\n";
        }
    }
    if (!found) {
        cout << "No pending requests.\n";
        pauseScr();
        return;
    }
    
    int mId = read<int>("\nMakeup Request ID: ", 1, 999999);
    cin.ignore();
    string day, st, et;
    cout << "Day (e.g. Monday): "; getline(cin, day);
    cout << "Start (HH:MM): "; getline(cin, st);
    cout << "End   (HH:MM): "; getline(cin, et);
    LMS.scheduleMakeupLab(mId, day, st, et);
    pauseScr();
}

void reportMenu() {
    while(true) {
        clearScr();
        cout << "========== HOD - REPORTS ==========\n"
             << "1. Weekly Schedule Report\n"
             << "2. Weekly Time-Sheet Report\n"
             << "3. Semester Report (contact hrs + leaves)\n"
             << "0. Back\n"
             << "Choice: ";
        int c; cin >> c; cin.ignore();
        if(c == 0) break;
        
        switch(c) {
        case 1: {
            int wk = read<int>("Week Number: ", 1, 52);
            LMS.generateWeeklyScheduleReport(wk); 
            pauseScr(); 
            break;
        }
        case 2: {
            cin.ignore();
            string ws;
            cout << "Week Start (YYYY-MM-DD): "; getline(cin, ws);
            LMS.generateWeeklyTimeSheetReport(ws); 
            pauseScr(); 
            break;
        }
        case 3: {
            cout << "\nAvailable Lab Sections:\n";
            for (auto* ls : LMS.getLabSections())
                cout << "  ID: " << ls->getSectionId() << " - " << ls->getCourseCode() << "\n";
            int sid = read<int>("\nLab Section ID: ", 1, 99);
            LMS.generateLabSemesterReport(sid); 
            pauseScr(); 
            break;
        }
        default: 
            cout << "Invalid.\n"; 
            pauseScr();
        }
    }
}

void dataManagementMenu() {
    while(true) {
        clearScr();
        cout << "========== DATA MANAGEMENT ==========\n"
             << "1. Add Building\n"
             << "2. Add Room\n"
             << "3. Add Instructor\n"
             << "4. Add TA\n"
             << "5. Add Lab Section\n"
             << "6. Add Attendant\n"
             << "7. View All Data\n"
             << "0. Back\n"
             << "Choice: ";
        int c; cin >> c; cin.ignore();
        if(c == 0) break;
        
        switch(c) {
        case 1: addBuildingMenu(); break;
        case 2: addRoomMenu(); break;
        case 3: addInstructorMenu(); break;
        case 4: addTAMenu(); break;
        case 5: addLabSectionMenu(); break;
        case 6: addAttendantMenu(); break;
        case 7: viewAllDataMenu(); break;
        default: cout << "Invalid.\n"; pauseScr();
        }
    }
}

void academicOfficerMenu() {
    while(true) {
        clearScr();
        cout << "========== ACADEMIC OFFICER MENU ==========\n"
             << "1. View Pending Makeup Requests\n"
             << "2. Schedule Makeup Lab\n"
             << "3. Populate Lab Data (Generate Schedules)\n"
             << "0. Back\n"
             << "Choice: ";
        int c; cin >> c; cin.ignore();
        if(c == 0) break;
        
        switch(c) {
        case 1: viewMakeupRequestsMenu(); break;
        case 2: scheduleMakeupMenu(); break;
        case 3: 
            if (LMS.getAcademicOfficer()) {
                LMS.getAcademicOfficer()->populateLabData();
                cout << "Lab data populated.\n";
            } else {
                cout << "Academic Officer not configured.\n";
            }
            pauseScr();
            break;
        default: cout << "Invalid.\n"; pauseScr();
        }
    }
}

void mainMenu() {
    while(true) {
        clearScr();
        cout << "========================================================\n"
             << "          UNIVERSITY LAB MANAGEMENT SYSTEM        \n"
             << "========================================================\n"
             << "  1.  Data Management (Add/View)\n"
             << "  2.  Instructor - Request Makeup Lab\n"
             << "  3.  Academic Officer - Manage Makeup Labs\n"
             << "  4.  Attendant - Fill TimeSheet\n"
             << "  5.  HOD - Generate Reports\n"
             << "  0.  Exit & Save\n"
             << "========================================================\n"
             << "Choice: ";
        int ch; cin >> ch; cin.ignore();
        
        switch(ch) {
        case 1: dataManagementMenu(); break;
        case 2: makeupRequestMenu(); break;
        case 3: academicOfficerMenu(); break;
        case 4: fillTimeSheetMenu(); break;
        case 5: reportMenu(); break;
        case 0: 
            savePersistent(); 
            cout << "Data saved successfully. Good-bye!\n"; 
            return;
        default: 
            cout << "Invalid choice.\n"; 
            pauseScr();
        }
    }
}

int main() {
    loadPersistent();
    
    if (LMS.getBuildings().empty()) {
        clearScr();
        cout << "No persisted data found. Seed with sample data? (y/n): ";
        char ans; cin >> ans; cin.ignore();
        if (ans == 'y' || ans == 'Y') seedData();
    }
    
    mainMenu();
    return 0;
}