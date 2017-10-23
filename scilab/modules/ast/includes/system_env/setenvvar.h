/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#include "stdio.h"
#include "PATH_MAX.h"

#include "dynlib_ast.h"

/* DIR_SEPARATOR : Under Windows by default is \ */
#ifdef _MSC_VER
#define DIR_SEPARATOR "\\"
#define DIR_SEPARATORW L"\\"
#else
#define DIR_SEPARATOR "/"
#define DIR_SEPARATORW L"/"
#endif

#define UNIX_SEPATATOR '/'
#define WINDOWS_SEPATATOR '\\'

EXTERN_AST BOOL IsTheGoodShell(void);
EXTERN_AST BOOL Set_Shell(void);
EXTERN_AST void Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
void SetScilabEnvironmentVariables(char *DefaultSCIPATH);
BOOL convertSlash(const char *path_in, char *path_out, BOOL slashToAntislash);
EXTERN_AST BOOL AntislashToSlash(const char *pathwindows, char *pathunix);
BOOL SlashToAntislash(const char *pathwindows, char *pathunix);
EXTERN_AST void SetScilabEnvironment(void);
#ifdef _MSC_VER
void SciEnvForWindows(void);
#else
int SciEnvForOthers(void);
#endif

