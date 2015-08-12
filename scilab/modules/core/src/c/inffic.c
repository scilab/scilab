/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "core_math.h"
#include "inffic.h"
#include "stack-def.h"
#include "BOOL.h"
#include "stackinfo.h"
/*--------------------------------------------------------------------------*/
static char *dataStrings[] =
{
    /* on crash save file */
    "home/scilab.save",
    /* start_up instructions file */
    "exec('SCI/etc/scilab.start',-1);",
    /* demos instructions file */
    /* exit instructions file */
    "exec('SCI/etc/scilab.quit',-1);quit;"	 ,
    /* on exit , we try to catch errors */
    /* workaround to not have scilab "zombie" */
    /* bug 3672 */
    "exec('SCI/etc/scilab.quit','errcatch',-1);quit;"
};
/*--------------------------------------------------------------------------*/
static char stringCommand[bsiz] = "";
/*--------------------------------------------------------------------------*/
static BOOL haveTooManyVariables(void);
/*--------------------------------------------------------------------------*/
void C2F(inffic)(int *iopt, char *name, int *nc)
{
    char *returnedLine = get_sci_data_strings(*iopt);
    *nc = (int) strlen(returnedLine);
    strcpy(name, returnedLine);
}
/*--------------------------------------------------------------------------*/
char *get_sci_data_strings(int n)
{
    if (((n == QUIT_ID) || (n == QUIT_ERRCATCH_ID)) && haveTooManyVariables())
    {
        /* bug 8079 */
        /* if there are too many variables, we force to do a clear
           before to do scilab.quit
        */
        strcpy(stringCommand, "clear;");
        strcat(stringCommand, dataStrings[Max(Min(n, MAX_ID), 0)]);
    }
    else
    {
        strcpy(stringCommand, dataStrings[Max(Min(n, MAX_ID), 0)]);
    }
    return stringCommand;
}
/*--------------------------------------------------------------------------*/
static BOOL haveTooManyVariables(void)
{
    /* nb variables used by scilab.quit 10 */
#define NB_VARIABLES_SECURITY 10*2
    int nbVarTotal = 0;
    int nbVarUsed = 0;

    C2F(getvariablesinfo)(&nbVarTotal, &nbVarUsed);

    return (BOOL)(nbVarTotal - nbVarUsed <= NB_VARIABLES_SECURITY);
}
/*--------------------------------------------------------------------------*/
