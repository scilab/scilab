/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "context.hxx"
#include "function.hxx"
#include "history_manager_gw.hxx"

#define MODULE_NAME L"history_manager"

int HistoryManagerModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"loadhistory", &sci_loadhistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"addhistory",   &sci_addhistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"displayhistory",   &sci_displayhistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"gethistoryfile",   &sci_gethistoryfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"gethistory", &sci_gethistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"historymanager",   &sci_historymanager, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"historysize",   &sci_historysize, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"removelinehistory",   &sci_removelinehistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"resethistory", &sci_resethistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"saveafterncommands",   &sci_saveafterncommands, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"saveconsecutivecommands",   &sci_saveconsecutivecommands, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"savehistory", &sci_savehistory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sethistoryfile",   &sci_sethistoryfile, MODULE_NAME));

    return true;
}
