#ifndef ECCommand_h
#define ECCommand_h

#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include "ECTextViewImp.h"
using namespace std;

class ECTextDocument;

class ECCommand
{
public:
    ECCommand(ECTextDocument *doc, vector<vector<char>> &docRef);
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
protected:
    ECTextDocument *doc;
    ECTextViewImp *textView;
    vector<vector<char>> &document;
};

// ******************************************************
// Implement command history

class ECCommandHistory
{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    bool Undo();
    bool Redo();
    void ExecuteCmd( ECCommand *pCmd );
    
private:
    // your code goes here
    // Do these need to be pointers?
    std::stack<ECCommand *> history;
    std::stack<ECCommand *> future;
};


#endif /* ECCommand_h */