/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"types"
int TypesModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"list", &sci_list, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"struct", &sci_struct, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"null", &sci_null, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"insert", &sci_insert, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cell", &sci_cell, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"makecell", &sci_makecell, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"usertype", &sci_usertype, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"typeof", &sci_typeof, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"tlist", &sci_tlist, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"mlist", &sci_mlist, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"isfield", &sci_isfield, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"getfield", &sci_getfield, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"fieldnames", &sci_fieldnames, MODULE_NAME));

    return 1;
}
