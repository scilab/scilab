/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Bruno JOFFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef   	__ERRMSG_H__
#define   	__ERRMSG_H__

#include "machine.h"

/*
* errmsg displays the error message
* @param[in] n : error number, if n exceeds the maximum error number this
* routines displays the error message contained in buf
* @param[out] errtyp : error type (recoverable:0 or not:1)
*/
int C2F(errmsg)(integer *n,integer *errtyp);

#endif 	    /* !__ERRMSG_H__ */
