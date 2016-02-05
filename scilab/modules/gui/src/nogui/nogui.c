/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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

#ifndef _MSC_VER
#include <errno.h>
#include <stdio.h> /* fileno */
#include <sys/select.h> /* fd_set */
#include "core_math.h" /* Max */
#include "sciprint.h"
#endif
#include "nogui.h"
#include "storeCommand.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
BOOL InitializeGUI(void)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/
int gw_gui()
{
    Scierror(999, _("Scilab '%s' module not installed.\n"), "GUI");
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL TerminateGUI(void)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
void setVisibleMainWindow(BOOL newVisibleState) {}
/*--------------------------------------------------------------------------*/
BOOL isVisibleMainWindow(void)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/
void deiconifyMainWindow(void) {}
/*--------------------------------------------------------------------------*/
void iconifyMainWindow(void) {}
/*--------------------------------------------------------------------------*/
void maximizeMainWindow(void) {}
/*--------------------------------------------------------------------------*/
void normalMainWindow(void) {}
/*--------------------------------------------------------------------------*/
#else
int XClearScreenConsole(char *fname)
{
    sciprint(_("%s: Only available under 'STANDARD' mode.\n"), fname);
    return 0;
}
/*--------------------------------------------------------------------------*/
void str_to_xterm ( register char * string, int nbytes)
{

}
/*--------------------------------------------------------------------------*/
void Click_menu(int n)
{

}
/*--------------------------------------------------------------------------*/
int GetScreenProperty(char *prop, char *value)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
#define LF                    0x000a
/* @TODO : this code is for a part duplicated from gui/src/c/xsci/inter.c
with some stuff removed (TCL/TK and a few other things)
*/
int Xorgetchar(int interrupt)
{
    int i;
    int inter_max_plus1 = 0;
    static fd_set Select_mask_ref;
    static fd_set select_mask;
    static fd_set Write_mask_ref;
    static fd_set write_mask;
    static struct timeval select_timeout;
    static int fd_in = 0, fd_out = 0, fd_err = 0 ;
    static int state = 0;
    fd_in = fileno(stdin) ;
    fd_out = fileno(stdout);
    fd_err = fileno(stderr);

    FD_ZERO(&Select_mask_ref);
    FD_SET(fd_in , &Select_mask_ref);
    FD_ZERO(&Write_mask_ref);

    inter_max_plus1 = fd_in;

    inter_max_plus1 = Max(fd_out, inter_max_plus1);
    inter_max_plus1 = Max(fd_err, inter_max_plus1);
    inter_max_plus1++;

    for ( ; ; )
    {
        fflush(stdout);
        fflush(stderr);

        /* Initialize masks  */
        select_mask = Select_mask_ref;
        write_mask  = Write_mask_ref;

        select_timeout.tv_sec = 0;
        select_timeout.tv_usec = 10;
        i = select(inter_max_plus1, &select_mask, &write_mask, (fd_set *)NULL, &select_timeout);
        if (i < 0)
        {
            if (errno != EINTR) /* EINTR  A signal was caught. */
            {
                sciprint(_("Error. A signal has been caught.\n"));
                exit(0);
                continue;
            }
        }

        /* if there's something to output */
        if ( FD_ISSET(fd_out, &write_mask))
        {
            fflush(stdout);
        }
        if ( FD_ISSET(fd_err, &write_mask))
        {
            fflush(stderr);
        }

        /* if there's something to read */
        if (FD_ISSET(fd_in, &select_mask))
        {
            state = 1;
        }

        if (state)
        {
            i = getchar();
            if (i == LF)
            {
                state = 0;
            }
            return (i);
        }

        if (interrupt && (ismenu() == 1))
        {
            return (-1);
        }

    }
}
/*--------------------------------------------------------------------------*/
int XEvorgetchar(int interrupt)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
void main_sci (char *startup, int lstartup, int memory)
{
    Scierror(999, _("Scilab '%s' module not installed.\n"), "GUI");
}
/*--------------------------------------------------------------------------*/
void InitXsession(void)
{
    Scierror(999, _("Scilab '%s' module not installed.\n"), "GUI");
}
/*--------------------------------------------------------------------------*/
#endif /* not  _MSC_VER */
