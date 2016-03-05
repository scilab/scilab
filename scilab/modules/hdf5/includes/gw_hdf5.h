/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __GW_HDF5_H__
#define __GW_HDF5_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_hdf5_scilab.h"
/*--------------------------------------------------------------------------*/
HDF5_SCILAB_IMPEXP int Initialize(void);
HDF5_SCILAB_IMPEXP int Finalize(void);
/*--------------------------------------------------------------------------*/
HDF5_SCILAB_IMPEXP int sci_hdf5_load_v1(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_hdf5_load_v2(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_hdf5_listvar_v2(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_hdf5_is_file(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5dump(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5open(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5close(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5read(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5ls(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_percent_H5Object_p(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_percent_H5Object_e(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5group(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5rm(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5dataset(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5write(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5attr(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5ln(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5readattr(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5flush(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5cp(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isFile(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isGroup(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isSet(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isAttr(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isSpace(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isType(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isRef(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isList(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5mount(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5umount(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5mv(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5get(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5label(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5exists(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isCompound(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_percent_H5Object_fieldnames(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isArray(char *fname, int* pvApiCtx);
HDF5_SCILAB_IMPEXP int sci_h5isVlen(char *fname, int* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_HDF5_H__ */
/*--------------------------------------------------------------------------*/
