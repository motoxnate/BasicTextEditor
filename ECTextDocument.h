#ifndef ECTextDocument_h
#define ECTextDocument_h
#include "ECCommand.h"
#include "ECFileIO.h"
#include <sstream>

using namespace std;
class ECTextDocument;

/*  ******************************************************
    Implement Commands */

class InsertCharCommand : public ECCommand {
public: 
    InsertCharCommand(ECTextDocument *doc, char c);      // Move the cursor to the specified position
    ~InsertCharCommand();
    void Execute();
    void UnExecute();
private:
    int cx, cy;
    int origCX, origCY, origViewCX, origViewCY;
    char c;
    vector<vector<char>> origDocument;
};

class BackspaceCommand : public ECCommand {
public:
    BackspaceCommand(ECTextDocument *doc);
    ~BackspaceCommand();
    void Execute();
    void UnExecute();
private:
    int cx, cy;
    int origCX, origCY, origViewCX, origViewCY;
    vector<vector<char>> origDocument;
};

class NewlineCommand : public ECCommand {
public:
    NewlineCommand(ECTextDocument *doc);
    ~NewlineCommand();
    void Execute();
    void UnExecute();
private:
    int cx, cy;
    int origCX, origCY, origViewCX, origViewCY;
    vector<vector<char>> origDocument;

};

/* ECTextDocCtrl Class. This is the controller that can issue commands to the text
 document */
class ECTextDocumentCtrl {
public:
    ECTextDocumentCtrl(ECTextDocument &docIn);      // controller constructor takes the document as input
    virtual ~ECTextDocumentCtrl();
    void Backspace();
    void InsertChar(char c);        // Insert a character
    void Newline();                   // Insert a newline

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
    ECTextViewImp* GetTextView();       // Get the view

    int GetNumRows() const;             // Return the number of rows.
    int GetNumRowsFormatted() const;    // Return the number of rows after formatting.
    int GetLenRow(int r) const;         // Return the length of a row. 0 if a row is empty.
    int GetLenRowFormatted(int r) const;     // Return the length of a row after formatting.
    int GetNumPages() const;            // Return the number of pages
    bool NextPage();                    // Go to the next page
    bool PrevPage();                    // Go to the previous page
    int GetPageNum();                   // Return the current page number.
    int GetCX();                        // Get cursor x.
    int GetCY();                        //Get cursor y
    bool SetCX(int x);                  // Set cursor x
    bool SetCY(int y);                  // Set cursor y
    bool StepCursorForward();           // Step the document cursor forward
    bool StepCursorBack();              // Step the document cursor back
    bool StepCursorUp();                // Step the document cursor up
    bool StepCursorDown();              // Step the document cursor down
    vector<string> GetAllRows();        // Return all the rows in the document as strings
    void AddRow(string row);            // Add a row to the document. Used for testing purposes
    void FormatDocument();              // Format the document text and store in formattedDocument
    vector<string> GetFormattedDocument();    // Returns the formatted document ready for display
    vector<string> GetCurrentPage();    // Returns the formatted text to display a page.

private:
    int cx, cy;                         // Cursor X and Y of the UNFORMATTED document. This is where edits need to happen.
    vector<vector<char>> document;
    vector<string> formattedDocument;   // The entire formatted document
    vector<string> currentPage;         // The current formatted page of the document
    int pages;
    int pageNo;
    ECTextDocumentCtrl docCtrl;
    ECTextViewImp *textView;
    ECFileIO *fileIO;

    bool UpdateCursor(int x, int y);                // Try to set the document cursor
    void Newlines();                    // Search for newlines. Testing
};

#endif