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
#include <vector>
#include <string>
extern "C"
{
#include "dynlib_history_manager.h"
#include "BOOL.h"
}

//disable warnings about exports STL items
#pragma warning (disable : 4251)

/*------------------------------------------------------------------------*/
class HISTORY_MANAGER_IMPEXP HistorySearch
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
    * @param a list of std::string
    * @return TRUE or FALSE
    */
    BOOL setHistory(std::list<std::string> _lstCommands);

    /**
    * set new token to search in history
    * @param token (a string)
    * @return TRUE or FALSE
    */
    BOOL setToken(std::string _stToken);

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
    std::list<std::string> m_Commands;
    std::string m_stToken;
    std::vector<std::string> m_vstLines;
    int m_iPosition;

    BOOL search(void);
};
/*------------------------------------------------------------------------*/
