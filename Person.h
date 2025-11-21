#ifndef PERSON_H
#define PERSON_H
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Person {
protected:
    int    id;
    string name;
    string email;
public:
    Person() : id(0), name(""), email("") {}
    Person(int pid, string pname, string pemail)
        : id(pid), name(pname), email(pemail) {}
    virtual ~Person() = default;

    int    getId()    const { return id; }
    string getName()  const { return name; }
    string getEmail() const { return email; }

    virtual void serialize(ofstream& f) const {
        f.write((char*)&id, sizeof(id));
        FileHandler::writeString(f, name);
        FileHandler::writeString(f, email);
    }
    virtual void deserialize(ifstream& f) {
        f.read((char*)&id, sizeof(id));
        name  = FileHandler::readString(f);
        email = FileHandler::readString(f);
    }

    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) { serialize(f); }
    }
    void loadFromFile(const string& filename) {
        ifstream f(filename, ios::binary);
        if (f.is_open()) { deserialize(f); }
    }
};
#endif