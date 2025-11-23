#ifndef TA_H
#define TA_H

#include "Person.h"
using namespace std;

/**
 * TA (Teaching Assistant) class represents student teaching assistants
 * 
 * Responsibilities:
 * - Store TA academic information
 * - Track student ID and program enrollment
 * - Assist in lab section instruction
 * 
 * Inherits from: Person (id, name, email)
 * Extended with: studentId, academic program
 */
class TA : public Person {
    string studentId;
    string program;
    
public:
    TA() : Person(), studentId(""), program("") {}
    
    /**
     * Creates a new TA record
     * @param pid Unique person identifier
     * @param pname Full name of TA
     * @param pemail Contact email address
     * @param psid Student identification number
     * @param pprog Academic program/major
     */
    TA(int pid, string pname, string pemail, string psid, string pprog)
        : Person(pid, pname, pemail), studentId(psid), program(pprog) {}

    string getStudentId() const { return studentId; }
    string getProgram()   const { return program; }

    /**
     * Serializes TA data to binary file
     * @param f Output file stream for binary writing
     */
    void serialize(ofstream& f) const override {
        Person::serialize(f);
        FileHandler::writeString(f, studentId);
        FileHandler::writeString(f, program);
    }

    /**
     * Deserializes TA data from binary file
     * @param f Input file stream for binary reading
     */
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        studentId = FileHandler::readString(f);
        program   = FileHandler::readString(f);
    }
};

#endif