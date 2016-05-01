/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
