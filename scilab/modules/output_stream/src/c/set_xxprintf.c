/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "set_xxprintf.h"
#include "sciprint.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
/* local function used to flush with sprintf */
static int voidflush(FILE *fp);
/* local function used to call scivprint */
static int local_sciprint (int iv, char *fmt,...);
/*--------------------------------------------------------------------------*/
extern char sprintf_buffer[MAX_SPRINTF_SIZE];
/*--------------------------------------------------------------------------*/
void set_xxprintf(FILE *fp,XXPRINTF *xxprintf,FLUSH *flush,char **target)
{
    /* Force Windows display to have two-digit exponent. */
#ifdef _MSC_VER
    _set_output_format(_TWO_DIGIT_EXPONENT); 
#endif
    /* re-initialize value of sprintf_buffer */
    strcpy(sprintf_buffer, "");
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
        *xxprintf = (XXPRINTF) local_sciprint;
    }
    else
    {
        /* fprintf */
        *target = (char *) fp;
        *flush = fflush;
        *xxprintf = (XXPRINTF) fprintf;
    }
}
/*--------------------------------------------------------------------------*/
static int voidflush(FILE *fp)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
static int local_sciprint (int iv, char *fmt,...)
{
    int count = 0;
    va_list ap;

    va_start(ap,fmt);
    count = scivprint(fmt, ap);
    va_end (ap);

    return count;
}
/*--------------------------------------------------------------------------*/
