/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>

#include "getvariablesname.h"
#include "commandwords.h"
#include "getfunctionsname.h"
#include "getmacrosname.h"
#include "BOOL.h"
#include "ScilabKeywords.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/

/*
** INFO : char** results will be given back to Java
** they should be NULL terminated in order Java can
**
*/

char **GetVariablesName(void)
{
#pragma message ("WARNING : GetVariablesName return a static list should be linked to declared variables.")
    int i = 0;
    char *pstStaticVarName[] = {"%i", "%pi", "%eps", "%t", "%T", "%f", "%F"};
    char **VarsName = MALLOC(8 * sizeof(char *));
    for (i = 0 ; i < 7 ; ++i)
    {
        VarsName[i] = os_strdup(pstStaticVarName[i]);
    }
    VarsName[7] = NULL;
    return VarsName;
#if 0
    int returnedArraySize = 0;
    char **VarsName = getVariablesName(&returnedArraySize, TRUE);
    /* SWIG char array must be finished by NULL */
    if (VarsName)
    {
        VarsName = (char**)REALLOC(VarsName, sizeof(char*) * (returnedArraySize + 1));
        VarsName[returnedArraySize] = NULL;
    }
    return VarsName;
#endif
}
/*--------------------------------------------------------------------------*/
char **GetCommandsName(void)
{
    /*
    ** This function is now useless
    ** will be removed by master-merge
    ** See http://codereview.scilab.org/#change,1456
    **
    */
#pragma message ("WARNING : GetCommandsName is now useless and will be removed by master-merge.")
    int i = 0;
    char *pstStaticCommandKeywords[2] = {"if", NULL };
    char **CommandKeywords = MALLOC(2 * sizeof(char *));
    for (i = 0 ; i < 2 ; ++i)
    {
        CommandKeywords[i] = os_strdup(pstStaticCommandKeywords[i]);
    }
    return CommandKeywords;
#if 0
    int returnedArraySize = 0;
    char **CommandKeywords = getcommandkeywords(&returnedArraySize);
    if (CommandKeywords)
    {
        CommandKeywords = (char**)REALLOC(CommandKeywords, sizeof(char*) * (returnedArraySize + 1));
        CommandKeywords[returnedArraySize] = NULL;
    }
    return CommandKeywords;
#endif
}
/*--------------------------------------------------------------------------*/
char **GetFunctionsName(void)
{
#pragma message ("WARNING : GetFunctionsName return a static list should be linked to declared functions.")
    int i = 0;
    char *pstStaticFunctionsName[] = {"disp", "size"};
    char **FunctionsName = MALLOC(3 * sizeof(char *));
    for (i = 0 ; i < 2 ; ++i)
    {
        FunctionsName[i] = os_strdup(pstStaticFunctionsName[i]);
    }
    FunctionsName[2] = NULL;
    return FunctionsName;
#if 0
    int returnedArraySize = 0;
    char **FunctionsName = getFunctionsName(&returnedArraySize);
    if (FunctionsName)
    {
        FunctionsName = (char**)REALLOC(FunctionsName, sizeof(char*) * (returnedArraySize + 1));
        FunctionsName[returnedArraySize] = NULL;
    }
    return FunctionsName;
#endif
}
/*--------------------------------------------------------------------------*/
char **GetMacrosName(void)
{
#pragma message ("WARNING : GetFunctionsName return a static list should be linked to declared macros.")
    int i = 0;
    char *pstStaticMacrosName[] = {"plot3d", "acosh"};
    char **MacrosName = MALLOC(3 * sizeof(char *));
    for (i = 0 ; i < 2 ; ++i)
    {
        MacrosName[i] = os_strdup(pstStaticMacrosName[i]);
    }
    MacrosName[2] = NULL;
    return MacrosName;

#if 0
    int returnedArraySize = 0;
    char **MacrosName = getMacrosName(&returnedArraySize);
    if (MacrosName)
    {
        MacrosName = (char**)REALLOC(MacrosName, sizeof(char*) * (returnedArraySize + 1));
        MacrosName[returnedArraySize] = NULL;
    }
    return MacrosName;
#endif
}
/*--------------------------------------------------------------------------*/
