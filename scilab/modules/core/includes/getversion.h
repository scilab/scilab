/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __GETVERSION_H__
#define __GETVERSION_H__

#include <wchar.h>
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
char* getScilabVersionAsString(void);

/**
* get Scilab version as wide string
* @return scilab version
*/
wchar_t* getScilabVersionAsWideString(void);

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
int* getModuleVersion(wchar_t* _pwstModule, int *sizeArrayReturned);

/**
* get module version as a string
* @param[in] module name
* @return scilab version
*/
wchar_t* getModuleVersionInfoAsString(wchar_t* _pwstModule);

/**
* get options used for this version
* @param[out]
* @return char ** with options used
*/
wchar_t** getScilabVersionOptions(int *sizeArrayReturned);

/**
* get release mode
* @return 'release' or 'debug'
*/
wchar_t* getReleaseMode(void);

/**
* get release date
* @return date of version
*/
wchar_t* getReleaseDate(void);

/**
* get release time
* @return time of version
*/
wchar_t* getReleaseTime(void);

/**
* get compiler used to build scilab
* @return 'VC++', 'ICC', 'GCC'
*/
wchar_t* getCompilerUsedToBuildScilab(void);

/**
* get Compiler Architecture
* @return 'x86' or 'x64'
*/
wchar_t* getCompilerArchitecture(void);

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

