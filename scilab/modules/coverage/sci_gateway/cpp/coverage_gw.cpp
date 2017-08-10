/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
