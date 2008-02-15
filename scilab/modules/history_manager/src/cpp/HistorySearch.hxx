/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <list>
#include "CommandLine.hxx"
using namespace std;
/*------------------------------------------------------------------------*/
class HistorySearch
{
	public:
		/**
		* Constructor
		*/
		HistorySearch();

		/**
		* Destructor
		*/
		~HistorySearch();

		/**
		* set History to search
		* @param a list of CommandLine
		* @return TRUE or FALSE
		*/
		BOOL setHistory(list<CommandLine> commands);

		/**
		* set new token to search in history
		* @param token (a string)
		* @return TRUE or FALSE
		*/
		BOOL setToken(std::string token);

		/**
		* get token searched in history
		* @return token (a string)
		*/
		std::string getToken(void);

		/**
		* returns size of the
		*/
		int getSize(void);

		/**
		* reset HistorySearch Object
		* @return TRUE or FALSE
		*/
		BOOL reset(void);

		/**
		* Get the previous line in search
		* @return a line
		*/
		std::string getPreviousLine(void);

		/**
		* Get the next line in search
		* @return a line
		*/
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
