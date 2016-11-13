/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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

/*------------------------------------------------------------------------*/
#include <time.h>
#include <stdio.h>
#include <string.h>             /* strlen */
#include "getCommentDateSession.h"
#include "sci_malloc.h"
#include "BOOL.h"
#include "localization.h"
#include "os_string.h"
#include "freeArrayOfString.h"
/*------------------------------------------------------------------------*/
#define STRING_BEGIN_SESSION _("Begin Session : ")
#define FORMAT_SESSION "%s%s%s"
#define LONG_FORMAT_SESSION "%s%s%s%s"
#define MAX_wday 7              /* number days in a week */
#define MAX_mon 12              /* number of month in a year */
/*------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr);
static char *ASCIItimeShort(const struct tm *timeptr);
static char **getDays(void);
static char **getMonths(void);

/*------------------------------------------------------------------------*/
char *getCommentDateSession(BOOL longFormat)
{
    char *line = NULL;
    char *time_str = NULL;
    time_t timer = time(NULL);

    if (longFormat)
    {
        time_str = ASCIItime(localtime(&timer));
    }
    else
    {
        time_str = ASCIItimeShort(localtime(&timer));
    }

    if (time_str)
    {
        size_t historyLineSize = strlen(SESSION_PRAGMA_BEGIN) + strlen(time_str) + strlen(FORMAT_SESSION) + strlen(SESSION_PRAGMA_END) + 1;

        if (longFormat)
        {
            historyLineSize += strlen(STRING_BEGIN_SESSION);
        }

        line = (char *)MALLOC(sizeof(char) * historyLineSize);

        if (line)
        {
            if (longFormat)
            {
                sprintf(line, LONG_FORMAT_SESSION, SESSION_PRAGMA_BEGIN, STRING_BEGIN_SESSION, time_str, SESSION_PRAGMA_END);
            }
            else
            {
                sprintf(line, FORMAT_SESSION, SESSION_PRAGMA_BEGIN, time_str, SESSION_PRAGMA_END);
            }
        }
        FREE(time_str);
        time_str = NULL;
    }

    return line;
}

/*------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr)
{
    char **wday_name = getDays();
    char **mon_name = getMonths();
    char *result = NULL;

    if ((wday_name) && (mon_name))
    {
#define FORMAT_TIME "%s %s%3d %.2d:%.2d:%.2d %d"
        int len_result = (int)strlen(wday_name[timeptr->tm_wday]) + (int)strlen(mon_name[timeptr->tm_mon]) + (int)strlen(FORMAT_TIME);

        result = (char *)MALLOC(sizeof(char) * (len_result + 1));
        if (result)
        {
            sprintf(result, FORMAT_TIME,
                    wday_name[timeptr->tm_wday],
                    mon_name[timeptr->tm_mon], timeptr->tm_mday, timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec, 1900 + timeptr->tm_year);
        }
    }
    else
    {
        result = (char *)MALLOC(sizeof(char));
        strcpy(result, "");
    }

    /* free pointers */
    freeArrayOfString(wday_name, MAX_wday);
    freeArrayOfString(mon_name, MAX_mon);

    return result;
}

/*------------------------------------------------------------------------*/
static char **getDays(void)
{
    char **days = NULL;

    days = (char **)MALLOC(sizeof(char *) * MAX_wday);
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
static char **getMonths(void)
{
    char **months = NULL;

    months = (char **)MALLOC(sizeof(char *) * MAX_mon);
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
static char *ASCIItimeShort(const struct tm *timeptr)
{
#define FORMAT_TIME_SHORT "%.2d/%.2d/%.4d %.2d:%.2d:%.2d"
    int len_result = (int)20;   //strlen("21/05/2011 14:11:04")+1

    char *result = (char *)MALLOC(sizeof(char) * (len_result + 1));

    if (result)
    {
        sprintf(result, FORMAT_TIME_SHORT,
                timeptr->tm_mday, timeptr->tm_mon + 1 , 1900 + timeptr->tm_year, timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec);
    }

    return result;
}
/*------------------------------------------------------------------------*/
