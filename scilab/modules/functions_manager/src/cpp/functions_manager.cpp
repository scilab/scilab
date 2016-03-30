/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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

#include "funcmanager.hxx"

extern "C"
{
#include "functions_manager.h"
}

bool LoadModules()
{
    FuncManager* pFM = FuncManager::getInstance();
    if (pFM == NULL)
    {
        return false;
    }

    pFM->LoadModules();
    return true;
}

void EndModules()
{
    FuncManager* pFM = FuncManager::getInstance();
    if (pFM)
    {
        pFM->EndModules();
    }
}

void UnloadModules()
{
    FuncManager* pFM = FuncManager::getInstance();
    if (pFM)
    {
        pFM->UnloadModules();
    }
}

void destroyfunctionManagerInstance(void)
{
    FuncManager::destroyInstance();
}
