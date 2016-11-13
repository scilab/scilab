/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "time_gw.hxx"

#define MODULE_NAME "time"
extern "C"
{
#include "gw_time.h"
}

int TimeModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getdate", &sci_getdate, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("Calendar", &sci_calendar, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("timer", &sci_timer, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sleep", &sci_sleep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xpause", &sci_xpause, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("realtimeinit", &sci_realtimeinit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("realtime", &sci_realtime, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("tic", &sci_tic, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("toc", &sci_toc, MODULE_NAME));
    return 1;
}
