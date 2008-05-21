
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
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
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*------------------------------------------------------------------------*/
#define string_begin_session _("// Begin Session : ")
#define string_end_session   _("// End Session   : ")
#define MAX_wday 7 /* number days in a week */
#define MAX_mon 12 /* number of month in a year */
/*------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr);
static char** getDays(void);
static char** getMonths(void);
/*------------------------------------------------------------------------*/
char *getCommentDateSession(BOOL BeginSession)
{
	char *line = NULL;
	char *time_str = NULL;
	time_t timer;
	timer=time(NULL);

	time_str = ASCIItime(localtime(&timer));

	if (BeginSession)
	{
		line = (char*)MALLOC(sizeof(char)*(strlen(string_begin_session)+strlen(time_str)+1));
		if (line) sprintf(line,"%s%s",string_begin_session,time_str);
	}
	else
	{
		line = (char*)MALLOC(sizeof(char)*(strlen(string_end_session)+strlen(time_str)+1));
		if (line) sprintf(line,"%s%s",string_end_session,time_str);
	}
	return line;
}
/*------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr)
{

	int i = 0;
	char **wday_name = getDays();
	char **mon_name = getMonths();
	static char result[27];

	if ( (wday_name) && (mon_name) )
	{
		sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
				wday_name[timeptr->tm_wday],
				mon_name[timeptr->tm_mon],
				timeptr->tm_mday, timeptr->tm_hour,
				timeptr->tm_min, timeptr->tm_sec,
				1900 + timeptr->tm_year);
	}
	else
	{
		strcpy(result,"");
	}
	
	/* free pointers */
	if (wday_name)
	{
		for (i = 0;i< MAX_wday;i++) 
		{
			if (wday_name[i]) { FREE(wday_name[i]); wday_name[i] = NULL;}
		}
		FREE(wday_name);
		wday_name = NULL;
	}
	
	if (mon_name)
	{
		for (i = 0;i< MAX_mon;i++) 
		{
			if (mon_name[i]) { FREE(mon_name[i]); mon_name[i] = NULL;}
		}
		FREE(mon_name);
		mon_name = NULL;
	}

	return result;
}
/*------------------------------------------------------------------------*/
static char** getDays(void)
{
	char **days = NULL;
	days = (char **)MALLOC(sizeof(char*)*MAX_wday);
	if (days)
	{
		days[0] = strdup(_("Sun"));
		days[1] = strdup(_("Mon"));
		days[2] = strdup(_("Tue"));
		days[3] = strdup(_("Wed"));
		days[4] = strdup(_("Thu"));
		days[5] = strdup(_("Fri"));
		days[6] = strdup(_("Sat"));
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
		months[0] = strdup(_("Jan"));
		months[1] = strdup(_("Feb"));
		months[2] = strdup(_("Mar"));
		months[3] = strdup(_("Apr"));
		months[4] = strdup(_("May"));
		months[5] = strdup(_("Jun"));
		months[6] = strdup(_("Jul"));
		months[7] = strdup(_("Aug"));
		months[8] = strdup(_("Sep"));
		months[9] = strdup(_("Oct"));
		months[10] = strdup(_("Nov"));
		months[11] = strdup(_("Dec"));
	}
	return months;
}
/*------------------------------------------------------------------------*/
