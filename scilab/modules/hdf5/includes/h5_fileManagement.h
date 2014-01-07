/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __H5_FILEMANAGEMENT_H__
#define __H5_FILEMANAGEMENT_H__

#include "dynlib_hdf5_scilab.h"

HDF5_SCILAB_IMPEXP void HDF5cleanup(void);
HDF5_SCILAB_IMPEXP int createHDF5File(char *name);
HDF5_SCILAB_IMPEXP int openHDF5File(char *name, int _iAppendMode);
HDF5_SCILAB_IMPEXP void closeHDF5File(int file);
HDF5_SCILAB_IMPEXP int isHDF5File(char* _pstFilename);

#endif /* !__H5_FILEMANAGEMENT_H__ */
