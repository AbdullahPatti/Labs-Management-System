/******************************************************************************************
 *  University Lab Management System  –  MENU DRIVEN, FULLY FEATURED, SELF-CONTAINED
 *
 *  Compile:  g++ -std=c++17 Main.cpp -o LabSys
 *  Run:      ./LabSys
 ******************************************************************************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <cstdlib>        // system("clear")
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
    cout << "\n---------- WEEKLY SCHEDULE – WEEK " << weekNo << " ----------\n";
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
    cout << "\n---------- TIMESHEET WEEK STARTING " << weekStart << " ----------\n";
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
    cout << "\n---------- SEMESTER REPORT  " << sec->getCourseCode()
         << " – " << sec->getSectionName() << " ----------\n";
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
    static int schId = 8000, tsId = 9000;
    auto* sch = new Schedule(++schId, sec, day, st, et);
    addSchedule(sch);
    m->setMakeupSchedule(sch);
    auto* ts = new TimeSheet(++tsId, sec, m->getRequestDate(), "", "", true);
    addTimeSheet(ts);
    cout << "Makeup lab scheduled.\n";
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
void clearScr()  { system("cls||clear"); }
void pauseScr()  { cout << "\nPress ENTER to continue..."; cin.ignore(numeric_limits<streamsize>::max(),'\n'); }
template<typename T>
T read(string prompt, T low, T high) {
    T x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= low && x <= high) return x;
        cin.clear(); cin.ignore(256,'\n');
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

/* rebuild pointers after load */
void rebuildLinks() {
    /* Building -> Room */
    for (auto* r : LMS.getRooms())
        for (auto* b : LMS.getBuildings())
            if (b->getBuildingId() == [&]{
                ifstream ifs(BASE_DIR + "buildings.dat", ios::binary);
                ifs.seekg(sizeof(size_t)); // skip count
                for (size_t i = 0; i < LMS.getBuildings().size(); ++i) {
                    int bid; ifs.read((char*)&bid, sizeof(bid));
                    string name = FileHandler::readString(ifs);
                    string loc  = FileHandler::readString(ifs);
                    if (b->getBuildingId() == bid) return bid;
                }
                return -1;
            }()) r->_setBuilding(b);

    /* LabSection -> Instructor, Room, TAs */
    for (auto* ls : LMS.getLabSections()) {
        int iid, rid; size_t tas;
        ifstream ifs(BASE_DIR + "labsections.dat", ios::binary);
        ifs.seekg(sizeof(size_t));
        for (size_t i = 0; i < LMS.getLabSections().size(); ++i) {
            int sid; ifs.read((char*)&sid, sizeof(sid));
            string cc = FileHandler::readString(ifs);
            string sn = FileHandler::readString(ifs);
            ifs.read((char*)&iid, sizeof(iid));
            ifs.read((char*)&rid, sizeof(rid));
            ifs.read((char*)&tas, sizeof(tas));
            if (sid == ls->getSectionId()) break;
            for (size_t j = 0; j < tas; ++j) { int tid; ifs.read((char*)&tid, sizeof(tid)); }
        }
        for (auto* inst : LMS.getInstructors()) if (inst->getId() == iid) ls->_setInstructor(inst);
        for (auto* r : LMS.getRooms()) if (r->getRoomId() == rid) ls->_setRoom(r);
        ls->_clearTAs();
        for (size_t j = 0; j < tas; ++j) {
            int tid; ifs.read((char*)&tid, sizeof(tid));
            for (auto* ta : LMS.getTAs()) if (ta->getId() == tid) ls->_addTA(ta);
        }
    }

    /* Attendant -> Building */
    for (auto* a : LMS.getAttendants()) {
        int bid;
        ifstream ifs(BASE_DIR + "attendants.dat", ios::binary);
        ifs.seekg(sizeof(size_t));
        for (size_t i = 0; i < LMS.getAttendants().size(); ++i) {
            int aid; ifs.read((char*)&aid, sizeof(aid));
            string nm = FileHandler::readString(ifs);
            string em = FileHandler::readString(ifs);
            ifs.read((char*)&bid, sizeof(bid));
            if (aid == a->getId()) break;
        }
        for (auto* b : LMS.getBuildings()) if (b->getBuildingId() == bid) a->_setBuilding(b);
    }

    /* HOD / AcademicOfficer -> LabManagementSystem */
    if (LMS.getHOD()) const_cast<HOD*>(LMS.getHOD())->setSystem(&LMS);
    if (LMS.getAcademicOfficer()) const_cast<AcademicOfficer*>(LMS.getAcademicOfficer())->setSystem(&LMS);
}
void savePersistent() {
    cout << "Saving persistent state...\n";
    system(("mkdir -p " + BASE_DIR).c_str());
    saveVector(LMS.getBuildings(),     "buildings.dat");
    saveVector(LMS.getRooms(),         "rooms.dat");
    saveVector(LMS.getInstructors(),   "instructors.dat");
    saveVector(LMS.getTAs(),           "tas.dat");
    saveVector(LMS.getLabSections(),   "labsections.dat");
    saveVector(LMS.getSchedules(),     "schedules.dat");
    saveVector(LMS.getTimeSheets(),    "timesheets.dat");
    saveVector(LMS.getAttendants(),    "attendants.dat");
    saveVector(LMS.getMakeupLabs(),    "makeuplabs.dat");
    /* officers */
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
    /* officers */
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

/* ============================================================
                      PRE-POPULATION ROUTINES
   ============================================================ */
void seedData() {
    cout << "Seeding master data...\n";

    /* Buildings & Rooms */
    Building* csB = new Building(1,"Computer-Science Block","Main Campus");
    Building* eeB = new Building(2,"Electrical Block","North Campus");
    LMS.addBuilding(csB); LMS.addBuilding(eeB);

    Room* r1 = new Room(101,"CS-Lab-1",35,csB);
    Room* r2 = new Room(102,"CS-Lab-2",30,csB);
    Room* r3 = new Room(201,"EE-Lab-1",40,eeB);
    LMS.addRoom(r1); LMS.addRoom(r2); LMS.addRoom(r3);

    /* Instructors */
    Instructor* i1 = new Instructor(1001,"Dr. Ahmed Khan","ahmed@uni.edu","CS","CS-101");
    Instructor* i2 = new Instructor(1002,"Dr. Fatima Ali","fatima@uni.edu","CS","CS-102");
    Instructor* i3 = new Instructor(1003,"Dr. Hassan Malik","hassan@uni.edu","EE","EE-201");
    LMS.addInstructor(i1); LMS.addInstructor(i2); LMS.addInstructor(i3);

    /* TAs */
    TA* t1 = new TA(2001,"Sara Ali","sara@student.edu","BSCS001","BSCS");
    TA* t2 = new TA(2002,"Bilal Ahmed","bilal@student.edu","BSCS002","BSCS");
    TA* t3 = new TA(2003,"Ayesha Khan","ayesha@student.edu","BSCS003","BSCS");
    TA* t4 = new TA(2004,"Usman Malik","usman@student.edu","BSEE001","BSEE");
    LMS.addTA(t1); LMS.addTA(t2); LMS.addTA(t3); LMS.addTA(t4);

    /* Lab Sections */
    LabSection* ls1 = new LabSection(1,"CS-101L","Prog-Fundamentals",i1,r1);
    LabSection* ls2 = new LabSection(2,"CS-201L","Data-Structures",i2,r2);
    LabSection* ls3 = new LabSection(3,"EE-301L","Digital-Logic",i3,r3);
    ls1->addTA(t1); ls1->addTA(t2); ls2->addTA(t3); ls3->addTA(t4);
    LMS.addLabSection(ls1); LMS.addLabSection(ls2); LMS.addLabSection(ls3);

    /* Default Weekly Schedule (Mon-Wed) */
    int sid=3001, tid=4001;
    auto add=[&](LabSection* sec, string day, string s, string e){
        Schedule* sch = new Schedule(sid++,sec,day,s,e);
        LMS.addSchedule(sch);
        TimeSheet* ts = new TimeSheet(tid++,sec,"2025-11-24","", "", false);
        LMS.addTimeSheet(ts);
    };
    add(ls1,"Monday",   "14:00","16:00");
    add(ls2,"Tuesday",  "10:00","12:00");
    add(ls3,"Wednesday","14:00","16:00");
    add(ls1,"Thursday", "14:00","16:00");   // second slot for ls1

    /* Officers */
    AcademicOfficer* ao = new AcademicOfficer(5001,"Academic Admin","admin@uni.edu","Academic");
    ao->setSystem(&LMS); LMS.setAcademicOfficer(ao);

    HOD* hod = new HOD(4001,"Prof. Dr. Naveed","naveed@uni.edu","CS");
    hod->setSystem(&LMS); LMS.setHOD(hod);

    /* Attendants */
    Attendant* at1 = new Attendant(6001,"Usman Attendant","usman@uni.edu",csB);
    Attendant* at2 = new Attendant(6002,"Ali Attendant","ali@uni.edu",eeB);
    LMS.addAttendant(at1); LMS.addAttendant(at2);

    cout << "✅  Master data seeded.\n";
}

/* ============================================================
                          MENUS
   ============================================================ */
void fillTimeSheetMenu() {
    clearScr();
    cout << "----------  ATTENDANT - FILL TIMESHEET  ----------\n";
    int tsId = read<int>("TimeSheet ID: ",1,999999);
    auto* ts = [&]()->TimeSheet*{
        for(auto* t:LMS.getTimeSheets()) if(t->getTimeSheetId()==tsId) return t;
        return nullptr;
    }();
    if(!ts){ cout<<"TimeSheet not found.\n"; pauseScr(); return; }
    string s,e;
    cout<<"Actual start (HH:MM): "; cin>>s;
    cout<<"Actual end   (HH:MM): "; cin>>e;
    LMS.fillTimeSheet(tsId,s,e);
    pauseScr();
}

void makeupRequestMenu() {
    clearScr();
    cout << "----------  INSTRUCTOR - REQUEST MAKEUP  ----------\n";
    int insId = read<int>("Your Instructor ID: ",1000,9999);
    int secId = read<int>("Lab Section ID: ",1,99);
    string date,reason;
    cout<<"Date (YYYY-MM-DD): "; cin>>date;
    cin.ignore();
    cout<<"Reason: "; getline(cin,reason);
    LMS.requestMakeupLab(insId,secId,date,reason);
    pauseScr();
}

void scheduleMakeupMenu() {
    clearScr();
    cout << "----------  ACADEMIC OFFICER - SCHEDULE MAKEUP  ----------\n";
    int mId = read<int>("Makeup Request ID: ",1,999);
    string day,st,et;
    cout<<"Day (e.g. Monday): "; cin>>day;
    cout<<"Start (HH:MM): "; cin>>st;
    cout<<"End   (HH:MM): "; cin>>et;
    LMS.scheduleMakeupLab(mId,day,st,et);
    pauseScr();
}

void reportMenu() {
    while(true){
        clearScr();
        cout << "----------  HOD - REPORTS  ----------\n"
             << "1. Weekly Schedule Report\n"
             << "2. Weekly Time-Sheet Report\n"
             << "3. Semester Report (contact hrs + leaves)\n"
             << "0. Back\n"
             << "Choice: ";
        int c; cin>>c; cin.ignore();
        if(c==0) break;
        switch(c){
        case 1:{ LMS.generateWeeklyScheduleReport(1); pauseScr(); break;}
        case 2:{ LMS.generateWeeklyTimeSheetReport("2025-11-24"); pauseScr(); break;}
        case 3:{ int sid=read<int>("Lab Section ID: ",1,99);
                 LMS.generateLabSemesterReport(sid); pauseScr(); break;}
        default: cout<<"❌  Invalid.\n"; pauseScr();
        }
    }
}

void mainMenu() {
    while(true){
        clearScr();
        cout << "----------------------------------------------------------\n"
             << "           UNIVERSITY LAB MANAGEMENT SYSTEM         \n"
             << "----------------------------------------------------------\n"
             << "   1.    Instructor - Request Makeup Lab            \n"
             << "   2.    AcademicOfficer - Schedule Makeup           \n"
             << "   3.    Attendant - Fill TimeSheet                \n"
             << "   4.    HOD - Generate Reports                    \n"
             << "   0.    Exit & Save                                \n"
             << "---------------------------------------------------------\n"
             << "Choice: ";
        int ch; cin>>ch; cin.ignore();
        switch(ch){
        case 1: makeupRequestMenu(); break;
        case 2: scheduleMakeupMenu(); break;
        case 3: fillTimeSheetMenu(); break;
        case 4: reportMenu(); break;
        case 0: savePersistent(); cout<<"Good-bye!\n"; return;
        default: cout<<"Invalid.\n"; pauseScr();
        }
    }
}

/* ============================================================ */
int main(){
    loadPersistent();
    if (LMS.getBuildings().empty()) {
        clearScr();
        cout << "No persisted data found.  Seed with sample data? (y/n): ";
        char ans; cin >> ans; cin.ignore();
        if (ans=='y' || ans=='Y') seedData();
    }
    mainMenu();
    return 0;
}