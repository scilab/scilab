/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include <list>
#include "CommandLine.hxx"
using namespace std;
/*------------------------------------------------------------------------*/
class HistoryFile
{
public:
	HistoryFile();
	~HistoryFile();

	char *getFilename(void);
	void setFilename(char *filename);
	BOOL setDefaultFilename(void);

	BOOL writeToFile(char *filename);
	BOOL writeToFile(void);
	BOOL loadFromFile(char *filename);
	BOOL loadFromFile(void);

	list<CommandLine> getHistory(void);
	BOOL setHistory(list<CommandLine> commands);

	BOOL reset(void);

protected:
private:
	char *my_history_filename;
	list<CommandLine> Commands;
};
/*------------------------------------------------------------------------*/
