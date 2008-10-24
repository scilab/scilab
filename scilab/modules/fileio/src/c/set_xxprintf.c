/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "set_xxprintf.h"
#include "sciprint.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static int voidflush(FILE *fp);
/*--------------------------------------------------------------------------*/
extern char sprintf_buffer[MAX_SPRINTF_SIZE];
/*--------------------------------------------------------------------------*/

#include "scilabmode.h"

void set_xxprintf(FILE *fp,XXPRINTF *xxprintf,FLUSH *flush,char **target)
{
	 setOutputInUTF(TRUE); /*default output encoding is UTF*/
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
		 setOutputInUTF(FALSE); /*output in  system locale  for external file*/
	}
}
/*--------------------------------------------------------------------------*/
static int voidflush(FILE *fp)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
