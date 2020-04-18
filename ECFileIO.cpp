#include "ECFileIO.h"
using namespace std;

/* Class ECFileIO */
// Private methods
ECFileIO:: ECFileIO(string path) {
    this->path = path;
    if(path == "None") {
        cerr << "No filename" << endl;
        exit(EXIT_FAILURE);
    } else {
        readMode();
    }
}

ECFileIO:: ~ECFileIO() {
    file.flush();
    file.close();
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

void ECFileIO::write(vector<string> output) {
    writeMode();
    file.clear();
    for(string line : output) {
        if(file.fail()) {
            if(!file.is_open()) cerr << "File is not open" << endl;
            cerr << "Failed writing output to file" << endl;
            exit(EXIT_FAILURE);
        }
        file << line << endl;
    }
}

// Private
void ECFileIO:: readMode() {
    if(file.is_open()) file.close();
    file.clear();
    file.open(path, fstream::in);
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
        file.open(path, fstream::in);
        if(file.fail()) {
            cerr << "File could not be opened after creation" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

void ECFileIO:: writeMode() {
    if(file.is_open()) file.close();
    file.clear();
    file.open(path, fstream::out | fstream::trunc);
    if(file.fail()) {
        cerr << "File could not be opened for writing" << endl;
        exit(EXIT_FAILURE);
    }
}