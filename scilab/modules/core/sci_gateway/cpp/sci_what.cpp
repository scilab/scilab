/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
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

#include "core_gw.hxx"
#include "string.hxx"
#include "scilabWrite.hxx"
#include "configvariable.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "commandwords.h"
#include "getfunctionsname.h"
#include "sciprint.h"
#include "freeArrayOfString.h"
}

void printVarList(const char* title, const char* const* vars, int size)
{
    int w = ConfigVariable::getConsoleWidth();
    int times = std::max(w / 24, 1);
    sciprint("\n");
    sciprint(_("%s:\n"), title);
    sciprint("\n");
    for (int i = 0; i < size; i++)
    {
        sciprint("%+24s ", vars[i]);
        if ((i + 1) % times == 0)
        {
            sciprint("\n");
        }
    }
    sciprint("\n");
}

const char fname[] = "what";
types::Function::ReturnValue sci_what(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char **pOutStringFctTmp = NULL;
    int sizeFct = 0;
    char **pOutStringCmdTmp = NULL;
    int sizeCmd = 0;

    // Check input arguments
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 0);
        return types::Function::Error;
    }

    // Check output arguments
    if (_iRetCount != 1 && _iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), fname, 1, 2);
        return types::Function::Error;
    }

    // Retrieve list of functions and list of keywords
    pOutStringFctTmp = getFunctionsName(&sizeFct);
    pOutStringCmdTmp = getcommandkeywords(&sizeCmd);

    if (_iRetCount == 1)
    {
        // Print functions
        printVarList("Internal Functions", pOutStringFctTmp, sizeFct);
        // Print keywords
        printVarList("Commands", pOutStringCmdTmp, sizeCmd);
    }
    else
    {
        // Scilab functions in first output argument
        types::String* pOutStringFct = new types::String(sizeFct, 1);
        pOutStringFct->set(pOutStringFctTmp);
        out.push_back(pOutStringFct);

        // Scilab commands in second output argument
        types::String* pOutStringCmd = new types::String(sizeCmd, 1);
        pOutStringCmd->set(pOutStringCmdTmp);
        out.push_back(pOutStringCmd);
    }

    freeArrayOfString(pOutStringFctTmp, sizeFct);
    freeArrayOfString(pOutStringCmdTmp, sizeCmd);
    return types::Function::OK;
}
