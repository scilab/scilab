/*--------------------------------------------------------------------------*/

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GETDATE_H__
#define __GETDATE_H__

#ifdef _MSC_VER
#include <sys/types.h> 
#include <sys/timeb.h>
#else
#include <sys/time.h> 
#endif

#include "machine.h"
/**
 * TODO : comment
 * @param dt
 * @param ierr
 */
void C2F(scigetdate) (time_t *dt, int *ierr);


/**
 * TODO : comment
 * @param dt
 * @param w
 */
void C2F(convertdate) (time_t *dt, int w[]);

#endif /* __GETDATE_H__ */
