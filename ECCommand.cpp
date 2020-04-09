/*  Implement Command History
    Created by Nate Fanning on 4/5/2020 */

#include "ECCommand.h"
using namespace std;

// ******************************************************
// Implement Commands

// ECCommand
ECCommand ::ECCommand(ECTextDocument *doc, vector<vector<char>> &docRef)
    : document(docRef)
{
    this->doc = doc;
    textView = doc->GetTextView();
}

// –––––––––––––––––––
// InsertCharCommand
InsertCharCommand ::InsertCharCommand(ECTextDocument *doc, int cursorX, int cursorY, char c)
    : ECCommand(doc, doc->document)
{
    this->cx = cursorX;
    this->cy = cursorY;
    this->c = c;
}

InsertCharCommand ::~InsertCharCommand()
{
    delete doc;
}

void InsertCharCommand ::Execute()
{
    // Store for undo
    origCX = cx;
    origCY = cy;
    origDocument = document;
    // If the line does not yet exist, add a new line
    if ((doc->GetNumRows() == 0) || (cy == document.size()))
    {
        vector<char> vec;
        document.push_back(vec);
    }
    document[cy].insert(document[cy].begin() + cx, c);
    textView->SetCursorX(cx);
    textView->SetCursorY(cy);
}

void InsertCharCommand ::UnExecute()
{
    document = origDocument;
    textView->SetCursorX(origCX);
    textView->SetCursorY(origCY);
    cx = origCX;
    cy = origCY;
}

// –––––––––––––––––––
// BackspaceCommand
BackspaceCommand::BackspaceCommand(ECTextDocument *doc, int cx, int cy)
    : ECCommand(doc, doc->document)
{
    this->cx = cx;
    this->cy = cy;
}

BackspaceCommand ::~BackspaceCommand()
{
    delete doc;
}

void BackspaceCommand::Execute()
{
    origCX = cx;
    origCY = cy;
    origDocument = document;
    cx -= 1;
    // If the cursor has reached the beginning of a line.
    if (cx < 0) // cx is < 0 and must be reset to a correct integer
    {
        cy -= 1;
        if (cy < 0)
        { // Cursor cannot go back further
            cerr << "Cursor reached first character" << endl;
            return;
        }
        // Cursor can go back to the previous line
        // Set cx to the end of the previous line
        cx = doc->GetLenRow(cy);
        // Check if the cursor was on the last line of the document.
        if (!(cy + 1 == doc->GetNumRows()))
        {
            // If line not empty, copy the remaining text to the row above
            if (!document[cy + 1].empty())
            {
                document[cy].insert(document[cy].begin() + cx,
                                    document[cy + 1].begin(),
                                    document[cy + 1].end());
            }
        // Erase the line
        document.erase(document.begin() + cy + 1);
        }
    }
    else // If a line was not removed, remove a character
    {
        document[cy].erase(document[cy].begin() + cx);
    }
    textView->SetCursorX(cx);
    textView->SetCursorY(cy);
}

void BackspaceCommand::UnExecute()
{
    textView->SetCursorX(origCX);
    textView->SetCursorY(origCY);
    cx = origCX;
    cy = origCY;
    document = origDocument;
}

// –––––––––––––––––––
// NewlineCommand
NewlineCommand ::NewlineCommand(ECTextDocument *doc, int cx, int cy)
    : ECCommand(doc, doc->document)
{
    this->cx = cx;
    this->cy = cy;
}

NewlineCommand ::~NewlineCommand()
{
    delete doc;
}

void NewlineCommand ::Execute()
{
    origCX = cx;
    origCY = cy;
    origDocument = document;

    // If the cursor is on the last line, just append a blank line
    if (cy == document.size())
    {
        vector<char> newline;
        document.push_back(newline);
        cy += 1;
    }
    else //The cursor is not on the last line
    {
        // Copy remaining characters from after cursor to newline
        vector<char> newline(document[cy].begin() + cx, document[cy].end());
        // Erase those characters from the line
        document[cy].erase(document[cy].begin() + cx, document[cy].end());
        // Move cursor to the next line.
        cy += 1;
        // Insert the characters into the next line
        document.insert(document.begin() + cy, newline);
    }
    // Want to move the cursor down and to the beginning
    cx = 0;

    textView->SetCursorX(cx);
    textView->SetCursorY(cy);
}

void NewlineCommand ::UnExecute()
{
    textView->SetCursorX(origCX);
    textView->SetCursorY(origCY);
    cx = origCX;
    cy = origCY;
    document = origDocument;
}

// ******************************************************
// Implement Command History
ECCommandHistory ::ECCommandHistory() {}

ECCommandHistory ::~ECCommandHistory()
{
    while (!history.empty())
    {
        history.pop();
    }
    while (!future.empty())
    {
        future.pop();
    }
}

bool ECCommandHistory ::Undo()
{
    if (history.empty())
        return false;
    // If there is history to undo
    history.top()->UnExecute();
    future.push(history.top());
    history.pop();
    return true;
}

bool ECCommandHistory ::Redo()
{
    if (future.empty())
        return false;
    // If there is something to redo
    future.top()->Execute();
    history.push(future.top());
    future.pop();
    return true;
}

void ECCommandHistory ::ExecuteCmd(ECCommand *pCmd)
{
    while (!future.empty())
        future.pop(); // No redo after new changes are made
    pCmd->Execute();
    history.push(pCmd);
}
