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

#include "time_gw.hxx"

#define MODULE_NAME L"time"
extern "C"
{
   #include "gw_time.h"
}

bool TimeModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"getdate", &sci_getdate, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"Calendar", &sci_calendar, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"timer", &sci_timer, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"sleep", &sci_sleep, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"xpause", &sci_xpause, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"realtimeinit", &sci_realtimeinit, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"realtime", &sci_realtime, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"tic", &sci_tic, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"toc", &sci_toc, MODULE_NAME));
    return true;
}
