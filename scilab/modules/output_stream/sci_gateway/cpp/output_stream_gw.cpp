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

#define MODULE_NAME L"output_stream"

bool OutputStreamModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"disp", &sci_disp, MODULE_NAME));
  return true;
}
