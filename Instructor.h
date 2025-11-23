#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include "Person.h"
using namespace std;

/**
 * Instructor class represents teaching faculty members
 * 
 * Responsibilities:
 * - Store instructor professional information
 * - Manage department and office assignments
 * - Serve as base for lab instruction roles
 * 
 * Inherits from: Person (id, name, email)
 * Extended with: department, office location
 */
class Instructor : public Person {
    string department;
    string office;
    
public:
    Instructor() : Person(), department(""), office("") {}
    
    /**
     * Creates a new instructor record
     * @param pid Unique person identifier
     * @param pname Full name of instructor
     * @param pemail Contact email address
     * @param pdept Academic department
     * @param poff Office location/room number
     */
    Instructor(int pid, string pname, string pemail, string pdept, string poff)
        : Person(pid, pname, pemail), department(pdept), office(poff) {}

    string getDepartment() const { return department; }
    string getOffice()     const { return office; }

    /**
     * Serializes instructor data to binary file
     * @param f Output file stream for binary writing
     */
    void serialize(ofstream& f) const override {
        Person::serialize(f);
        FileHandler::writeString(f, department);
        FileHandler::writeString(f, office);
    }

    /**
     * Deserializes instructor data from binary file
     * @param f Input file stream for binary reading
     */
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        department = FileHandler::readString(f);
        office     = FileHandler::readString(f);
    }
};

#endif