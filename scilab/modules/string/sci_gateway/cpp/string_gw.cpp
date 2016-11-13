/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif


#include "string_gw.hxx"
#include "context.hxx"
#include "function.hxx"

#define MODULE_NAME "string"

int StringModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("grep", &sci_grep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("stripblanks", &sci_stripblanks, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("regexp", &sci_regexp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("part", &sci_part, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("length", &sci_length, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strindex", &sci_strindex, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strsubst", &sci_strsubst, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strsplit", &sci_strsplit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ascii", &sci_ascii, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strcat", &sci_strcat, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("string", &sci_string, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("convstr", &sci_convstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strncpy", &sci_strncpy, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strrchr", &sci_strrchr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strchr", &sci_strchr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strstr", &sci_strstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strrev", &sci_strrev, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strtod", &sci_strtod, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("tokens", &sci_tokens, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strcmp", &sci_strcmp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isletter", &sci_isletter, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strcspn", &sci_strcspn, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strtok", &sci_strtok, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("strspn", &sci_strspn, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isalphanum", &sci_isalphanum, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isdigit", &sci_isdigit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isascii", &sci_isascii, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isnum", &sci_isnum, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("csvIsnum", &sci_isnum, MODULE_NAME));
    return 1;
}
