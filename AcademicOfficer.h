#ifndef ACADEMICOFFICER_H
#define ACADEMICOFFICER_H

#include "Person.h"
class LabManagementSystem;
using namespace std;

// Academic officer - manages lab schedules and makeup labs
class AcademicOfficer : public Person {
    string department;
    LabManagementSystem* system = nullptr;
    
public:
    AcademicOfficer() : Person(), department("") {}
    AcademicOfficer(int pid, string pname, string pemail, string pdept)
        : Person(pid, pname, pemail), department(pdept) {}

    string getDepartment() const { return department; }
    void   setSystem(LabManagementSystem* sys) { system = sys; }

    // Populate lab schedules and timesheets
    void populateLabData();
    // Schedule makeup lab session
    void scheduleMakeupLab(int mid, const string& day, const string& st, const string& et);

    // Serialize to binary file
    void serialize(ofstream& f) const override {
        Person::serialize(f);
        FileHandler::writeString(f, department);
    }

    // Deserialize from binary file
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        department = FileHandler::readString(f);
    }
};

#endif