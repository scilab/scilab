/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "coverage_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME L"coverage"

int CoverageModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"covStart", &sci_covStart, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"covWrite", &sci_covWrite, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"covStop", &sci_covStop, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"covMerge", &sci_covMerge, NULL, MODULE_NAME));
    return 1;
}
