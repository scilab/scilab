/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include <list>
#include "CommandLine.hxx"
using namespace std;
/*------------------------------------------------------------------------*/
class HistorySearch
{
	public:
		HistorySearch();
		~HistorySearch();
		BOOL setHistory(list<CommandLine> commands);
		BOOL setToken(std::string token);
		std::string getToken(void);
		
		int getSize(void);
		BOOL reset(void);

		std::string getPreviousLine(void);
		std::string getNextLine(void);

	protected:

	private:
		list<CommandLine> Commands;
		std::string my_token;
		char **my_lines;
		int *my_linenumbers;
		int my_sizearray;
		int current_position;
		BOOL moveOnNext;

		BOOL search(void);

		BOOL freeMyToken(void);
		BOOL freeMylines(void);
		BOOL freeMylinenumbers(void);

};
/*------------------------------------------------------------------------*/
