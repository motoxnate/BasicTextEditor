/*  Main Function: Setup the single instances and start the text editor
    Written by: Nate Fanning */

// #include "ECTextEditor.cpp"
#include <iostream>
#include "ECTextViewImp.h"
#include "ECTextEditor.h"
#include "ECTextDocument.h"
#include "ECCommand.h"
#include "ECFileIO.h"

int main(int argc, char *argv[])
{
    string path;
    // if(argc == 1) path = "None";
    if(argc == 2) path = argv[1];
    else {
        cerr << "Useage: ./main <filename.txt>" << endl;
        exit(EXIT_FAILURE);
    }

    // Initialize items
    ECTextViewImp textView;     // Singleton
    ECFileIO *fileIO = new ECFileIO(path);      // Singleton
    ECTextDocument textDocument(&textView, fileIO);
    ECTextDocumentCtrl textDocumentCtrl = textDocument.GetCtrl();

    ECTextEditor textEditor(textView, textDocument, textDocumentCtrl);
    return 0;
}
