#include "ECFileIO.h"
using namespace std;

/* Class ECFileIO */
// Private methods
ECFileIO:: ECFileIO(string path) {
    if(path == "None") {
        cerr << "No filename" << endl;
        exit(EXIT_FAILURE);
    } else {
        file.open(path, fstream::in | fstream::out);
        if(file.fail()) {   // File opening error checking
            // If file could not be opened, try creating it.
            file.clear();
            file.open(path, ios::out);
            if(file.fail()) {
                cerr << "File could not be created" << endl;
                exit(EXIT_FAILURE);
            }
            file.close();
            file.clear();
            file.open(path, fstream::in | fstream::out);
            if(file.fail()) {
                cerr << "File could not be opened after creation" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}

ECFileIO:: ~ECFileIO() {
    file.flush();
    file.close();
}

void ECFileIO::write(vector<string> output) {
    for(string line : output) {
        file << line << endl;
    }
}

vector<string> ECFileIO:: read() {
    vector<string> intext;
    string line;
    while(getline(file, line)) {
        if(file.fail()) {
            cerr << "Read from file failed" << endl;
            exit(EXIT_FAILURE);
        }
        intext.push_back(line);
    }
    return intext;
}
