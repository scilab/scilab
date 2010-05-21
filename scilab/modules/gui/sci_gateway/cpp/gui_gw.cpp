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

#include "gui_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME "gui"
extern "C"
{
	#include "gw_gui.h"
}

using namespace types;

bool GuiModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction("about", &sci_about, MODULE_NAME));
    return true;
}
