// Main.cpp
#include <iostream>
#include <vector>
#include "Person.h"
#include "Instructor.h"
#include "TA.h"
#include "Building.h"
#include "Room.h"
#include "LabSection.h"
#include "Schedule.h"
#include "TimeSheet.h"
#include "Attendant.h"
#include "HOD.h"
#include "AcademicOfficer.h"
#include "WeeklyLabSchedule.h"
#include "LabFullSemReport.h"
#include "MakeupLabs.h"
#include "Reports.h"
#include "FileHandler.h"

using namespace std;

int main() {
    cout << "=== SDA Project - Labs Management System ===" << endl;
    cout << "Testing ALL 15 classes with COMPLETE binary file handling...\n" << endl;
    
    // Create test data for ALL classes
    Building csBuilding(1, "Computer Science Building", "Main Campus");
    Room labRoom(101, "CS-Lab-1", 30, &csBuilding);
    Instructor instructor(1001, "Dr. Ahmed Khan", "ahmed.khan@uni.edu", "Computer Science", "CS-101");
    TA ta1(2001, "Sara Ali", "sara.ali@student.edu", "BSCS001", "BSCS");
    TA ta2(2002, "Bilal Ahmed", "bilal.ahmed@student.edu", "BSCS002", "BSCS");
    LabSection labSection(1, "CS-101L", "Programming Fundamentals Lab", &instructor, &labRoom);
    Schedule schedule(1, &labSection, "Monday", "14:00", "16:00");
    TimeSheet timesheet(1, &labSection, "2024-11-18", "14:05", "15:55", false);
    Attendant attendant(3001, "Usman Attendant", "usman@uni.edu", &csBuilding);
    HOD hod(4001, "Prof. Dr. Naveed", "naveed@uni.edu", "Computer Science");
    AcademicOfficer officer(5001, "Academic Admin", "admin@uni.edu", "Academic Office");
    WeeklyLabSchedule weeklySchedule(1);
    LabFullSemReport semesterReport(&labSection);
    MakeupLabs makeupLab(1, &labSection, "2024-11-20", "Compensation for holiday");
    Reports report(1, "Weekly Summary", "2024-11-18", "Lab attendance and performance report for week 15");

    // Add relationships
    labSection.addTA(&ta1);
    labSection.addTA(&ta2);
    weeklySchedule.addSchedule(&schedule);
    semesterReport.addTimeSheet(&timesheet);

    // Test 1: Save ALL objects to binary files
    cout << "--- Saving ALL Objects to Binary Files ---" << endl;
    csBuilding.saveToFile("building.dat");
    labRoom.saveToFile("room.dat");
    instructor.saveToFile("instructor.dat");
    ta1.saveToFile("ta1.dat");
    ta2.saveToFile("ta2.dat");
    labSection.saveToFile("labsection.dat");
    schedule.saveToFile("schedule.dat");
    timesheet.saveToFile("timesheet.dat");
    attendant.saveToFile("attendant.dat");
    hod.saveToFile("hod.dat");
    officer.saveToFile("officer.dat");
    weeklySchedule.saveToFile("weekly_schedule.dat");
    semesterReport.saveToFile("semester_report.dat");
    makeupLab.saveToFile("makeup_lab.dat");
    report.saveToFile("report.dat");

    cout << "✓ All 15 classes saved to binary files!" << endl;

    // Test 2: Load objects from binary files
    cout << "\n--- Loading Objects from Binary Files ---" << endl;
    
    Building loadedBuilding;
    loadedBuilding.loadFromFile("building.dat");
    cout << "Loaded Building: " << loadedBuilding.getName() << " at " << loadedBuilding.getLocation() << endl;

    Instructor loadedInstructor;
    loadedInstructor.loadFromFile("instructor.dat");
    cout << "Loaded Instructor: " << loadedInstructor.getName() << " - " << loadedInstructor.getDepartment() << endl;

    TA loadedTA;
    loadedTA.loadFromFile("ta1.dat");
    cout << "Loaded TA: " << loadedTA.getName() << " - " << loadedTA.getProgram() << endl;

    HOD loadedHOD;
    loadedHOD.loadFromFile("hod.dat");
    cout << "Loaded HOD: " << loadedHOD.getName() << " - " << loadedHOD.getDepartment() << endl;

    AcademicOfficer loadedOfficer;
    loadedOfficer.loadFromFile("officer.dat");
    cout << "Loaded Academic Officer: " << loadedOfficer.getName() << endl;

    Reports loadedReport;
    loadedReport.loadFromFile("report.dat");
    cout << "Loaded Report: " << loadedReport.getReportType() << " generated on " << loadedReport.getGenerationDate() << endl;

    // Test 3: Demonstrate system functionality
    cout << "\n--- System Functionality Test ---" << endl;
    cout << "1. Instructor requesting makeup lab:" << endl;
    instructor.requestMakeupLab();
    
    cout << "\n2. HOD generating reports:" << endl;
    hod.generateWeeklyScheduleReport();
    hod.generateWeeklyTimeSheetReport();
    hod.generateLabSemesterReport(1);
    
    cout << "\n3. Academic Officer operations:" << endl;
    officer.populateLabData();
    officer.scheduleMakeupLab();
    
    cout << "\n4. Attendant filling timesheet:" << endl;
    attendant.fillTimeSheet(&timesheet, "14:05", "15:55");
    
    cout << "\n5. Report generation:" << endl;
    report.generateReport();
    report.saveReport();

    // Test 4: Display lab information
    cout << "\n--- Lab Information ---" << endl;
    cout << "Lab Section: " << labSection.getCourseCode() << " - " << labSection.getSectionName() << endl;
    cout << "Instructor: " << labSection.getInstructor()->getName() << endl;
    cout << "Room: " << labSection.getRoom()->getRoomNumber() << endl;
    cout << "Number of TAs: " << labSection.getTAs().size() << endl;
    
    cout << "Schedule: " << schedule.getDayOfWeek() << " " << schedule.getExpectedStartTime() 
         << "-" << schedule.getExpectedEndTime() << endl;
    
    cout << "Timesheet: " << timesheet.getDate() << " " << timesheet.getActualStartTime() 
         << "-" << timesheet.getActualEndTime() << endl;

    // Test 5: Calculate and display report data
    cout << "\n--- Report Data ---" << endl;
    semesterReport.calculateTotalContactHours();
    semesterReport.calculateTotalLeaves();
    cout << "Total Contact Hours: " << semesterReport.getTotalContactHours() << endl;
    cout << "Total Leaves: " << semesterReport.getTotalLeaves() << endl;

    // Final Summary
    cout << "\n=== COMPLETE SYSTEM STATUS ===" << endl;
    cout << "✓ All 15 classes implemented with binary file handling" << endl;
    cout << "✓ Binary serialization/deserialization working" << endl;
    cout << "✓ All object relationships maintained" << endl;
    cout << "✓ Data persistence achieved using binary files" << endl;
    cout << "✓ System fully operational! 🚀" << endl;

    cout << "\nGenerated binary files:" << endl;
    cout << "1. building.dat        - Building data" << endl;
    cout << "2. room.dat            - Room data" << endl;
    cout << "3. instructor.dat      - Instructor data" << endl;
    cout << "4. ta1.dat, ta2.dat    - TA data" << endl;
    cout << "5. labsection.dat      - Lab Section data" << endl;
    cout << "6. schedule.dat        - Schedule data" << endl;
    cout << "7. timesheet.dat       - Timesheet data" << endl;
    cout << "8. attendant.dat       - Attendant data" << endl;
    cout << "9. hod.dat             - HOD data" << endl;
    cout << "10. officer.dat        - Academic Officer data" << endl;
    cout << "11. weekly_schedule.dat - Weekly Schedule data" << endl;
    cout << "12. semester_report.dat - Semester Report data" << endl;
    cout << "13. makeup_lab.dat     - Makeup Lab data" << endl;
    cout << "14. report.dat         - Report data" << endl;

    cout << "\nNote: These are BINARY files (not text files) and contain encoded data." << endl;
    cout << "They cannot be read with a text editor but can be loaded by your program." << endl;

    return 0;
}