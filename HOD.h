#ifndef HOD_H
#define HOD_H
#include "Person.h"
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class HOD : public Person {
private:
    string department;
    
public:
    HOD() : Person(), department("") {}
    HOD(int pid, string pname, string pemail, string pdept) 
        : Person(pid, pname, pemail), department(pdept) {}
    
    string getDepartment() { return department; }
    void setDepartment(string pdept) { department = pdept; }
    
    void generateWeeklyScheduleReport() {
        cout << "HOD " << name << " generated weekly schedule report." << endl;
    }
    
    void generateWeeklyTimeSheetReport() {
        cout << "HOD " << name << " generated weekly timesheet report." << endl;
    }
    
    void generateLabSemesterReport(int labSectionId) {
        cout << "HOD " << name << " generated semester report for lab " << labSectionId << "." << endl;
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