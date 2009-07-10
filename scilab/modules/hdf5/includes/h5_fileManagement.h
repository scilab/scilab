/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __H5_FILEMANAGEMENT_H__
#define __H5_FILEMANAGEMENT_H__

#include "dynlib_hdf5_scilab.h"

/**
*
* @param[in]
* @return
*/
HDF5_SCILAB_IMPEXP int createHDF5File(char *name);

/**
*
* @param[in]
* @return
*/
HDF5_SCILAB_IMPEXP int openHDF5File(char *name);

/**
*
* @param[in]
*/
HDF5_SCILAB_IMPEXP void closeHDF5File(int file);

#endif /* !__H5_FILEMANAGEMENT_H__ */
