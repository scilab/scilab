/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#ifndef __SCAN_FUNCTIONS_H__
#define __SCAN_FUNCTIONS_H__

#include "do_xxscanf.h"

/**
* @TODO add comments
*/
void Free_Scan (int rowcount,
                int ncol,
                sfdir *type_s,
                entry **data);

/**
* @TODO add comments
*/
int Sci_Store (int nrow,
               int ncol,
               entry* data,
               sfdir *type,
               int retval);

/**
* @TODO add comments
*/
int Store_Scan (int *nrow,
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
