/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#ifndef __HDF5_GW_HXX__
#define __HDF5_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_hdf5_scilab.h"
}

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_hdf5_save, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_hdf5_load, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_hdf5_load_v3, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_hdf5_listvar, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_hdf5_listvar_v3, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_hdf5_file_version, HDF5_SCILAB_IMPEXP);

#endif /* !__HDF5_GW_HXX__ */
