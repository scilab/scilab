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
#include "CommandLine.hxx"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
CommandLine::CommandLine(void)
{
}
/*------------------------------------------------------------------------*/
CommandLine::CommandLine(std::string line_)
{
    this->Command = line_ ;
}
/*------------------------------------------------------------------------*/
CommandLine::~CommandLine()
{
    Command.erase();
}
/*------------------------------------------------------------------------*/
std::string CommandLine::get(void)
{
    return Command ;
}
/*------------------------------------------------------------------------*/
BOOL CommandLine::set(std::string line_)
{
    if (!line_.empty())
    {
        this->Command = line_ ;
        return TRUE;
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
