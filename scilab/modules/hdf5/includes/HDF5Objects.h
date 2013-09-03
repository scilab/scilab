/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __HDF5OBJECTS_H__
#define __HDF5OBJECTS_H__

#define H5_NO_DEPRECATED_SYMBOLS
#undef H5_USE_16_API

#define H5Eset_auto_vers 2
#include <hdf5.h>
#include <hdf5_hl.h>

#undef H5_NO_DEPRECATED_SYMBOLS

//#define __HDF5OBJECTS_DEBUG__
//#define __HDF5ERROR_PRINT__

#define __SCILAB_MLIST_H5OBJECT__ "H5Object"
#define __SCILAB_HDF5_MAX_DIMS__ 64

#endif // __HDF5OBJECTS_H__
