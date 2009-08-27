/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_HDF5_H__
#define __GW_HDF5_H__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_hdf5_scilab.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/ 
HDF5_SCILAB_IMPEXP int gw_hdf5(void);
/*--------------------------------------------------------------------------*/ 
HDF5_SCILAB_IMPEXP int sci_export_to_hdf5(char *fname,unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_import_from_hdf5(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_HDF5_H__ */
/*--------------------------------------------------------------------------*/
