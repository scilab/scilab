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

#include "CallFigure.hxx"

using namespace org_scilab_modules_gui_bridge;

void setFigureEventHandler(int ID, char * command)
{
   CallScilabBridge::setEventHandler(getScilabJavaVM(), ID, command);
}

void setFigureEventHandlerEnabled(int ID, BOOL status)
{
   CallScilabBridge::setEventHandlerEnabled(getScilabJavaVM(), ID, BOOLtobool(status));
}

int newFigure(int ID)
{
	return CallScilabBridge::newWindow(getScilabJavaVM(), ID);
}
