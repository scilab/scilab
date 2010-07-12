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

#define MODULE_NAME L"types"
bool TypesModule::Load()
{
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"list", &sci_list, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"struct", &sci_struct, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"null", &sci_null, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"insert", &sci_insert, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"cell", &sci_cell, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"makecell", &sci_makecell, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"usertype", &sci_usertype, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"typeof", &sci_typeof, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction(L"tlist", &sci_tlist, MODULE_NAME));

  return true;
}
