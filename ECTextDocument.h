#ifndef ECTextDocument_h
#define ECTextDocument_h
#include "ECCommand.h"
#include "ECFileIO.h"

using namespace std;
class ECTextDocument;

/*  ******************************************************
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

/* ECTextDocCtrl Class. This is the controller that can issue commands to the text
 document */
class ECTextDocumentCtrl {
public:
    ECTextDocumentCtrl(ECTextDocument &docIn);      // controller constructor takes the document as input
    virtual ~ECTextDocumentCtrl();
    void Backspace(int cx, int cy);
    void InsertChar(int cx, int cy, char c);        // Insert a character
    void Newline(int cx, int cy);                   // Insert a newline

    bool Undo();                                    // undo any change you did to the text
    bool Redo();                                    // redo the change to the text
    
private:
    ECTextDocument &doc;
    ECCommandHistory histCmds;

 };

/* ECDTextDocument Class. Stores all the text data for the document and allows commands
to be run */
class ECTextDocument {
    friend class InsertCharCommand;
    friend class BackspaceCommand;
    friend class NewlineCommand;
public:
    ECTextDocument(ECTextViewImp *textView, ECFileIO *fileIO);
    ~ECTextDocument();

    ECTextDocumentCtrl GetCtrl();       // Get the document controller.
    ECTextViewImp* GetTextView();        // Get the view
    int GetNumRows() const;             // Return the number of rows.
    int GetLenRow(int r) const;         // Return the length of a row. 0 if a row is empty.
    vector<string> GetAllRows();        // Return all the rows in the document as strings
    void AddRow(string row);            // Add a row to the document. Used for testing purposes

private:
    vector<vector<char>> document;
    ECTextDocumentCtrl docCtrl;
    ECTextViewImp *textView;
    ECFileIO *fileIO;
};

#endif