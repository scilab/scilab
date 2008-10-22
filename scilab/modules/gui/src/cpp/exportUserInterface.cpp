/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to export files 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "exportUserInterface.hxx"
#include "CallScilabBridge.hxx"


extern "C"
{
#include "getScilabJavaVM.h"
}

/*---------------------------------------------------------------------------------*/
int exportUserInterface(int figureId)
{
  return org_scilab_modules_gui_bridge::CallScilabBridge::newExportFileChooser(getScilabJavaVM(), figureId);
}
/*---------------------------------------------------------------------------------*/
