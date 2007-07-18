/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#ifndef __HISTORYMANAGER_CPP_H__
#define __HISTORYMANAGER_CPP_H__
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
/*------------------------------------------------------------------------*/
extern "C"
{
	#include <time.h>
	#include "machine.h" /* BOOL */
};
/*------------------------------------------------------------------------*/
#include <list>
#include "CommandLine.hxx"
#include "HistoryFile.hxx"
#include "HistorySearch.hxx"
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

	void setSaveConsecutiveDuplicateLines(BOOL doit);
	BOOL getSaveConsecutiveDuplicateLines(void);

	void setAfterHowManyLinesHistoryIsSaved(int num);
	int getAfterHowManyLinesHistoryIsSaved(void);


	BOOL setToken(char *token);
	char *getToken(void);
	BOOL resetToken(void);
	char *getPreviousLine(void);
	char *getNextLine(void);

protected:

private:
	HistoryFile my_file;
	HistorySearch my_search;

	list<CommandLine> Commands;
	list<CommandLine>::iterator it_current_position;

	BOOL saveconsecutiveduplicatelines;
	int afterhowmanylineshistoryissaved;
	int numberoflinesbeforehistoryissaved;

};
/*------------------------------------------------------------------------*/
#endif /* __cplusplus */
/*------------------------------------------------------------------------*/
#endif /* __HISTORYMANAGER_CPP_H__ */
