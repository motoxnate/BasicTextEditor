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
    UpdateStatusRow();
    // AddRow("\p");
    // AddRow("This is a second test row");
    UpdateTextDisplay();

    textView.Show();
}

ECTextEditor::~ECTextEditor() {}

// Update is called from the observer any time a key is pressed.
// First we parse they key, do something with it, then refresh.
void ECTextEditor::Update()
{
    ParseKeyCode(textView.GetPressedKey());
    UpdateStatusRow();
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
        docCtrl.InsertChar(c);
        int dy = textView.GetCursorY();
        StepCursorForward();
        dy = textView.GetCursorY() - dy;
        if(dy > 0) SetCursorWordEnd();
        UpdateTextDisplay();
    }
    else if (c == BACKSPACE)
    { // Backspace
        Backspace();
    }
    else if (c >= 0 && c <= 27)
    { // Special Characters
        switch (c)
        {
        case ENTER:
            docCtrl.Newline();
            StepCursorDown();
            document.SetCX(0);
            assert(UpdateCursor(0, textView.GetCursorY()));
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
            UpdateTextDisplay();
            break;
        case CTRL_Y:
            cerr << "Redo" << endl;
            docCtrl.Redo();
            UpdateTextDisplay();
            break;
        }
    }
    else if (c >= 1000)
    { // Special keys like arrow keys, del, etc.
        switch (c)
        {
        case ARROW_LEFT:
            cerr << "Arrow Left ";
            StepCursorBack();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            UpdateTextDisplay();
            break;
        case ARROW_RIGHT:
            cerr << "Arrow Right ";
            StepCursorForward();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            UpdateTextDisplay();
            break;
        case ARROW_UP:
            cerr << "Arrow Up ";
            StepCursorUp();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            UpdateTextDisplay();
            break;
        case ARROW_DOWN:
            cerr << "Arrow Down ";
            StepCursorDown();
            cerr << "x=" << textView.GetCursorX() << " y=" << textView.GetCursorY() << endl;
            UpdateTextDisplay();
            break;
        }
    }
}

/* Backspace command logic */
void ECTextEditor:: Backspace() {
    char prev = document.GetCharAt(textView.GetCursorX()-2, textView.GetCursorY());
    docCtrl.Backspace();
    if(document.GetCX() == 0)
        StepCursorUp();
    else StepCursorBack();
    int cx = textView.GetCursorX();
    int cy = textView.GetCursorY();
    // If the word moves back to the last line, it will happen on display update.
    // Check the character previous to the one removed. If it changes, then go to the end of the word.
    // cerr << "Prev: " << prev << " Update: ";
    UpdateTextDisplay();
    char prev2 = document.GetCharAt(textView.GetCursorX()-1, textView.GetCursorY());
    // cerr << prev2 << endl;
    // Error check: if x is now beyond the end of the row, must move back to prev
    if(cx > document.GetCurrentPage()[cy].size()) {
        if(cy >= 1) {
            // cerr << "Backspace fix" << endl;
            textView.SetCursorY(cy - 1);
            SetCursorLineEnd();
        }
    }
    else if(prev != prev2) {
        textView.SetCursorY(cy - 1);
        SetCursorLineEnd();
    }
}

