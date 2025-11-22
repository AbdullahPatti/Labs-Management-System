#ifndef ATTENDANT_H
#define ATTENDANT_H
#include "Person.h"
#include "TimeSheet.h"
class Building;
using namespace std;

class Attendant : public Person {
    Building* assignedBuilding;
public:
    Attendant() : Person(), assignedBuilding(nullptr) {}
    Attendant(int pid, string pname, string pemail, Building* b)
        : Person(pid, pname, pemail), assignedBuilding(b) {}

    Building* getAssignedBuilding() const { return assignedBuilding; }
    // persistence helper
void _setBuilding(Building* b) { assignedBuilding = b; }

    void fillTimeSheet(TimeSheet* ts, string st, string et) {
        if (!ts) return;
        ts->setActualStartTime(st);
        ts->setActualEndTime(et);
        cout << "\n[Attendant " << getName() << "] filled timesheet "
             << ts->getTimeSheetId() << "  " << st << " – " << et << "\n";
    }

    void serialize(ofstream& f) const override {
        Person::serialize(f);
        int bid = assignedBuilding ? assignedBuilding->getBuildingId() : -1;
        f.write((char*)&bid, sizeof(bid));
    }
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        int bid; f.read((char*)&bid, sizeof(bid));
        // rebuild pointer via id later
    }
};
#endif