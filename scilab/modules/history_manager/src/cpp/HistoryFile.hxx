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
	/**
	* Constructor
	*/
	HistoryFile();

	/**
	* Destructor
	*/
	~HistoryFile();

	/**
	* get filename used by history
	* @return a string (filenam)
	*/
	std::string getFilename(void);

	/**
	* set filename used by history
	* @param a string (filename)
	*/
	void setFilename(std::string filename);

	/**
	* set default filename used by history
	* SCIHOME/history.scilab
	* @return TRUE or FALSE
	*/
	BOOL setDefaultFilename(void);

	/**
	* write history to a file
	* @param a string the filename
	* @return TRUE or FALSE
	*/
	BOOL writeToFile(std::string filename);

	/**
	* write history to a file
	* use default filename
	* @return TRUE or FALSE
	*/
	BOOL writeToFile(void);

	/**
	* load history from a file
	* @param a string the filename
	* @return TRUE or FALSE
	*/
	BOOL loadFromFile(std::string filename);

	/**
	* load history from a file
	* use default filename
	* @return TRUE or FALSE
	*/
	BOOL loadFromFile(void);

	/**
	* get loaded history
	* @return a list of CommandLine
	*/
	list<CommandLine> getHistory(void);

	/**
	* set History to save
	* @param a list of CommandLine
	* @return TRUE or FALSE
	*/
	BOOL setHistory(list<CommandLine> commands);

	/**
	* reset HistoryFile Object
	* @return TRUE or FALSE
	*/
	BOOL reset(void);

protected:
private:
	std::string my_history_filename;
	list<CommandLine> Commands;
};
/*------------------------------------------------------------------------*/
