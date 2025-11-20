#ifndef REPORTS_H
#define REPORTS_H
#include <string>
#include <fstream>
#include "FileHandler.h"
using namespace std;

class Reports {
private:
    int reportId;
    string reportType;
    string generationDate;
    string content;
    
public:
    Reports() : reportId(0), reportType(""), generationDate(""), content("") {}
    Reports(int rid, string type, string date, string pcontent) 
        : reportId(rid), reportType(type), generationDate(date), content(pcontent) {}
    
    // Getters and setters...
    int getReportId() const { return reportId; }
    string getReportType() const { return reportType; }
    string getGenerationDate() const { return generationDate; }
    string getContent() const { return content; }

    void generateReport() {
        cout << "Report generated: " << reportType << endl;
    }
    
    void saveReport() {
        cout << "Report saved to file." << endl;
    }

    void serialize(ofstream& file) const {
        file.write((char*)&reportId, sizeof(reportId));
        FileHandler::writeString(file, reportType);
        FileHandler::writeString(file, generationDate);
        FileHandler::writeString(file, content);
    }

    void deserialize(ifstream& file) {
        file.read((char*)&reportId, sizeof(reportId));
        reportType = FileHandler::readString(file);
        generationDate = FileHandler::readString(file);
        content = FileHandler::readString(file);
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