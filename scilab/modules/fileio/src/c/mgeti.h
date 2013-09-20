
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __MGETI_H__
#define __MGETI_H__

#include "machine.h"
#include "dynlib_fileio.h"
/**
 * <long-description>
 *
 * @param char type[]
 * @param fd
 * @return <ReturnValue>
 */
int SWAP(char type[], int *fd);

/**
 * <long-description>
 *
 * @param fd
 * @param res
 * @param n
 * @param char type[]
 * @param ierr
 */
FILEIO_IMPEXP void C2F(mgeti) (int *fd, long long *res, int *n, char type[], int *ierr);

#endif /* __MGETI_H__ */

