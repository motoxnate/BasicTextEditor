/*  ECFileIO
    Written by: Nate Fanning */
    
#ifndef ECFileIO_h
#define ECFileIO_h

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class ECFileIO {
public:
    ECFileIO(string fname);
    ~ECFileIO();
    vector<string> read();                  // Read contents of file
    void write(vector<string> output);      // Write contents of output to file

private:
    string path;
    fstream file;

    void readMode();        // Open the file for reading
    void writeMode();       // Open the file for writing
};

#endif