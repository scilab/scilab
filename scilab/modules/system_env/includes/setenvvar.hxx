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
#include "configvariable.hxx"

#ifdef _MSC_VER
	#ifndef MAX_PATH_SHORT
		#define MAX_PATH_SHORT 260
	#endif
	
	//Only for windows
	#pragma warning(disable : 4996) //It's not beautifull but that works !
#endif

/* DIR_SEPARATOR : Under Windows by default is \ */
#ifdef _MSC_VER
	#define DIR_SEPARATOR "\\"
#else
	#define DIR_SEPARATOR "/"
#endif
#define SCI_VERSION_STRING "scilab-branch-YaSp"
#define UNIX_SEPATATOR '/'
#define WINDOWS_SEPATATOR '\\'

static char SCIHOMEPATH[PATH_MAX*2] = "empty_SCIHOME";

bool IsTheGoodShell(void);
bool Set_Shell(void);
bool Set_YASP_PATH(char *DefaultPath);
bool Set_HOME_PATH(char *DefaultPath);
bool Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
EXTERN_SYSTEM_ENV char *getScilabDirectory(bool UnixStyle);
void SciEnvForWindows(void);
void SetScilabEnvironmentVariables(char *DefaultSCIPATH);
bool setYASPHOME(void);
bool convertSlash(char *path_in,char *path_out,bool slashToAntislash);
bool AntislashToSlash(char *pathwindows,char *pathunix);
bool slashToAntislash(char *pathunix,char *pathwindows);
char *getYASPpath(void);
void setYASPpath(char *path);
void setYASPHome(char *path);
char *getYASPHome(void);
void getenvc(int *ierr,char *var,char *buf,int *buflen,int *iflag);
char *getshortpathname(char *longpathname,bool *convertok);
bool isdir(const char * path);
bool createdirectory(const char *path);
bool isDrive(const char *strname);
EXTERN_SYSTEM_ENV void SetScilabEnvironment(void);
int SciEnvForOthers(void);

