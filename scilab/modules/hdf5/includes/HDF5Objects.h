/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
