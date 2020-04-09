#ifndef ECCommand_h
#define ECCommand_h

#include <vector>
#include <stack>
#include "ECTextViewImp.h"

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

/* ******************************************************
Implement Commands */

class InsertCharCommand : public ECCommand {
public: 
    InsertCharCommand(ECTextDocument *doc, int cursorX, int cursorY, char c);      // Move the cursor to the specified position
    ~InsertCharCommand();
    void Execute();
    void UnExecute();
private:
    int cx, cy;
    int origCX, origCY;
    char c;
    vector<vector<char>> origDocument;
};

class BackspaceCommand : public ECCommand {
public:
    BackspaceCommand(ECTextDocument *doc, int cx, int cy);
    ~BackspaceCommand();
    void Execute();
    void UnExecute();
private:
    int cx, cy;
    int origCX, origCY;
    vector<vector<char>> origDocument;
};

class NewlineCommand : public ECCommand {
public:
    NewlineCommand(ECTextDocument *doc, int cx, int cy);
    ~NewlineCommand();
    void Execute();
    void UnExecute();
private:
    int cx, cy;
    int origCX, origCY;
    vector<vector<char>> origDocument;

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