#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include "Person.h"
using namespace std;

class Instructor : public Person {
    string department;
    string office;
public:
    Instructor() : Person(), department(""), office("") {}
    Instructor(int pid, string pname, string pemail, string pdept, string poff)
        : Person(pid, pname, pemail), department(pdept), office(poff) {}

    string getDepartment() const { return department; }
    string getOffice()     const { return office; }

    void serialize(ofstream& f) const override {
        Person::serialize(f);
        FileHandler::writeString(f, department);
        FileHandler::writeString(f, office);
    }
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        department = FileHandler::readString(f);
        office     = FileHandler::readString(f);
    }
};
#endif