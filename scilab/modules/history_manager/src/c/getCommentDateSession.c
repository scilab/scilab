/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <time.h>
#include <stdio.h>
#include <string.h> /* strlen */
#include "getCommentDateSession.h"
#include "MALLOC.h"
#include "localization.h"
#include "os_strdup.h"
#include "freeArrayOfString.h"
/*------------------------------------------------------------------------*/
#define STRING_BEGIN_SESSION _("Begin Session : ")
#define FORMAT_SESSION "%s%s%s%s"
#define MAX_wday 7 /* number days in a week */
#define MAX_mon 12 /* number of month in a year */
/*------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr);
static char** getDays(void);
static char** getMonths(void);
/*------------------------------------------------------------------------*/
char *getCommentDateSession(void)
{
    char *line = NULL;
    char *time_str = NULL;
    time_t timer;
    timer=time(NULL);

    time_str = ASCIItime(localtime(&timer));

    if (time_str)
    {
        line = (char*)MALLOC(sizeof(char)*(strlen(SESSION_PRAGMA_BEGIN) + 
                                           strlen(STRING_BEGIN_SESSION) + 
                                           strlen(time_str) + 
                                           strlen(FORMAT_SESSION) + 
                                           strlen(SESSION_PRAGMA_END) + 1));
        if (line) sprintf(line, FORMAT_SESSION, SESSION_PRAGMA_BEGIN,
                                                STRING_BEGIN_SESSION, 
                                                time_str,
                                                SESSION_PRAGMA_END);
        FREE(time_str);
        time_str = NULL;
    }

    return line;
}
/*------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr)
{
    int i = 0;
    char **wday_name = getDays();
    char **mon_name = getMonths();
    char *result = NULL;

    if ( (wday_name) && (mon_name) )
    {
        #define FORMAT_TIME "%s %s%3d %.2d:%.2d:%.2d %d"
        int len_result = (int) strlen(wday_name[timeptr->tm_wday]) +
            (int) strlen(mon_name[timeptr->tm_mon]) +
            (int) strlen(FORMAT_TIME);

        result = (char*)MALLOC(sizeof(char)*(len_result + 1));
        if (result)
        {
            sprintf(result, FORMAT_TIME,
                wday_name[timeptr->tm_wday],
                mon_name[timeptr->tm_mon],
                timeptr->tm_mday, timeptr->tm_hour,
                timeptr->tm_min, timeptr->tm_sec,
                1900 + timeptr->tm_year);
        }
    }
    else
    {
        strcpy(result, "");
    }

    /* free pointers */
    freeArrayOfString(wday_name, MAX_wday);
    freeArrayOfString(mon_name, MAX_mon);

    return result;
}
/*------------------------------------------------------------------------*/
static char** getDays(void)
{
    char **days = NULL;
    days = (char **)MALLOC(sizeof(char*)*MAX_wday);
    if (days)
    {
        days[0] = os_strdup(_("Sun"));
        days[1] = os_strdup(_("Mon"));
        days[2] = os_strdup(_("Tue"));
        days[3] = os_strdup(_("Wed"));
        days[4] = os_strdup(_("Thu"));
        days[5] = os_strdup(_("Fri"));
        days[6] = os_strdup(_("Sat"));
    }
    return days;
}
/*------------------------------------------------------------------------*/
static char** getMonths(void)
{
    char **months = NULL;
    months = (char **)MALLOC(sizeof(char*)*MAX_mon);
    if (months)
    {
        /* initialize month */ 
        months[0] = os_strdup(_("Jan"));
        months[1] = os_strdup(_("Feb"));
        months[2] = os_strdup(_("Mar"));
        months[3] = os_strdup(_("Apr"));
        months[4] = os_strdup(_("May"));
        months[5] = os_strdup(_("Jun"));
        months[6] = os_strdup(_("Jul"));
        months[7] = os_strdup(_("Aug"));
        months[8] = os_strdup(_("Sep"));
        months[9] = os_strdup(_("Oct"));
        months[10] = os_strdup(_("Nov"));
        months[11] = os_strdup(_("Dec"));
    }
    return months;
}
/*------------------------------------------------------------------------*/
