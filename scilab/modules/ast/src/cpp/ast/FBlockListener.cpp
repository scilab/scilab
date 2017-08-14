/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2017 - ESI - Antoine ELIAS
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

#include "FBlockListener.hxx"

std::unordered_map<uint64_t, ast::Exp*> FBlockListener::functions;

FBlockListener::FBlockListener()
{
}

void FBlockListener::action(analysis::FunctionBlock & fblock)
{
    std::wcerr << L"Add function \"" << fblock.getName() << L"\" as " << fblock.getFunctionId() << std::endl;
    functions[fblock.getFunctionId()] = fblock.getExp()->clone();
}

ast::Exp* FBlockListener::getExp(uint64_t functionId)
{
    auto it = functions.find(functionId);
    if (it != functions.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}