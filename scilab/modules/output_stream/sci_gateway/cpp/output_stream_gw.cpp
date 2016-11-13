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

#include "output_stream_gw.hxx"
#include "function.hxx"
#include "context.hxx"

extern "C"
{
#include "gw_output_stream.h"
}

#define MODULE_NAME "output_stream"

int OutputStreamModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("disp", &sci_disp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("msprintf", &sci_msprintf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sprintf", &sci_msprintf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("printf", &sci_mprintf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mprintf", &sci_mprintf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("diary", &sci_diary, MODULE_NAME));
    return 1;
}
