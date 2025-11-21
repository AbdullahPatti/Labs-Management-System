#ifndef ACADEMICOFFICER_H
#define ACADEMICOFFICER_H
#include "Person.h"
class LabManagementSystem;
using namespace std;

class AcademicOfficer : public Person {
    string department;
    LabManagementSystem* system = nullptr;
public:
    AcademicOfficer() : Person(), department("") {}
    AcademicOfficer(int pid, string pname, string pemail, string pdept)
        : Person(pid, pname, pemail), department(pdept) {}

    string getDepartment() const { return department; }
    void   setSystem(LabManagementSystem* sys) { system = sys; }

    void populateLabData();
    void scheduleMakeupLab(int mid, const string& day, const string& st, const string& et);

    void serialize(ofstream& f) const override {
        Person::serialize(f);
        FileHandler::writeString(f, department);
    }
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        department = FileHandler::readString(f);
    }
};
#endif