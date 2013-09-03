/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "CallHelpBrowser.hxx"
#include "BOOL.h"

using namespace org_scilab_modules_gui_bridge;

int launchHelpBrowser(char **helps, int helpsSize, char* language)
{
    try
    {
        CallScilabBridge::launchHelpBrowser(getScilabJavaVM(), helps, helpsSize, language);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), "help", e.whatStr().c_str());
        return 0;
    }

    return 1;
}

int searchKeyword(char **helps, int helpsSize, char *keyword, char* language, BOOL fullText)
{
    try
    {
        CallScilabBridge::searchKeyword(getScilabJavaVM(), helps, helpsSize, keyword, language, BOOLtobool(fullText));
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), "help", e.whatStr().c_str());
        return 0;
    }

    return 1;
}
