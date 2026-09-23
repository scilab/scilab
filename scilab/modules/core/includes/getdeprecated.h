/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __GETDEPRECATED_H__
#define __GETDEPRECATED_H__

#include "dynlib_core.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Return the UTF-8 names of deprecated Scilab functions.
     * The returned array must be released with freeArrayOfString.
     *
     * @param[out] size number of returned names
     * @return allocated array of names, or NULL when empty
     */
    CORE_IMPEXP char** getDeprecatedNames(int* size);

#ifdef __cplusplus
}
#endif

#endif /* __GETDEPRECATED_H__ */
