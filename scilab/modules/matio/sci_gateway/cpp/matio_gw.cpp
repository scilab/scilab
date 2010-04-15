/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "matio_gw.hxx"

#define MODULE_NAME "matio"
extern "C"
{
#include "gw_matio.h"
}

using namespace types;

bool MatioModule::Load()
{
  symbol::Context::getInstance()->AddFunction(Function::createFunction("matfile_open", &sci_matfile_open, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction("matfile_close", &sci_matfile_close, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction("matfile_listvar", &sci_matfile_listvar, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction("matfile_varreadnext", &sci_matfile_varreadnext, MODULE_NAME));
  symbol::Context::getInstance()->AddFunction(Function::createFunction("matfile_varwrite", &sci_matfile_varwrite, MODULE_NAME));
  
  return true;
}

