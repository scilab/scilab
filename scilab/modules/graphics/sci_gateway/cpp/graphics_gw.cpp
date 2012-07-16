/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "graphics_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME L"graphics"
extern "C"
{
	#include "gw_graphics.h"
}

using namespace types;

int GraphicsModule::Load()
{
    return 1;
}
