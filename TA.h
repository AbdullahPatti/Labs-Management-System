#ifndef TA_H
#define TA_H
#include "Person.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class TA : public Person {
private:
    string studentId;
    string program;
    
public:
    TA() : Person(), studentId(""), program("") {}
    TA(int pid, string pname, string pemail, string pstudentId, string pprogram) 
        : Person(pid, pname, pemail), studentId(pstudentId), program(pprogram) {}
    
    string getStudentId() { return studentId; }
    void setStudentId(string pid) { studentId = pid; }
    string getProgram() { return program; }
    void setProgram(string pprogram) { program = pprogram; }

    // Binary file handling
    void serialize(ofstream& file) override {
        Person::serialize(file);
        FileHandler::writeString(file, studentId);
        FileHandler::writeString(file, program);
    }

    void deserialize(ifstream& file) override {
        Person::deserialize(file);
        studentId = FileHandler::readString(file);
        program = FileHandler::readString(file);
    }

    void saveToFile(const string& filename) {
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            serialize(file);
            file.close();
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (file.is_open()) {
            deserialize(file);
            file.close();
        }
    }
};

#endif