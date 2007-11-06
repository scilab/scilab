/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include "set_xxprintf.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
static int voidflush(FILE *fp);
/*-----------------------------------------------------------------------------------*/
extern char sprintf_buffer[MAX_SPRINTF_SIZE];
/*-----------------------------------------------------------------------------------*/
void set_xxprintf(FILE *fp,XXPRINTF *xxprintf,FLUSH *flush,char **target)
{
	if (fp == (FILE *) 0)
	{
		/* sprintf */
		*target = sprintf_buffer;
		*flush = voidflush;
		*xxprintf = (XXPRINTF) sprintf;
	}
	else if ( fp == stdout )
	{
		/* sciprint2 */
		*target =  (char *) 0;
		*flush = fflush;
		*xxprintf = (XXPRINTF) sciprint2;
	}
	else
	{
		/* fprintf */
		*target = (char *) fp;
		*flush = fflush;
		*xxprintf = (XXPRINTF) fprintf;
	}
}
/*-----------------------------------------------------------------------------------*/
static int voidflush(FILE *fp)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/
