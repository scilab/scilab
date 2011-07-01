/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2011 - DIGITEO - Allan CORNET
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
/*------------------------------------------------------------------------*/
typedef enum 
{
    NO_ERROR_HISTORY_LOADED,
    ERROR_HISTORY_NOT_LOADED,
    HISTORY_TRUNCATED
} errorLoadHistoryCode;
/*------------------------------------------------------------------------*/
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
    errorLoadHistoryCode loadFromFile(std::string filename);

    /**
    * load history from a file
    * use default filename
    * @return TRUE or FALSE
    */
    errorLoadHistoryCode loadFromFile(void);

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

    /**
    *
    */
    int getDefaultMaxNbLines(void);

    /**
    *
    */
    BOOL setDefaultMaxNbLines(int nbLinesMax);

protected:
private:
    std::string my_history_filename;
    list<CommandLine> Commands;
    int MaxLinesToRead;
};
/*------------------------------------------------------------------------*/
