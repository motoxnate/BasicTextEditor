/*  ECTextEditor
    Written by: Nate Fanning */
#ifndef ECTextEditor_h
#define ECTextEditor_h

#include "ECTextViewImp.cpp"
#include "ECTextDocument.cpp"
#include "ECCommand.cpp"

class ECTextEditor : public ECObserver
{
public:
    ECTextEditor(ECTextViewImp &view, ECTextDocument &doc, ECTextDocumentCtrl &ctrl);
    ~ECTextEditor();

    void Update();

private:
    void ParseKeyCode(int c);
    bool StepCursorForward();
    bool StepCursorBack();
    bool StepCursorUp();
    bool StepCursorDown();
    bool SetCursorLineEnd();           // Used to place the cursor after undo / redo.
    bool UpdateCursor(int cx, int cy); // Update cursor position if valid.

    void UpdateTextDisplay(); // Add the document text to the view.
    void AddRow(string row);  // Add a new row for testing purposes.

    ECTextViewImp &textView;     // The text view instance displays the text.
    ECTextDocument &document;    // The TextDocument holds all the text data.
    ECTextDocumentCtrl &docCtrl; // The Document Controller controls the text document.
};

#endif