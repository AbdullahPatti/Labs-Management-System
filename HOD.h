#ifndef HOD_H
#define HOD_H
#include "Person.h"
class LabManagementSystem;
using namespace std;

class HOD : public Person {
    string department;
    LabManagementSystem* system = nullptr;
public:
    HOD() : Person(), department("") {}
    HOD(int pid, string pname, string pemail, string pdept)
        : Person(pid, pname, pemail), department(pdept) {}

    string getDepartment() const { return department; }
    void   setSystem(LabManagementSystem* sys) { system = sys; }

    /* business functions – implemented later */
    void generateWeeklyScheduleReport(int weekNo);
    void generateWeeklyTimeSheetReport(const string& weekStart);
    void generateLabSemesterReport(int labSecId);

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