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
		BOOL setToken(char *token);
		char *getToken(void);
		BOOL search(void);
		char **getSearchResult(void);
		int *getSearchResultIndice(void);
		int getSize(void);
		BOOL reset(void);

		char *getPreviousLine(void);
		char *getNextLine(void);



	protected:

	private:
		list<CommandLine> Commands;
		char *my_token;
		char **my_lines;
		int *my_linenumbers;
		int my_sizearray;
		int current_position;

};
/*------------------------------------------------------------------------*/
