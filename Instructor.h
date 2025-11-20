#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include "Person.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Instructor : public Person {
private:
    string department;
    string office;
    
public:
    Instructor() : Person(), department(""), office("") {}
    Instructor(int pid, string pname, string pemail, string pdept, string poffice) 
        : Person(pid, pname, pemail), department(pdept), office(poffice) {}
    
    string getDepartment() { return department; }
    void setDepartment(string pdept) { department = pdept; }
    string getOffice() { return office; }
    void setOffice(string poffice) { office = poffice; }
    
    void requestMakeupLab() {
        cout << "Instructor " << name << " requested a makeup lab." << endl;
    }

    // Binary file handling
    void serialize(ofstream& file) override {
        Person::serialize(file);
        FileHandler::writeString(file, department);
        FileHandler::writeString(file, office);
    }

    void deserialize(ifstream& file) override {
        Person::deserialize(file);
        department = FileHandler::readString(file);
        office = FileHandler::readString(file);
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