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
#ifndef __GETDRIVES_H__
#define __GETDRIVES_H__

/**
* Get the drive letters of all mounted filesystems on the computer.
* @param[out] nbDrives
* @return List of Drives
*/
char **getdrives(int *nbDrives);

#endif /* __GETDRIVES_H__ */
