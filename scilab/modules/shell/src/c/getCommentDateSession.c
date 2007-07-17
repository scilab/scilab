/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include <time.h>
#include <stdio.h>
#include "getCommentDateSession.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
#define string_begin_session "// Begin Session : "
#define string_end_session   "// End Session   : "
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
	static char wday_name[7][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static char mon_name[12][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
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
