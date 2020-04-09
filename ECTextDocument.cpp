#include <algorithm>
#include "ECTextDocument.h"

using namespace std;

/* ******************************************************
    Implement ECTextDocumentCtrl */
    ECTextDocumentCtrl::ECTextDocumentCtrl(ECTextDocument &docIn) : doc(docIn) {}
    ECTextDocumentCtrl::~ECTextDocumentCtrl() {}

    // void ECTextDocumentCtrl:: AddEditor(ECTextEditor *editor) {
    //     textEditor = editor;
    // }
    
    void ECTextDocumentCtrl:: InsertChar(int cx, int cy, char c) {
        InsertCharCommand *cmd = new InsertCharCommand(&doc, cx, cy, c);
        histCmds.ExecuteCmd(cmd);
    }

    void ECTextDocumentCtrl:: Backspace(int cx, int cy) {
        BackspaceCommand *cmd = new BackspaceCommand(&doc, cx, cy);
        histCmds.ExecuteCmd(cmd);
    }

    bool ECTextDocumentCtrl:: Undo() {
        return histCmds.Undo();
    }

    bool ECTextDocumentCtrl:: Redo() {
        return histCmds.Redo();
    }

/* ******************************************************
    Implement ECTextDocument */
    ECTextDocument::ECTextDocument(ECTextViewImp *textView) : docCtrl(*this) {
        this->textView = textView;
    }

    ECTextDocumentCtrl ECTextDocument::GetCtrl() {
        return docCtrl;
    }

    ECTextViewImp* ECTextDocument:: GetTextView() {
        return textView;
    }

    // Return the number of rows in the document
    int ECTextDocument :: GetNumRows() const {
        return document.size();
    }

    // Return the length of a row of the document. If the row is empty
    // then return 0
    int ECTextDocument :: GetLenRow(int r) const {
        if(r >= GetNumRows()) return 0;
        return document[r].size();
    }

    // Turn document into a vector of strings and return
    vector<string> ECTextDocument::GetAllRows() {
        vector<string> output;
        for(int i=0; i<document.size(); i++) {
            string append(document[i].begin(), document[i].end());
            output.push_back(append);
        }
        return output;
    }

    void ECTextDocument :: AddRow(string row) {
        vector<char> newRow;
        for(char c : row) {
            newRow.push_back(c);
        }
        document.push_back(newRow);
        return;
    }