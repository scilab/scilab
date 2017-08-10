/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2008 - INRIA - Allan CORNET
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

#ifndef __GETENVC_H__
#define __GETENVC_H__

#include <wchar.h>
#include "dynlib_io.h"
#include "machine.h"

/**
 * Retrieve a system variable
 * @param[out] ierr the error code
 * @param[in] var the name of the variable
 * @param[out] buf the value of the variable
 * @param[out] buflen  the length of the variable
 * @param[in] iflag
 */
IO_IMPEXP void getenvc(int *ierr, const char *var, char *buf, int *buflen, int *iflag);

/**
* Searches for a file using environment paths
* @param[in] filename
* @param[in] environment variable where we search
* @return path found
*/
IO_IMPEXP char *searchEnv(const char *name, const char *env_var);
IO_IMPEXP wchar_t* searchEnvW(const wchar_t* _pwstName, const wchar_t* _pwstEnv);



#endif /* __GETENVC_H__ */
