/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include "CallScilabBridge.hxx"

extern "C"
{
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "BOOL.h"
#include "MALLOC.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;
/*--------------------------------------------------------------------------*/
int sci_about( char * fname, int *_piKey )
{
    std::cerr << "Calling sci_about" << std::endl;
  CallScilabBridge::scilabAboutBox(getScilabJavaVM());
  
  LhsVar(1) = 0;
  PutLhsVar();
  
  return 0;
 
}
/*--------------------------------------------------------------------------*/
