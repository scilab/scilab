/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
