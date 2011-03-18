/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "helptools_gw.hxx"

extern "C"
{
#include "loadOnUseClassPath.h"
}

#define MODULE_NAME L"helptools"

extern "C"
{
    #include "gw_helptools.h"
}

bool HelptoolsModule::loadedDep = false;

void HelptoolsModule::LoadDeps(void)
{
    if (loadedDep == false)
    {
        loadOnUseClassPath("documentationGeneration");
        loadedDep = true;
    }
}

bool HelptoolsModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"buildDoc", &sci_buildDoc, &HelptoolsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"buildDocv2", &sci_buildDocv2, &HelptoolsModule::LoadDeps, MODULE_NAME));
    return true;
}
