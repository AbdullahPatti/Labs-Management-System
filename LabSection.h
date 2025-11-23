#ifndef LABSECTION_H
#define LABSECTION_H

#include <vector>
#include <fstream>
#include "FileHandler.h"
class Instructor;
class TA;
class Room;
using namespace std;

/**
 * LabSection class represents individual lab course sections
 * 
 * Responsibilities:
 * - Store lab section course information
 * - Manage instructor and TA assignments
 * - Track room assignments and capacity
 * - Coordinate schedules and timesheets
 * 
 * Core entity linking: Course + Instructor + Room + TAs
 */
class LabSection {
    int              sectionId;
    string           courseCode;
    string           sectionName;
    Instructor*      instructor;
    vector<TA*>      tas;
    Room*            room;
    
public:
    LabSection() : sectionId(0), courseCode(""), sectionName(""), instructor(nullptr), room(nullptr) {}
    
    /**
     * Creates a new lab section
     * @param sid Unique section identifier
     * @param ccode Course code (e.g., "CS-101L")
     * @param sname Section name/identifier
     * @param instr Pointer to assigned Instructor
     * @param r Pointer to assigned Room
     */
    LabSection(int sid, string ccode, string sname, Instructor* instr, Room* r)
        : sectionId(sid), courseCode(ccode), sectionName(sname), instructor(instr), room(r) {}

    // Getters
    int         getSectionId()   const { return sectionId; }
    string      getCourseCode()  const { return courseCode; }
    string      getSectionName() const { return sectionName; }
    Instructor* getInstructor()  const { return instructor; }
    Room*       getRoom()        const { return room; }
    vector<TA*> getTAs()         const { return tas; }

    /**
     * Adds a TA to this lab section
     * @param t Pointer to TA object to add
     */
    void addTA(TA* t) { tas.push_back(t); }

    // Persistence helper methods for link reconstruction
    
    /**
     * Sets instructor reference during deserialization
     * @param i Pointer to Instructor object
     */
    void _setInstructor(Instructor* i) { instructor = i; }
    
    /**
     * Sets room reference during deserialization
     * @param r Pointer to Room object
     */
    void _setRoom(Room* r)            { room = r; }
    
    /**
     * Clears TA list before reconstruction
     */
    void _clearTAs()                  { tas.clear(); }
    
    /**
     * Adds TA during link reconstruction
     * @param t Pointer to TA object
     */
    void _addTA(TA* t)                { tas.push_back(t); }

    /**
     * Serializes lab section data to binary file
     * @param f Output file stream for binary writing
     */
    void serialize(ofstream& f) const {
        f.write((char*)&sectionId, sizeof(sectionId));
        FileHandler::writeString(f, courseCode);
        FileHandler::writeString(f, sectionName);
        int iid = instructor ? instructor->getId() : -1;
        f.write((char*)&iid, sizeof(iid));
        int rid = room ? room->getRoomId() : -1;
        f.write((char*)&rid, sizeof(rid));
        size_t sz = tas.size();
        f.write((char*)&sz, sizeof(sz));
        for (TA* t : tas) {
            int tid = t ? t->getId() : -1;
            f.write((char*)&tid, sizeof(tid));
        }
    }

    /**
     * Saves lab section to binary file
     * @param filename Path to output file
     */
    void saveToFile(const string& filename) const {
        ofstream f(filename, ios::binary);
        if (f.is_open()) serialize(f);
    }

    /**
     * Deserializes lab section data from binary file
     * @param f Input file stream for binary reading
     */
    void deserialize(ifstream& f) {
        f.read((char*)&sectionId, sizeof(sectionId));
        courseCode  = FileHandler::readString(f);
        sectionName = FileHandler::readString(f);
        int iid, rid;
        f.read((char*)&iid, sizeof(iid));
        f.read((char*)&rid, sizeof(rid));
        instructor = nullptr;   // rebuilt later during link reconstruction
        room       = nullptr;   // rebuilt later during link reconstruction
        size_t tasCnt;
        f.read((char*)&tasCnt, sizeof(tasCnt));
        tas.clear();
        for (size_t i = 0; i < tasCnt; ++i) {
            int tid; f.read((char*)&tid, sizeof(tid));
            tas.push_back(nullptr); // placeholder, rebuilt later during link reconstruction
        }
    }
};

#endif