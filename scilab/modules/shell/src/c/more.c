/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "more.h"
#include "scilabmode.h"
#include "sciprint.h"
#include "../../../console/includes/GetCharWithoutOutput.h"
/*-----------------------------------------------------------------------------------*/ 
#define MSG_MORE_NW " more ? "
/*-----------------------------------------------------------------------------------*/ 
int linesmore(void)
{
	int retval = 0;
	if (getScilabMode() != SCILAB_STD) 
	{
		int ch = 0;
		/* Scilab has not his own window */
		sciprint(MSG_MORE_NW);
		ch = getchar();
		if ( (ch != ' ') && (ch != '\n') && (ch != 'y') ) retval = 1;
	}
	else
	{
		int ch = 0;
		/* scilab has his own window */
		/* retval = 0 */
	}
	return retval;
}
/*-----------------------------------------------------------------------------------*/ 
