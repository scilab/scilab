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
	BOOL setDefaultFilename(void);

	BOOL writeToFile(char *filename);
	BOOL loadFromFile(char *filename);
	void reset(void);
	char **getAllLines(int *numberoflines);
	char *getLastLine(void);
	char *getFirstLine(void);
	int getNumberOfLines(void);
	char *getNthLine(int N);
	
	char *getLineCurrentPositionIterator(void);
	void moveToPreviousPositionIterator(void);
	void moveToNextPositionIterator(void);
	char **searchToken(char *token,int *nb);
	int **searchTokenAndReturnsLinesNumber(char *token,int *nb);

	void setSaveConsecutiveDuplicateLines(BOOL doit);
	BOOL getSaveConsecutiveDuplicateLines(void);

	void setAfterHowManyLinesHistoryIsSaved(int num);
	int getAfterHowManyLinesHistoryIsSaved(void);

	

protected:

private:
	list<CommandLine> Commands;
	char *historyfilename;
	list<CommandLine>::iterator it_current_position;
	BOOL saveconsecutiveduplicatelines;
	int afterhowmanylineshistoryissaved;
	int numberoflinesbeforehistoryissaved;

	char *getCommentDateSession(BOOL BeginSession);
	char *ASCIItime(const struct tm *timeptr);

	BOOL saveHistory(void);

};
/*------------------------------------------------------------------------*/
#endif /* __HISTORYMANAGER_CPP_H__ */