/*  Main Function: Setup the single instances and start the text editor
    Written by: Nate Fanning */

// #include "ECTextEditor.cpp"
#include <iostream>
#include "ECTextViewImp.h"
#include "ECTextEditor.h"
#include "ECTextDocument.h"
#include "ECCommand.h"

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
