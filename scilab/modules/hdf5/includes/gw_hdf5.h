/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_HDF5_H__
#define __GW_HDF5_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_hdf5_scilab.h"
/*--------------------------------------------------------------------------*/
HDF5_SCILAB_IMPEXP int gw_hdf5(void);
/*--------------------------------------------------------------------------*/
HDF5_SCILAB_IMPEXP int sci_export_to_hdf5(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_import_from_hdf5(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_listvar_in_hdf5(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_is_hdf5_file(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5dump(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5open(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5close(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5read(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5ls(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_percent_H5Object_p(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_percent_H5Object_e(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5group(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5rm(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5dataset(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5write(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5attr(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5ln(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5readattr(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5flush(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5cp(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isFile(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isGroup(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isSet(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isAttr(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isSpace(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isType(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isRef(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isList(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5mount(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5umount(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5mv(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5get(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5label(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5exists(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isCompound(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_percent_H5Object_fieldnames(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isArray(char *fname, unsigned long fname_len);
HDF5_SCILAB_IMPEXP int sci_h5isVlen(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_HDF5_H__ */
/*--------------------------------------------------------------------------*/
