#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class FileHandler {
public:
    static void writeString(ofstream& file, const string& str) {
        int length = str.length();
        file.write((char*)&length, sizeof(length));
        file.write(str.c_str(), length);
    }

    static string readString(ifstream& file) {
        int length;
        file.read((char*)&length, sizeof(length));
        char* buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';
        string result(buffer);
        delete[] buffer;
        return result;
    }
};

#endif