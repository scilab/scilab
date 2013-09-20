/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*---------------------------------------------------------------------------*/
#ifndef __ILIB_VERBOSE_H__
#define __ILIB_VERBOSE_H__

#include "dynlib_dynamic_link.h"
#include "BOOL.h"
#include "machine.h"

typedef enum
{
    ILIB_VERBOSE_NO_OUTPUT = 0,
    ILIB_VERBOSE_STANDARD = 1,
    ILIB_VERBOSE_MAX = 2
} ilib_verbose_level;

/**
* set verbose level for dynamic link functions
* @param[in] level
* @return TRUE if level is modified
*/
DYNAMIC_LINK_IMPEXP BOOL setIlibVerboseLevel(ilib_verbose_level _level);

/**
* get verbose level for dynamic link functions
* @return level
*/
DYNAMIC_LINK_IMPEXP ilib_verbose_level getIlibVerboseLevel(void);

/**
* get verbose level for dynamic link functions (for fortran external subroutines)
* @param[out] level
*/
DYNAMIC_LINK_IMPEXP int C2F(getilibverboselevel)(int *_level);

#endif /* __ILIB_VERBOSE_H__ */
/*---------------------------------------------------------------------------*/
