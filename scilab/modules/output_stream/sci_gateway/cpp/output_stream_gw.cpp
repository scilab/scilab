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
