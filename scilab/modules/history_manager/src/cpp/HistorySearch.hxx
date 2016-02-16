/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

#ifdef _MSC_VER
//disable warnings about exports STL items
#pragma warning (disable : 4251)
#endif

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
