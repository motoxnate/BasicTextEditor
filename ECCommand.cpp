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
    this->cursorX = cursorX;
    this->cursorY = cursorY;
    this->c = c;
}

InsertCharCommand ::~InsertCharCommand()
{
    delete doc;
}

void InsertCharCommand ::Execute()
{
    // If the line does not yet exist, add a new line
    if(cursorY == document.size()) {
        vector<char> vec;
        document.push_back(vec);
    }
    document[cursorY].insert(document[cursorY].begin() + cursorX, c);
    // StepCursorForward();
}

void InsertCharCommand ::UnExecute()
{
    document[cursorY].erase(document[cursorY].begin() + cursorX);
    textView->SetCursorX(cursorX);
    textView->SetCursorY(cursorY);
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
    if (cx < 0)
    {
        cy -= 1;
        if (cy < 0)
        { // Cursor cannot go back further
            cerr << "Cursor reached first character" << endl;
            return;
        }
        else // Set cursorX to last char of previous row
        {
            cx = doc->GetLenRow(cy) - 1;
            // Check if the cursor was on the last line of the document.
            if(!cy + 1 == document.size()) {
                // If line not empty, copy the remaining text to the row above
                if (!document[cy + 1].empty())
                {
                    document[cy].insert(document[cy].begin() + cx,
                                        document[cy + 1].begin(),
                                        document[cy + 1].end());
                }
                // Then erase the line below it
                document.erase(document.begin() + cy + 1);
            } else ; // Pass completely if the cursor was past the last line.
        }
    } //TODO: Error checking for blank lines
    c = document[cy][cx];
    document[cy].erase(document[cy].begin() + cx);
    textView->SetCursorX(cx);
    textView->SetCursorY(cy);
}

void BackspaceCommand::UnExecute()
{
    textView->SetCursorX(origCX);
    textView->SetCursorY(origCY);
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
