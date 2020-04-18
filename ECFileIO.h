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
    void write(vector<string> output);      // Write contents of output to file
    vector<string> read();                  // Read contents of file

private:
    fstream file;

};

#endif