/* Must step cursor forward on the document AND in the formatted text view */
bool ECTextEditor::StepCursorForward()
{
    int dy = document.GetCY();
    if(!document.StepCursorForward()) return false;
    dy = document.GetCY() - dy;
    int cx = textView.GetCursorX() + 1;
    int cy = textView.GetCursorY();
    // If cursor reaches end of window
    if (cx >= textView.GetColNumInView())
    {
        cx = 0;
        cy += 1;
    } // If cursor reaches end of line.
    else if (cx > GetLenRow(cy) || dy != 0)
    {
        // Cursor next line
        cx = 0;
        cy += 1;
    }   // Check Y position
    if (cy > textView.GetRowNumInView())
    {
        if(!document.NextPage()) {
            cerr << "Cursor out of bounds Y: " << cy << endl;
            return false;
        }
        cx = 0;
        cy = 0;
    }
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor ::StepCursorBack()
{
    if(!document.StepCursorBack()) return false;
    int dy = document.GetCY();
    int cx = textView.GetCursorX() - 1;
    int cy = textView.GetCursorY();
    // If the cursor has reached the beginning of a line.
    if (cx < 0)
    {
        cy -= 1;
        if (cy < 0)
        { // Cursor cannot go back furthers
            if(!document.PrevPage()) {
                cerr << "Cursor reached first character" << endl;
                return false;
            } else {
                cy = textView.GetRowNumInView();
                cx = GetLenRow(cy) - 1;
            }
        } // Set cursorX to last char of previous row
        else
        {
            if(document.GetAllRows()[dy].back() != ' ') {
                cx = GetLenRow(cy) - 1;
                cerr << "Step back with space at end of row" << endl;
            }
            else cx = GetLenRow(cy);
            if(cx < 0) cx = 0;
        }
    }
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor ::StepCursorUp()
{
    if(!document.StepCursorUp()) return false;
    int dy = document.GetCY();
    int cx = textView.GetCursorX();
    int cy = textView.GetCursorY() - 1;
    // If cursor has hit beginning of document
    if (cy < 0)
    {
        if(!document.PrevPage()) {
            cerr << "Cursor reached first line" << endl;
            return false;
        }   // Now on previous page
        else 
        {
            cy = textView.GetRowNumInView() - 1;
            if (GetLenRow(cy) < cx) {
                if(document.GetAllRows()[dy].back() != ' ') {
                    cx = GetLenRow(cy) - 1;
                }
            else cx = GetLenRow(cy);
            if(cx < 0) cx = 0;
            }
        }
    } // If cursor can move up, but the line is shorter than cursorX.
    else if (GetLenRow(cy) < cx)
    {
        if(document.GetAllRows()[dy].back() != ' ') {
                cx = GetLenRow(cy) - 1;
        }
        else cx = GetLenRow(cy);
        if(cx < 0) cx = 0;
    }
    // Otherwise the cursor just moves up
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor ::StepCursorDown()
{
    if(!document.StepCursorDown()) return false;
    int dy = document.GetCY();
    int cx = textView.GetCursorX();
    int cy = textView.GetCursorY() + 1;
    // If cursor has hit end of window
    // if (cy > textView.GetRowNumInView())
    // {
    //     cerr << "Cursor reached end of window" << endl;

    //     return false;
    // } // If cursor has hit the end of the document
    if (cy > document.GetNumRowsFormatted())
    {
        cerr << "Cursor has reached end of document" << endl;
        return false;
    }   // If cursor can move down, but the line is shorter than cursorX
    else if (document.GetCurrentPage()[cy].size() < cx)
    {
        if(document.GetAllRows()[dy].back() != ' ') {
            cx = document.GetCurrentPage()[cy].size() - 1;
        }
        else cx = document.GetCurrentPage()[cy].size();
        if(cx < 0) cx = 0;
    }
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor:: SetCursor(int x, int y) {
    if(!UpdateCursor(x, y)) return false;
    return true;
}

bool ECTextEditor ::SetCursorLineEnd()
{
    int cx = textView.GetCursorX();
    int cy = textView.GetCursorY();
    if(document.GetCurrentPage()[cy].back() == ' ') {
        cx = GetLenRow(cy) - 1;
        cerr << "Step back with space at end of row" << endl;
    }
    else cx = GetLenRow(cy);
    if(cx < 0) cx = 0;
    if (!UpdateCursor(cx, cy))
        return false;
    return true;
}

bool ECTextEditor:: SetCursorWordEnd() {
    int x = textView.GetCursorX();
    string line = document.GetCurrentPage()[textView.GetCursorY()];
    while(line[x] != ' ') x++;
    if (!UpdateCursor(x, textView.GetCursorY()))
        return false;
    return true;
}

// Update the cursor position, if valid.
bool ECTextEditor ::UpdateCursor(int cx, int cy)
{
    // Check Validity before updating cursor
    // if (cy > document.GetNumRows())
    if (cy > textView.GetRowNumInView() - 1)
    {
        cerr << "Go to next page: ";
        if(!document.NextPage()) return false;
        cy = 0;
        cerr << "Page = " << document.GetPageNum() << endl;
    }
    else if (cy > document.GetNumRowsFormatted()) {
        cerr << "Cursor out of bounds Y: " << cy << " for document size: " << document.GetNumRowsFormatted() << endl;
        return false;
    }
    else if (cx > document.GetCurrentPage()[cy].size())
    {
        cerr << "Cursor out of bounds X: " << cx << " for row length: " << document.GetLenRow(cy) << endl;
        return false;
    }
    // Update cursor position if valid
    textView.SetCursorX(cx);
    textView.SetCursorY(cy);
    return true;
}

int ECTextEditor:: GetNumRows() {
    return document.GetCurrentPage().size();
}

int ECTextEditor:: GetLenRow(int y) {
    if (y > GetNumRows()) return 0;
    return document.GetCurrentPage()[y].size();
}

/* Update the status row in the textView */
void ECTextEditor:: UpdateStatusRow() {
    string baseLeft = "TextView Cursor: ";
    string baseRight = "TextDoc Cursor: ";
    StatusLeft = baseLeft + to_string(textView.GetCursorX()) + "," + to_string(textView.GetCursorY());
    StatusRight = baseRight + to_string(document.GetCX()) + "," + to_string(document.GetCY());
    textView.ClearStatusRows();
    textView.AddStatusRow(StatusLeft, StatusRight, false);
}

/* Clear the view entirely. All all rows from the document one by one. */
void ECTextEditor ::UpdateTextDisplay()
{
    textView.InitRows();
    formattedPage.clear();
    formattedPage.resize(0);
    for (string s : document.GetCurrentPage()) {
        textView.AddRow(s);
        formattedPage.push_back(s);
    }
    return;
}

// Add a row to the document. Used for testing.
void ECTextEditor ::AddRow(string row)
{
    document.AddRow(row);
    UpdateTextDisplay();
    textView.Refresh();
}
