/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Francois Vogel
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
/* sciprint_full function */
/*--------------------------------------------------------------------------*/
/* sciprint geared towards long strings (>MAXPRINTF) */
/* the long string is splitted in elements of length equal to the number of columns  */
/* from lines()                                                                      */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sciprint_full.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "localization.h"
#include "scilines.h"
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
/*--------------------------------------------------------------------------*/
/* MAXCHARSSCIPRINT_FULL is for sciprint_full - more than this gets truncated */
#define MAXCHARSSCIPRINT_FULL 5000
/*--------------------------------------------------------------------------*/
void sciprint_full(char *fmt, ...)
{
    int lstr;
    va_list ap;
    char *s_buf = NULL;
    char *split_s_buf = NULL;
    int count = 0;
    int p_s = 0;
    static int colwidth;

    s_buf = MALLOC(sizeof(char) * (MAXCHARSSCIPRINT_FULL + 1));
    if (s_buf == (char *) 0)
    {
        sciprint(_("%s: No more memory.\n"), "sciprint_full");
        return;
    }

    /* number of columns as set by command lines() */
    colwidth = getConsoleWidth();

    split_s_buf = MALLOC(sizeof(char) * (colwidth + 1));
    if (split_s_buf == (char *) 0)
    {
        sciprint(_("%s: No more memory.\n"), "sciprint_full");
        FREE(s_buf);
        return;
    }

    va_start(ap, fmt);

#if defined(linux) || defined(_MSC_VER)
    count = vsnprintf (s_buf, MAXCHARSSCIPRINT_FULL - 1, fmt, ap );
    if (count == -1)
    {
        s_buf[MAXCHARSSCIPRINT_FULL - 1] = '\0';
    }
#else
    (void )vsprintf(s_buf, fmt, ap );
#endif

    va_end(ap);

    lstr = (int) strlen(s_buf);

    if (lstr < colwidth)
    {
        sciprint("%s", s_buf);
    }
    else
    {
        strncpy(split_s_buf, s_buf + p_s, colwidth - 1);
        split_s_buf[colwidth] = '\0';
        p_s = p_s + colwidth - 1;
        sciprint("%s", split_s_buf);
        sciprint("\n");
        while (p_s + colwidth - 1 < (int)lstr)
        {
            strncpy(split_s_buf, s_buf + p_s, colwidth - 1);
            split_s_buf[colwidth] = '\0';
            p_s = p_s + colwidth - 1;
            sciprint(_("  (cont'd) %s\n"), split_s_buf);
        }
        strncpy(split_s_buf, s_buf + p_s, lstr - p_s);
        split_s_buf[lstr - p_s] = '\0';
        sciprint(_("     (end) %s\n"), split_s_buf);
    }

    if (s_buf)
    {
        FREE(s_buf);
        s_buf = NULL;
    }
    if (split_s_buf)
    {
        FREE(split_s_buf);
        split_s_buf = NULL;
    }

}
/*--------------------------------------------------------------------------*/
