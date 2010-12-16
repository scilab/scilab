/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "string_gw.hxx"
extern "C"
{
    #include "gw_string.h"
}

#define MODULE_NAME L"string"

bool StringModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"grep", &sci_grep, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"stripblanks", &sci_stripblanks, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"regexp", &sci_regexp, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"part", &sci_part, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"length", &sci_length, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"strindex", &sci_strindex, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"strsubst", &sci_strsubst, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"strsplit", &sci_strsplit, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"ascii", &sci_ascii, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"strcat", &sci_strcat, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"string", &sci_string, MODULE_NAME));
    return true;
}
