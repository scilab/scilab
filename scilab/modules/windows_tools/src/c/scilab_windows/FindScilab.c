/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <windows.h>
#include <string.h>
#include "FindScilab.h"
#include "version.h"
#include "sci_malloc.h"
#include "WndThread.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#define LineMax 255
#define NumberScilabMax 10
/*--------------------------------------------------------------------------*/
static char BeginningHiddenScilabWindow[LineMax];
static char ListScilabName[NumberScilabMax][LineMax];
static char ListHiddenScilabName[NumberScilabMax][LineMax];
static int NumberScilab = 0;
static BOOL MoreMaxNumberScilabMax = FALSE;
/*--------------------------------------------------------------------------*/
BOOL HaveAnotherWindowScilab(void)
{
    BOOL Retour = FALSE;
    HWND CurrenthWnd = NULL;

    wsprintf(BeginningHiddenScilabWindow, FORMAT_TITLE_HIDDEN_WINDOWS, SCI_VERSION_STRING, 0);
    /* scilab-5.0 hidden window */
    BeginningHiddenScilabWindow[strlen(BeginningHiddenScilabWindow) - 4] = '\0';

    CurrenthWnd = GetWindow(GetDesktopWindow(), GW_CHILD);
    CurrenthWnd = GetWindow(CurrenthWnd, GW_HWNDFIRST);

    while ( CurrenthWnd != NULL )
    {
        char Title[LineMax];

        GetWindowText(CurrenthWnd, Title, (int)strlen(BeginningHiddenScilabWindow) + 1);
        if (strcmp(Title, BeginningHiddenScilabWindow) == 0)
        {
            GetWindowText(CurrenthWnd, Title, LineMax);
            if (NumberScilab < NumberScilabMax)
            {
                wsprintf(ListHiddenScilabName[NumberScilab], "%s", Title);
            }
            else
            {
                MoreMaxNumberScilabMax = TRUE;
            }
            NumberScilab++;
            Retour = TRUE;
        }
        CurrenthWnd = GetWindow(CurrenthWnd, GW_HWNDNEXT);
    }
    return Retour;
}
/*--------------------------------------------------------------------------*/
char * getLastScilabFound(void)
{
    return os_strdup(ListHiddenScilabName[0]);
}
/*--------------------------------------------------------------------------*/
char * getFirstScilabFound(void)
{
    return os_strdup(ListHiddenScilabName[NumberScilab - 1]);
}
/*--------------------------------------------------------------------------*/
