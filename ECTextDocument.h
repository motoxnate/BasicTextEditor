#ifndef ECTextDocument_h
#define ECTextDocument_h
#include "ECCommand.h"

using namespace std;
class ECTextDocument;

/* ECTextDocCtrl Class. This is the controller that can issue commands to the text
 document */
class ECTextDocumentCtrl {
public:
    ECTextDocumentCtrl(ECTextDocument &docIn);      // controller constructor takes the document as input
    virtual ~ECTextDocumentCtrl();
    void Backspace(int cx, int cy);
    void InsertChar(int cx, int cy, char c);        // Insert a character

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
public:
    ECTextDocument(ECTextViewImp *textView);
    ~ECTextDocument() {}

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
};

#endif