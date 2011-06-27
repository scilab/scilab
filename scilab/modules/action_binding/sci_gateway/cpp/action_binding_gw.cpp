/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "action_binding_gw.hxx"
#include "context.hxx"
#include "types.hxx"

extern "C"
{
#include "dynlib_action_binding.h"
}

#define MODULE_NAME L"action_binding"

int ActionBindingModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"notify", &sci_notify, MODULE_NAME));
    return 1;
}
