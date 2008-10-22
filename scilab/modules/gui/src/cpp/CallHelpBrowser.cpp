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

#include "CallHelpBrowser.hxx"
#include "BOOL.h"

using namespace org_scilab_modules_gui_bridge;

void launchHelpBrowser(char **helps, int helpsSize, char* language)
{
  CallScilabBridge::launchHelpBrowser(getScilabJavaVM(), helps, helpsSize, language);
}

void searchKeyword(char **helps, int helpsSize, char *keyword, char* language, BOOL fullText)
{
  CallScilabBridge::searchKeyword(getScilabJavaVM(), helps, helpsSize, keyword, language, BOOLtobool(fullText));
}
