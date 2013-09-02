/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__
/*------------------------------------------------------------------------*/
extern "C"
{
#include "BOOL.h"
};
#include <string>
/*------------------------------------------------------------------------*/
using namespace std ;
/*------------------------------------------------------------------------*/
class CommandLine
{
public:
    /**
    * Constructor CommandLine
    */
    CommandLine(void);

    /**
    * Constructor CommandLine
    * @param line_ : a string
    */
    CommandLine(std::string line_);

    /**
    * Destructor CommandLine
    */
    ~CommandLine();

    /**
    * get command line
    * @return a string
    */
    std::string get(void);

    /**
    * set command line
    * @param line_ : a string
    * @return TRUE or FALSE
    */
    BOOL set(std::string line_);

protected:

private:
    std::string Command;
};
/*------------------------------------------------------------------------*/
#endif /* __COMMANDLINE_H__ */
/*------------------------------------------------------------------------*/
