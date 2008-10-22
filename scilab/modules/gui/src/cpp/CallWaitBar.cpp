/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CallWaitBar.hxx"

using namespace org_scilab_modules_gui_bridge;

int createWaitBar()
{
  return CallScilabBridge::newWaitBar(getScilabJavaVM());
}

void setWaitBarValue(int ID, int value)
{
  CallScilabBridge::setWaitBarValue(getScilabJavaVM(), ID, value);
}

void setWaitBarMessage(int ID, char **message, int nbLine)
{
  CallScilabBridge::setWaitBarMessage(getScilabJavaVM(), ID, message, nbLine);
}

void setWaitBarIndeterminateMode(int ID, BOOL status)
{
  CallScilabBridge::setWaitBarIndeterminateMode(getScilabJavaVM(), ID, BOOLtobool(status));
}

