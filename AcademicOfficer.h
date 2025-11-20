#ifndef ACADEMICOFFICER_H
#define ACADEMICOFFICER_H
#include "Person.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class AcademicOfficer : public Person {
private:
    string department;
    
public:
    AcademicOfficer() : Person(), department("") {}
    AcademicOfficer(int pid, string pname, string pemail, string pdept) 
        : Person(pid, pname, pemail), department(pdept) {}
    
    string getDepartment() { return department; }
    void setDepartment(string pdept) { department = pdept; }
    
    void populateLabData() {
        cout << "Academic Officer " << name << " populated lab data." << endl;
    }
    
    void scheduleMakeupLab() {
        cout << "Academic Officer " << name << " scheduled a makeup lab." << endl;
    }

    // Binary file handling
    void serialize(ofstream& file) override {
        Person::serialize(file);
        FileHandler::writeString(file, department);
    }

    void deserialize(ifstream& file) override {
        Person::deserialize(file);
        department = FileHandler::readString(file);
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