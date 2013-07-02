/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __GETVERSION_H__
#define __GETVERSION_H__

#include "BOOL.h"

/**
* get Scilab Version as a int array
* @param[out] size of returned array
* @return int array ver
* ver[0] Major version
* ver[1] Minor version
* ver[2] Maintenance version
* ver[3] timestamp
*/
int *getScilabVersion(int *sizeArrayReturned);

/**
* get Scilab version major number
* @return scilab version major
*/
int getScilabVersionMajor();

/**
* get Scilab version minor number
* @return scilab version minor
*/
int getScilabVersionMinor();

/**
* get Scilab version maintenance number
* @return scilab version maintenance
*/
int getScilabVersionMaintenance();

/**
* get Scilab version timestamp number
* @return scilab version timestamp
*/
int getScilabVersionTimestamp();

/**
* get Scilab version as a string
* @return scilab version
*/
char *getScilabVersionAsString(void);

/**
* get module version as a int array
* @param[in] module name
* @param[out] size of returned array
* @return int array ver
* ver[0] Major version
* ver[1] Minor version
* ver[2] Maintenance version
* ver[3] timestamp
*/
int* getModuleVersion(char *modulename, int *sizeArrayReturned);

/**
* get module version as a string
* @param[in] module name
* @return scilab version
*/
char *getModuleVersionInfoAsString(char *modulename);

/**
* get options used for this version
* @param[out]
* @return char ** with options used
*/
char **getScilabVersionOptions(int *sizeArrayReturned);

/**
* get release mode
* @return 'release' or 'debug'
*/
char *getReleaseMode(void);

/**
* get release date
* @return date of version
*/
char *getReleaseDate(void);

/**
* get release time
* @return time of version
*/
char *getReleaseTime(void);

/**
* get compiler used to build scilab
* @return 'VC++', 'ICC', 'GCC'
*/
char *getCompilerUsedToBuildScilab(void);

/**
* get Compiler Architecture
* @return 'x86' or 'x64'
*/
char *getCompilerArchitecture(void);

/**
* with modelica compiler ?
* @return TRUE or FALSE
*/
BOOL with_modelica_compiler(void);

/**
* with tcl/tk ?
* @return TRUE or FALSE
*/
BOOL with_tk(void);

#endif /* __GETVERSION_H__ */
/*--------------------------------------------------------------------------*/

