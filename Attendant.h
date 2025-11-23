#ifndef ATTENDANT_H
#define ATTENDANT_H

#include "Person.h"
#include "TimeSheet.h"
class Building;
using namespace std;

/**
 * Attendant class represents lab facility support staff
 * 
 * Responsibilities:
 * - Manage lab attendance tracking
 * - Fill timesheets with actual session times
 * - Handle building-specific lab oversight
 * 
 * Key function: fillTimeSheet() for recording actual lab durations
 */
class Attendant : public Person {
    Building* assignedBuilding;
    
public:
    Attendant() : Person(), assignedBuilding(nullptr) {}
    
    /**
     * Creates a new attendant record
     * @param pid Unique person identifier
     * @param pname Full name of attendant
     * @param pemail Contact email address
     * @param b Pointer to assigned Building
     */
    Attendant(int pid, string pname, string pemail, Building* b)
        : Person(pid, pname, pemail), assignedBuilding(b) {}

    Building* getAssignedBuilding() const { return assignedBuilding; }
    
    /**
     * Sets building reference during deserialization
     * @param b Pointer to Building object
     */
    void _setBuilding(Building* b) { assignedBuilding = b; }

    /**
     * Fills timesheet with actual lab session times
     * @param ts Pointer to TimeSheet to fill
     * @param st Actual start time in HH:MM format
     * @param et Actual end time in HH:MM format
     */
    void fillTimeSheet(TimeSheet* ts, string st, string et) {
        if (!ts) return;
        ts->setActualStartTime(st);
        ts->setActualEndTime(et);
        cout << "\n[Attendant " << getName() << "] filled timesheet "
             << ts->getTimeSheetId() << "  " << st << " – " << et << "\n";
    }

    /**
     * Serializes attendant data to binary file
     * @param f Output file stream for binary writing
     */
    void serialize(ofstream& f) const override {
        Person::serialize(f);
        int bid = assignedBuilding ? assignedBuilding->getBuildingId() : -1;
        f.write((char*)&bid, sizeof(bid));
    }

    /**
     * Deserializes attendant data from binary file
     * @param f Input file stream for binary reading
     */
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        int bid; f.read((char*)&bid, sizeof(bid));
        // building pointer rebuilt later via id during link reconstruction
    }
};

#endif