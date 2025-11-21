#ifndef TA_H
#define TA_H
#include "Person.h"
using namespace std;

class TA : public Person {
    string studentId;
    string program;
public:
    TA() : Person(), studentId(""), program("") {}
    TA(int pid, string pname, string pemail, string psid, string pprog)
        : Person(pid, pname, pemail), studentId(psid), program(pprog) {}

    string getStudentId() const { return studentId; }
    string getProgram()   const { return program; }

    void serialize(ofstream& f) const override {
        Person::serialize(f);
        FileHandler::writeString(f, studentId);
        FileHandler::writeString(f, program);
    }
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        studentId = FileHandler::readString(f);
        program   = FileHandler::readString(f);
    }
};
#endif