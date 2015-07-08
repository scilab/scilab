/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __HDF5_GW_HXX__
#define __HDF5_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_hdf5_scilab.h"
}

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_export_to_hdf5, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_import_from_hdf5, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_import_from_hdf5_v3, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_listvar_in_hdf5, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_listvar_in_hdf5_v3, HDF5_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_hdf5_file_version, HDF5_SCILAB_IMPEXP);

#endif /* !__HDF5_GW_HXX__ */
