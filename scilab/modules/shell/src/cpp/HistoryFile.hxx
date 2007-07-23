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

	std::string getFilename(void);
	void setFilename(std::string filename);
	BOOL setDefaultFilename(void);

	BOOL writeToFile(std::string filename);
	BOOL writeToFile(void);
	BOOL loadFromFile(std::string filename);
	BOOL loadFromFile(void);

	list<CommandLine> getHistory(void);
	BOOL setHistory(list<CommandLine> commands);

	BOOL reset(void);

protected:
private:
	std::string my_history_filename;
	list<CommandLine> Commands;
};
/*------------------------------------------------------------------------*/
