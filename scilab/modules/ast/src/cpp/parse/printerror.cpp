/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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

#include <fstream>
#include <string>
#include "parser_private.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "charEncoding.h"
}

void ParserSingleInstance::PrintError(const std::wstring& msg)
{
    std::wostringstream ostr;
    char *codeLine = (char *) malloc(4096 * sizeof(char));

    //If the error is a the very beginning of a line
    if (yylloc.first_line == yylloc.last_line
            && yylloc.first_column == 1
            && yylloc.last_column == 1)
    {
        --yylloc.first_line;
    }

    // fill call stack
    ConfigVariable::fillWhereError(yylloc.first_line);

    /** Print where in the script the error is located */
    wchar_t* str = to_wide_string(ParserSingleInstance::getCodeLine(yylloc.first_line, &codeLine));
    ostr << str;
    // add EOL only if the code line doesn't already contains it.
    if (wcscmp(str + wcslen(str) - 1, L"\n") != 0)
    {
        ostr << std::endl;
    }
    free(codeLine);
    FREE(str);

    /** Underline what causes the trouble */
    int i = 0;
    for (i = 1 ; i < yylloc.first_column ; ++i)
    {
        ostr << L" ";
    }
    ostr << L"^";
    for (i = i + 1 ; i < yylloc.last_column ; ++i)
    {
        ostr << L"~";
    }
    if ( yylloc.first_column != yylloc.last_column )
    {
        ostr << L"^" ;
    }
    ostr << std::endl;

    /** Display Parser message  */
    std::wstring wstrError(_W("Error: "));
    ostr << wstrError << msg << std::endl;

    ParserSingleInstance::appendErrorMessage(ostr.str());
}
