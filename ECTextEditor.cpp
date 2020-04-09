/*  ECTextEditor
    Written by: Nate Fanning */

#include <iostream>
#include "ECTextEditor.h"

using namespace std;

// Definiitions for ECTextEditor Class
// Public Functions
ECTextEditor::ECTextEditor(ECTextViewImp &view, ECTextDocument &doc, ECTextDocumentCtrl &ctrl)
    : textView(view), document(doc), docCtrl(ctrl)
{
    // Add the editor to the controller
    // docCtrl.AddEditor(this);
    // Attach to the text view observer
    textView.Attach(this);

    textView.AddStatusRow("Test Left Message", "Test Right Message", false);
    AddRow("This is a test row");
    AddRow("This is a second test row");
    // cerr << "Test TextEditor::AddRow()" << endl;
    // cerr << "Document: " << document.GetAllRows().size() << endl;
    textView.Show();
}

ECTextEditor::~ECTextEditor() {}

// Update is called from the observer any time a key is pressed.
// First we parse they key, do something with it, then refresh.
void ECTextEditor::Update()
{
    cout << "Update Called" << endl;
    ParseKeyCode(textView.GetPressedKey());
    textView.Refresh();
    return;
}

// *********************************************
// Private Functions

void ECTextEditor::ParseKeyCode(int c)
{
    // Error check: has the char been gotten correctly?
    if (c == -1 || c == KEY_NULL)
    {
        cerr << "Invalid Key Press" << endl;
    }
    else if (c >= 32 && c <= 126)
    { // Letters and characters
        docCtrl.InsertChar(textView.GetCursorX(), textView.GetCursorY(), c);
        StepCursorForward();
        UpdateTextDisplay();
    }
    else if (c == BACKSPACE)
    { // Backspace
        docCtrl.Backspace(textView.GetCursorX(), textView.GetCursorY());
        UpdateTextDisplay();
    }
    else if (c >= 0 && c <= 27)
    { // Special Characters
        switch (c)
        {
        case ENTER:
            docCtrl.Newline(textView.GetCursorX(), textView.GetCursorY());
            UpdateTextDisplay();
            break;
        case CTRL_Q: // This is here because of the specification, but will never be called
            cerr << "Quit" << endl;
            textView.Quit();
            textView.InitRows();
            system("clear");
            break;
        case CTRL_Z:
            cerr << "Undo" << endl;
            docCtrl.Undo();
            // SetCursorLineEnd();
            UpdateTextDisplay();
            break;
        case CTRL_Y:
            cerr << "Redo" << endl;
            docCtrl.Redo();
            // SetCursorLineEnd();
            UpdateTextDisplay();
            break;
        }
    }
    else if (c >= 1000)
    { // Special keys like arrow keys, del, etc.
        switch (c)
        {
        case ARROW_LEFT:
            cerr << "Arrow Left" << endl;
            StepCursorBack();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            break;
        case ARROW_RIGHT:
            cerr << "Arrow Right ";
            StepCursorForward();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            break;
        case ARROW_UP:
            cerr << "Arrow Up" << endl;
            StepCursorUp();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            break;
        case ARROW_DOWN:
            cerr << "Arrow Down" << endl;
            StepCursorDown();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            break;
        }
    }
}

bool ECTextEditor::StepCursorForward()
{
    int cursorX = textView.GetCursorX() + 1;
    int cursorY = textView.GetCursorY();
    // If cursor reaches end of window
    if (cursorX > textView.GetColNumInView())
    {
        cursorX = 0;
        cursorY += 1;
    } // If cursor reaches end of line.
    else if (cursorX > document.GetLenRow(cursorY))
    {
        // Cursor next line
        cursorX = 0;
        cursorY += 1;
    } // Check Y position
    if (cursorY > textView.GetRowNumInView())
    {
        cerr << "Cursor out of bounds Y: " << cursorY << endl;
        return false;
    }
    if (!UpdateCursor(cursorX, cursorY))
        return false;
    return true;
}

bool ECTextEditor ::StepCursorBack()
{
    int cx = textView.GetCursorX() - 1;
    int cy = textView.GetCursorY();
    // If the cursor has reached the beginning of a line.
    if (cx < 0)
    {
        cy -= 1;
        if (cy < 0)
        { // Cursor cannot go back furthers
            cerr << "Cursor reached first character" << endl;
            return false;
        } // Set cursorX to last char of previous row
        else
        {
            cx = document.GetLenRow(cy);
        }
    }
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor ::StepCursorUp()
{
    int cx = textView.GetCursorX();
    int cy = textView.GetCursorY() - 1;
    // If cursor has hit beginning of document
    if (cy < 0)
    {
        cerr << "Cursor reached first line" << endl;
        return false;
    } // If cursor can move up, but the line is shorter than cursorX.
    else if (document.GetLenRow(cy) < cx)
    {
        cx = document.GetLenRow(cy);
    }
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor ::StepCursorDown()
{
    int cx = textView.GetCursorX();
    int cy = textView.GetCursorY() + 1;
    // If cursor has hit end of window
    if (cy > textView.GetRowNumInView())
    {
        cerr << "Cursor reached end of window" << endl;
        return false;
    } // If cursor has hit the end of the document
    else if (cy > document.GetNumRows())
    {
        cerr << "Cursor has reached end of document" << endl;
        return false;
    } // If cursor can move down, but the line is shorter than cursorX
    else if (document.GetLenRow(cy) < cx)
    {
        cx = document.GetLenRow(cy);
    }
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor ::SetCursorLineEnd()
{
    int cx = textView.GetCursorX();
    int cy = textView.GetCursorY();
    cx = document.GetLenRow(cy);
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

// Update the cursor position, if valid.
bool ECTextEditor ::UpdateCursor(int cx, int cy)
{
    // Check Validity before updating cursor
    if (cy > document.GetNumRows())
    {
        cerr << "Cursor out of bounds Y: " << cy << endl;
        return false;
    }
    else if (cx > document.GetLenRow(cy))
    {
        cerr << "Cursor out of bounds X: " << cx << " for row length: " << document.GetLenRow(cy) << endl;
        return false;
    }
    // Update cursor position if valid
    textView.SetCursorX(cx);
    textView.SetCursorY(cy);
    return true;
}

void ECTextEditor ::UpdateTextDisplay()
{
    textView.InitRows();
    for (string s : document.GetAllRows())
        textView.AddRow(s);
    return;
}

// Add a row to the document. Used for testing.
void ECTextEditor ::AddRow(string row)
{
    document.AddRow(row);
    UpdateTextDisplay();
    textView.Refresh();
}

// *********************************************
// Main Sequence

int main(int argc, char *argv[])
{
    cerr << "Program Start" << endl;
    // Initialize items
    ECTextViewImp textView;
    ECTextDocument textDocument(&textView);
    ECTextDocumentCtrl textDocumentCtrl = textDocument.GetCtrl();

    ECTextEditor textEditor(textView, textDocument, textDocumentCtrl);
    return 0;
}
