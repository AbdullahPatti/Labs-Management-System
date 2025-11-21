#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <fstream>
#include <string>
using namespace std;

class FileHandler {
public:
    static void writeString(ofstream& f, const string& s) {
        int len = s.length();
        f.write((char*)&len, sizeof(len));
        f.write(s.c_str(), len);
    }
    static string readString(ifstream& f) {
        int len;  f.read((char*)&len, sizeof(len));
        char* buf = new char[len + 1];
        f.read(buf, len);  buf[len] = '\0';
        string r(buf);  delete[] buf;  return r;
    }
};
#endif