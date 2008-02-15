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
/*------------------------------------------------------------------------*/
#define string_begin_session _("// Begin Session : ")
#define string_end_session   _("// End Session   : ")
/*------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr);
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
char *ASCIItime(const struct tm *timeptr)
{
	char *wday_name[7] = {
		_("Sun"),
		_("Mon"),
		_("Tue"),
		_("Wed"),
		_("Thu"),
		_("Fri"),
		_("Sat")
	};
	char *mon_name[12] = {
		_("Jan"), _("Feb"), _("Mar"), _("Apr"), _("May"), _("Jun"),
		_("Jul"), _("Aug"), _("Sep"), _("Oct"), _("Nov"), _("Dec")
	};

	static char result[27];

	sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
		wday_name[timeptr->tm_wday],
		mon_name[timeptr->tm_mon],
		timeptr->tm_mday, timeptr->tm_hour,
		timeptr->tm_min, timeptr->tm_sec,
		1900 + timeptr->tm_year);

	return result;
}
/*------------------------------------------------------------------------*/
