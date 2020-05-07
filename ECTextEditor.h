/*  ECTextEditor
    Written by: Nate Fanning */
#ifndef ECTextEditor_h
#define ECTextEditor_h

#include "ECTextViewImp.h"
#include "ECTextDocument.h"
#include "ECCommand.h"
#include <assert.h>

class ECTextEditor : public ECObserver
{
public:
    ECTextEditor(ECTextViewImp &view, ECTextDocument &doc, ECTextDocumentCtrl &ctrl);
    ~ECTextEditor();

    void Update();

private:
    void ParseKeyCode(int c);
    void Backspace();                   // For key code backspace

    bool StepCursorForward();
    bool StepCursorBack();
    bool StepCursorUp();
    bool StepCursorDown();
    bool SetCursor(int x, int y);       // Set the cursor to the desired position
    bool SetCursorLineEnd();            // Set the cursor to the end of the current line.
    bool SetCursorWordEnd();            // Set the cursor to the end of the current word.
    bool UpdateCursor(int cx, int cy);  // Update cursor position if valid.

    int GetNumRows();                   // Get num rows on the current page
    int GetLenRow(int y);               // Get the length of a row on the current page

    void UpdateStatusRow();             // Update the status row
    void UpdateTextDisplay();           // Add the document text to the view.
    void AddRow(string row);            // Add a new row for testing purposes.

    ECTextViewImp &textView;        // The text view instance displays the text.
    ECTextDocument &document;       // The TextDocument holds all the text data.
    ECTextDocumentCtrl &docCtrl;    // The Document Controller controls the text document.
    
    vector<string> formattedPage;  // The formatted page in view
    string StatusLeft;          // Status row at the end of the view.
    string StatusRight;
};

#endif