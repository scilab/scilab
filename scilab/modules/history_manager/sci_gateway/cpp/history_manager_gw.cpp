/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "context.hxx"
#include "function.hxx"
#include "history_manager_gw.hxx"

#define MODULE_NAME "history_manager"

int HistoryManagerModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("loadhistory", &sci_loadhistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("addhistory",   &sci_addhistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("displayhistory",   &sci_displayhistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("gethistoryfile",   &sci_gethistoryfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("gethistory", &sci_gethistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("historymanager",   &sci_historymanager, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("historysize",   &sci_historysize, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("removelinehistory",   &sci_removelinehistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("resethistory", &sci_resethistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("saveafterncommands",   &sci_saveafterncommands, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("saveconsecutivecommands",   &sci_saveconsecutivecommands, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("savehistory", &sci_savehistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sethistoryfile",   &sci_sethistoryfile, MODULE_NAME));

    return true;
}
