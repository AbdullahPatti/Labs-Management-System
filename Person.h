#ifndef PERSON_H
#define PERSON_H
#include <iostream>
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Person {
protected:
    int id;
    string name;
    string email;
    
public:
    Person() : id(0), name(""), email("") {}
    Person(int pid, string pname, string pemail) : id(pid), name(pname), email(pemail) {}
    virtual ~Person() {}
    
    int getId() { return id; }
    void setId(int pid) { id = pid; }
    string getName() { return name; }
    void setName(string pname) { name = pname; }
    string getEmail() { return email; }
    void setEmail(string pemail) { email = pemail; }

    // Binary file handling
    virtual void serialize(ofstream& file) {
        file.write((char*)&id, sizeof(id));
        FileHandler::writeString(file, name);
        FileHandler::writeString(file, email);
    }

    virtual void deserialize(ifstream& file) {
        file.read((char*)&id, sizeof(id));
        name = FileHandler::readString(file);
        email = FileHandler::readString(file);
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