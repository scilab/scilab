/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "stdio.h"
#include "PATH_MAX.h"

#include "dynlib_ast.h"

/* DIR_SEPARATOR : Under Windows by default is \ */
#ifdef _MSC_VER
#define DIR_SEPARATOR "\\"
#else
#define DIR_SEPARATOR "/"
#endif

#define UNIX_SEPATATOR '/'
#define WINDOWS_SEPATATOR '\\'

EXTERN_AST bool IsTheGoodShell(void);
EXTERN_AST bool Set_Shell(void);
EXTERN_AST void Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
EXTERN_AST void SciEnvForWindows(void);
EXTERN_AST void SetScilabEnvironmentVariables(char *DefaultSCIPATH);
EXTERN_AST bool convertSlash(const char *path_in, char *path_out, bool slashToAntislash);
EXTERN_AST bool AntislashToSlash(const char *pathwindows, char *pathunix);
EXTERN_AST bool SlashToAntislash(const char *pathwindows, char *pathunix);
EXTERN_AST bool isdir(const char * path);
EXTERN_AST bool createdirectory(const char *path);
EXTERN_AST bool isDrive(const char *strname);
EXTERN_AST void SetScilabEnvironment(void);
EXTERN_AST void SetScilabVariables(void);
EXTERN_AST int SciEnvForOthers(void);

