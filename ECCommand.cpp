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
