/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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