/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "CallHelpBrowser.hxx"
extern "C"
{
#include "BOOL.h"
#include "Scierror.h"
#include "localization.h"
}

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
