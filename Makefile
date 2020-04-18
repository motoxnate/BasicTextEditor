default:	main

main: main.o ECTextEditor.o ECTextDocument.o ECCommand.o ECTextViewImp.o ECFileIO.o
	c++ -g -Wall -o main main.o ECTextEditor.o ECTextDocument.o ECCommand.o ECTextViewImp.o ECFileIO.o

main.o: main.cpp ECTextEditor.h ECTextDocument.h ECCommand.h ECTextViewImp.h ECFileIO.h

ECTextEditor.o: ECTextEditor.h ECTextDocument.h ECCommand.h

ECTextDocument.o: ECTextDocument.h ECCommand.h ECFileIO.h

ECCommand.o: ECCommand.h ECTextViewImp.h ECTextDocument.h

ECTextViewImp.o: ECTextViewImp.h ECObserver.h

ECFileIO.o: ECFileIO.h

clean:
	rm main && rm *.o