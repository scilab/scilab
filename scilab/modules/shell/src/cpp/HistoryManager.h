/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#ifndef __HISTORYMANAGER_CPP_H__
#define __HISTORYMANAGER_CPP_H__
/*------------------------------------------------------------------------*/
extern "C"
{
	#include <time.h>
	#include "machine.h" /* BOOL */
};
/*------------------------------------------------------------------------*/
#include <list>
#include "CommandLine.h"
using namespace std;
/*------------------------------------------------------------------------*/
class HistoryManager
{
public:
	HistoryManager();
	~HistoryManager();
	BOOL appendLine(char *line);
	BOOL appendLines(char **lines,int nbrlines);
	void displayHistory(void);
	char *getFilename(void);
	void setFilename(char *filename);
	BOOL writeToFile(char *filename);
	BOOL loadFromFile(char *filename);
	void reset(void);
	char **getAllLines(int *numberoflines);
	char *getLastLine(void);
	char *getFirstLine(void);
	
	char *getLineCurrentPositionIterator(void);
	void moveToPreviousPositionIterator(void);
	void moveToNextPositionIterator(void);

protected:

private:
	list<CommandLine> Commands;
	char *historyfilename;
	list<CommandLine>::iterator it_current_position;

	char *getCommentDateSession(BOOL BeginSession);
	char *ASCIItime(const struct tm *timeptr);
};
/*------------------------------------------------------------------------*/
#endif /* __HISTORYMANAGER_CPP_H__ */