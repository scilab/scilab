/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*------------------------------------------------------------------------*/
#include <list>

extern "C"
{
#include "dynlib_history_manager.h"
#include "BOOL.h"
}

//disable warnings about exports STL items
#pragma warning (disable : 4251)

/*------------------------------------------------------------------------*/
typedef enum
{
    NO_ERROR_HISTORY_LOADED,
    ERROR_HISTORY_NOT_LOADED,
    HISTORY_TRUNCATED
}
errorLoadHistoryCode;
/*------------------------------------------------------------------------*/
class HISTORY_MANAGER_IMPEXP HistoryFile
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
    BOOL writeToFile(std::string _stFilename);

    /**
    * get filename used by history
    * @return a string (filenam)
    */
    std::string getFilename(void);

    /**
    * set filename used by history
    * @param a string (filename)
    */
    void setFilename(std::string _stFilename);

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
    errorLoadHistoryCode loadFromFile(std::string _stFilename);


    /**
    * load history from a file
    * use default filename
    * @return TRUE or FALSE
    */
    errorLoadHistoryCode loadFromFile(void);

    /**
    * reset HistoryFile Object
    * @return TRUE or FALSE
    */
    BOOL reset(void);

    /**
    *
    */
    int getDefaultMaxNbLines(void);

    /**
    * get loaded history
    * @return a list of CommandLine
    */
    std::list<std::string> getHistory(void);

    /**
    * set History to save
    * @param a list of CommandLine
    * @return TRUE or FALSE
    */
    BOOL setHistory(std::list<std::string> _lstCommands);

    /**
    *
    */
    BOOL setDefaultMaxNbLines(int _iMaxLines);

protected:
private:

    int m_iMaxLines;
    std::string m_stFilename;
    std::list<std::string> m_Commands;
};
/*------------------------------------------------------------------------*/
