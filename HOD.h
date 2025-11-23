#ifndef HOD_H
#define HOD_H

#include "Person.h"
class LabManagementSystem;
using namespace std;

/**
 * HOD (Head of Department) class represents department leadership
 * 
 * Responsibilities:
 * - Generate departmental reports and analytics
 * - Oversee lab scheduling and performance
 * - Access comprehensive system data
 * 
 * Key functions: Various report generation for departmental oversight
 */
class HOD : public Person {
    string department;
    LabManagementSystem* system = nullptr;
    
public:
    HOD() : Person(), department("") {}
    
    /**
     * Creates a new HOD record
     * @param pid Unique person identifier
     * @param pname Full name of HOD
     * @param pemail Contact email address
     * @param pdept Academic department
     */
    HOD(int pid, string pname, string pemail, string pdept)
        : Person(pid, pname, pemail), department(pdept) {}

    string getDepartment() const { return department; }
    void   setSystem(LabManagementSystem* sys) { system = sys; }

    /* Business functions – implemented in Main.cpp */
    
    /**
     * Generates weekly schedule report for department
     * @param weekNo Week number for report (1-52)
     */
    void generateWeeklyScheduleReport(int weekNo);
    
    /**
     * Generates weekly timesheet report for department
     * @param weekStart Starting date of week (YYYY-MM-DD)
     */
    void generateWeeklyTimeSheetReport(const string& weekStart);
    
    /**
     * Generates semester report for specific lab section
     * @param labSecId Lab section identifier
     */
    void generateLabSemesterReport(int labSecId);

    /**
     * Serializes HOD data to binary file
     * @param f Output file stream for binary writing
     */
    void serialize(ofstream& f) const override {
        Person::serialize(f);
        FileHandler::writeString(f, department);
    }

    /**
     * Deserializes HOD data from binary file
     * @param f Input file stream for binary reading
     */
    void deserialize(ifstream& f) override {
        Person::deserialize(f);
        department = FileHandler::readString(f);
    }
};

#endif