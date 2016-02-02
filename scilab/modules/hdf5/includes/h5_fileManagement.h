/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __H5_FILEMANAGEMENT_H__
#define __H5_FILEMANAGEMENT_H__

#include "dynlib_hdf5_scilab.h"

HDF5_SCILAB_IMPEXP void HDF5cleanup(void);
HDF5_SCILAB_IMPEXP int createHDF5File(const char *name);
HDF5_SCILAB_IMPEXP int openHDF5File(const char *name, int _iAppendMode);
HDF5_SCILAB_IMPEXP void closeHDF5File(int file);
HDF5_SCILAB_IMPEXP int isHDF5File(const char* _pstFilename);

#endif /* !__H5_FILEMANAGEMENT_H__ */
