#include "ECTextDocument.h"

using namespace std;

/*  ******************************************************
    Define Commands in this file to deal with inclusions */

// –––––––––––––––––––
// InsertCharCommand
InsertCharCommand ::InsertCharCommand(ECTextDocument *doc, int cursorX, int cursorY, char c)
    : ECCommand(doc, doc->document)
{
    this->cx = cursorX;
    this->cy = cursorY;
    this->c = c;
    textView = doc->GetTextView();
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
    textView = doc->GetTextView();
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
    textView = doc->GetTextView();
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

/* ******************************************************
    Implement ECTextDocumentCtrl */
ECTextDocumentCtrl::ECTextDocumentCtrl(ECTextDocument &docIn) : doc(docIn) {}
ECTextDocumentCtrl::~ECTextDocumentCtrl() {}

// void ECTextDocumentCtrl:: AddEditor(ECTextEditor *editor) {
//     textEditor = editor;
// }

void ECTextDocumentCtrl::InsertChar(int cx, int cy, char c)
{
    InsertCharCommand *cmd = new InsertCharCommand(&doc, cx, cy, c);
    histCmds.ExecuteCmd(cmd);
}

void ECTextDocumentCtrl::Backspace(int cx, int cy)
{
    BackspaceCommand *cmd = new BackspaceCommand(&doc, cx, cy);
    histCmds.ExecuteCmd(cmd);
}

void ECTextDocumentCtrl::Newline(int cx, int cy)
{
    NewlineCommand *cmd = new NewlineCommand(&doc, cx, cy);
    histCmds.ExecuteCmd(cmd);
}

bool ECTextDocumentCtrl::Undo()
{
    return histCmds.Undo();
}

bool ECTextDocumentCtrl::Redo()
{
    return histCmds.Redo();
}

/* ******************************************************
    Implement ECTextDocument */
ECTextDocument::ECTextDocument(ECTextViewImp *textView, ECFileIO *fileIO) : docCtrl(*this)
{
    this->textView = textView;
    this->fileIO = fileIO;
    // Read input file, if any
    vector<string> readfile = fileIO->read();
    for(int i=0; i<readfile.size(); i++) {
        vector<char> line(readfile[i].begin(), readfile[i].end());
        document.push_back(line);
    }
}

ECTextDocument:: ~ECTextDocument() {
    fileIO->write(GetAllRows());
}

ECTextDocumentCtrl ECTextDocument::GetCtrl()
{
    return docCtrl;
}

ECTextViewImp *ECTextDocument::GetTextView()
{
    return textView;
}

// Return the number of rows in the document
int ECTextDocument ::GetNumRows() const
{
    return document.size();
}

// Return the length of a row of the document. If the row is empty
// then return 0
int ECTextDocument ::GetLenRow(int r) const
{
    if (r >= GetNumRows())
        return 0;
    return document[r].size();
}

// Turn document into a vector of strings and return
vector<string> ECTextDocument::GetAllRows()
{
    vector<string> output;
    for (int i = 0; i < document.size(); i++)
    {
        string append(document[i].begin(), document[i].end());
        output.push_back(append);
    }
    return output;
}

void ECTextDocument ::AddRow(string row)
{
    vector<char> newRow;
    for (char c : row)
    {
        newRow.push_back(c);
    }
    document.push_back(newRow);
    return;
}