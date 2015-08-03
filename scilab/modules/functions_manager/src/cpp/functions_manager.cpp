/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "funcmanager.hxx"

extern "C"
{
#include "functions_manager.h"
}

void LoadModules()
{
    FuncManager* pFM = FuncManager::getInstance();
    if (pFM)
    {
        pFM->LoadModules();
    }
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