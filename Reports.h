#ifndef REPORTS_H
#define REPORTS_H
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "FileHandler.h"
using namespace std;

class Reports {
private:
    int    reportId;
    string reportType;      // e.g. "WeeklySchedule", "WeeklyTimeSheet", "LabSemester"
    string generationDate;  // expected in YYYY-MM-DD format
    string content;         // will hold the actual report text

public:
    Reports() : reportId(0), reportType(""), generationDate(""), content("") {}
    Reports(int rid, string type, string date, string pcontent)
        : reportId(rid), reportType(type), generationDate(date), content(pcontent) {}

    /* ---- Getters ---- */
    int    getReportId()        const { return reportId; }
    string getReportType()      const { return reportType; }
    string getGenerationDate()  const { return generationDate; }
    string getContent()         const { return content; }

    /* =========================================================
       Builds a human-readable report and stores it in `content`
       ========================================================= */
    void generateReport() {
        ostringstream oss;
        oss << "========================================\n";
        oss << "  UNIVERSITY LAB MANAGEMENT SYSTEM\n";
        oss << "----------------------------------------\n";
        oss << "Report ID : " << reportId << "\n";
        oss << "Type      : " << reportType << "\n";
        oss << "Generated : " << generationDate << "\n";
        oss << "========================================\n\n";
        oss << content;   // append existing content (if any)
        content = oss.str();
    }

    /* =========================================================
       Saves the report to a text file:
            <reportType>_<reportId>.txt
       ========================================================= */
    void saveReport() {
        string fileName = reportType + "_" + to_string(reportId) + ".txt";
        ofstream out(fileName);
        if (out.is_open()) {
            out << content;
            out.close();
            cout << "Report saved to \"" << fileName << "\"\n";
        } else {
            cerr << "Error: could not save report to \"" << fileName << "\"\n";
        }
    }

    /* =========================================================
       Binary persistence (unchanged)
       ========================================================= */
    void serialize(ofstream& file) const {
        file.write((char*)&reportId, sizeof(reportId));
        FileHandler::writeString(file, reportType);
        FileHandler::writeString(file, generationDate);
        FileHandler::writeString(file, content);
    }

    void deserialize(ifstream& file) {
        file.read((char*)&reportId, sizeof(reportId));
        reportType      = FileHandler::readString(file);
        generationDate  = FileHandler::readString(file);
        content         = FileHandler::readString(file);
    }

    void saveToFile(const string& filename) const {
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