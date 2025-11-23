#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

/**
 * Person base class representing individuals in the system
 * 
 * Responsibilities:
 * - Store common personal information (id, name, email)
 * - Provide base serialization functionality
 * - Serve as parent class for all person types
 * 
 * Derived classes:
 * - Instructor, TA, Attendant, HOD, AcademicOfficer
 * 
 * Design: Base class with virtual methods for polymorphism
 */
class Person {
protected:
    int    id;
    string name;
    string email;
    
public:
    Person() : id(0), name(""), email("") {}
    
    /**
     * Creates a new person record
     * @param pid Unique person identifier
     * @param pname Full name of person
     * @param pemail Contact email address
     */
    Person(int pid, string pname, string pemail)
        : id(pid), name(pname), email(pemail) {}
        
    virtual ~Person() = default;

    // Getters
    int    getId()    const { return id; }
    string getName()  const { return name; }
    string getEmail() const { return email; }

    /**
     * Serializes person data to binary file
     * @param f Output file stream for binary writing
     */
    virtual void serialize(ofstream& f) const {
        f.write((char*)&id, sizeof(id));
        FileHandler::writeString(f, name);
        FileHandler::writeString(f, email);
    }

    /**
     * Deserializes person data from binary file
     * @param f Input file stream for binary reading
     */
    virtual void deserialize(ifstream& f) {
        f.read((char*)&id, sizeof(id));
        name  = FileHandler::readString(f);
        email = FileHandler::readString(f);
    }

    /**
     * Saves person data to binary file
     * @param filename Path to output file
     */
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) { serialize(f); }
    }

    /**
     * Loads person data from binary file
     * @param filename Path to input file
     */
    void loadFromFile(const string& filename) {
        ifstream f(filename, ios::binary);
        if (f.is_open()) { deserialize(f); }
    }
};

#endif