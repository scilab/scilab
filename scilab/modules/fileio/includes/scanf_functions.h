/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
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

#ifndef __SCAN_FUNCTIONS_H__
#define __SCAN_FUNCTIONS_H__

#include "do_xxscanf.h"
#include "dynlib_fileio.h"

/**
* @TODO add comments
*/
FILEIO_IMPEXP void Free_Scan (int rowcount,
                              int ncol,
                              sfdir *type_s,
                              entry **data);

/**
* @TODO add comments
*/
FILEIO_IMPEXP int Store_Scan (int *nrow,
                              int *ncol,
                              sfdir *type_s,
                              sfdir *type,
                              int *retval,
                              int *retval_s,
                              rec_entry *buf,
                              entry **data,
                              int rowcount,
                              int n);

#endif /* __SCAN_FUNCTIONS_H__ */
