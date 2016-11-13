/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#ifndef __V2CUNITERROR_H__
#define __V2CUNITERROR_H__

#include "machine.h"
#include "dynlib_io.h"

/**
* error message for v2cunit subroutine fortran
* @param[in] _errorcode error code
* @param[in] _filename filename used by v2cunit
* @param[in] _length_filename length of _filename (fortran)
* @return 0
*/

/* !!! this function must be used ONLY by v2cunit */

IO_IMPEXP int C2F(v2cuniterror)(int *_errorcode, char *_filename,
                                unsigned long _length_filename);

#endif /* __V2CUNITERROR_H__ */
/*--------------------------------------------------------------------------*/