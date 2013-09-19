/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __FINDFILEEXTENSION_H__
#define __FINDFILEEXTENSION_H__

#include "dynlib_fileio.h"

/**
* Searches a path for an extension
* @param[in] pathname
* @return the address of the "." preceding the extension
* NULL character otherwise.
*/
FILEIO_IMPEXP char *FindFileExtension(char *filename);

#endif /* __FINDFILEEXTENSION_H__ */
/*--------------------------------------------------------------------------*/
