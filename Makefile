default:	main

main: main.o ECTextEditor.o ECTextDocument.o ECCommand.o ECTextViewImp.o
	c++ -g -Wall -o main main.o ECTextEditor.o ECTextDocument.o ECCommand.o ECTextViewImp.o

main.o: main.cpp ECTextEditor.h ECTextDocument.h ECCommand.h ECTextViewImp.h

ECTextEditor.o: ECTextEditor.h ECTextDocument.h ECCommand.h

ECTextDocument.o: ECTextDocument.h ECCommand.h

ECCommand.o: ECCommand.h ECTextViewImp.h ECTextDocument.h

ECTextViewImp.o: ECTextViewImp.h ECObserver.h

clean:
	rm *.o