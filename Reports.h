#ifndef REPORTS_H
#define REPORTS_H

#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "FileHandler.h"
using namespace std;

/**
 * Reports class handles report generation and management
 * 
 * Responsibilities:
 * - Generate formatted reports for various system data
 * - Save reports to text files for distribution
 * - Manage report metadata and content
 * 
 * Report types: WeeklySchedule, WeeklyTimeSheet, LabSemester
 */
class Reports {
private:
    int    reportId;
    string reportType;      // e.g. "WeeklySchedule", "WeeklyTimeSheet", "LabSemester"
    string generationDate;  // expected in YYYY-MM-DD format
    string content;         // will hold the actual report text

public:
    Reports() : reportId(0), reportType(""), generationDate(""), content("") {}
    
    /**
     * Creates a new report instance
     * @param rid Unique report identifier
     * @param type Type of report (e.g., "WeeklySchedule")
     * @param date Generation date in YYYY-MM-DD format
     * @param pcontent Report content text
     */
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
    /**
     * Generates formatted report with header and content
     * Adds standard university header to existing content
     */
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
    /**
     * Saves report to text file with standardized naming
     * Format: <reportType>_<reportId>.txt
     */
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
       Binary persistence methods
       ========================================================= */
    /**
     * Serializes report data to binary file
     * @param file Output file stream for binary writing
     */
    void serialize(ofstream& file) const {
        file.write((char*)&reportId, sizeof(reportId));
        FileHandler::writeString(file, reportType);
        FileHandler::writeString(file, generationDate);
        FileHandler::writeString(file, content);
    }

    /**
     * Deserializes report data from binary file
     * @param file Input file stream for binary reading
     */
    void deserialize(ifstream& file) {
        file.read((char*)&reportId, sizeof(reportId));
        reportType      = FileHandler::readString(file);
        generationDate  = FileHandler::readString(file);
        content         = FileHandler::readString(file);
    }

    /**
     * Saves report to binary file
     * @param filename Path to output file
     */
    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            serialize(file);
            file.close();
        }
    }

    /**
     * Loads report from binary file
     * @param filename Path to input file
     */
    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (file.is_open()) {
            deserialize(file);
            file.close();
        }
    }
};

#endif