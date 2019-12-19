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

#ifndef __H5_WRITEDATATOFILE_H__
#define __H5_WRITEDATATOFILE_H__

#include "dynlib_hdf5_scilab.h"

#include <hdf5.h>

HDF5_SCILAB_IMPEXP int updateFileVersion(hid_t _iFile);
HDF5_SCILAB_IMPEXP int updateScilabVersion(hid_t _iFile);

HDF5_SCILAB_IMPEXP char* createGroupName(const char* _pstGroupName);
HDF5_SCILAB_IMPEXP char* createPathName(char* _pstGroupName, int _iIndex);
HDF5_SCILAB_IMPEXP int deleteHDF5Var(hid_t _iFile, const char* _pstName);

HDF5_SCILAB_IMPEXP int writeDoubleMatrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, double *_pdblData);
HDF5_SCILAB_IMPEXP int writeDoubleComplexMatrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, double *_pdblReal, double *_pdblImg);

HDF5_SCILAB_IMPEXP int writeStringMatrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, char **_pstData);

HDF5_SCILAB_IMPEXP int writeBooleanMatrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, int *_piData);

HDF5_SCILAB_IMPEXP int writePolyMatrix(hid_t _iFile, char* _pstDatasetName, char* _pstVarName, int _iDims, int* _piDims, int* _piNbCoef, double** _pdblData);
HDF5_SCILAB_IMPEXP int writePolyComplexMatrix(hid_t _iFile, char* _pstDatasetName, char* _pstVarName, int _iDims, int* _piDims, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

HDF5_SCILAB_IMPEXP int writeInteger8Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, char* _piData8);
HDF5_SCILAB_IMPEXP int writeInteger16Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, short* _piData16);
HDF5_SCILAB_IMPEXP int writeInteger32Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, int* _piData32);
HDF5_SCILAB_IMPEXP int writeInteger64Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, long long* _piData64);

HDF5_SCILAB_IMPEXP int writeUnsignedInteger8Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned char* _piData8);
HDF5_SCILAB_IMPEXP int writeUnsignedInteger16Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned short* _piData16);
HDF5_SCILAB_IMPEXP int writeUnsignedInteger32Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned int* _piData32);
HDF5_SCILAB_IMPEXP int writeUnsignedInteger64Matrix(hid_t _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned long long* _piData64);

HDF5_SCILAB_IMPEXP int writeSparseMatrix(hid_t _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);
HDF5_SCILAB_IMPEXP int writeSparseComplexMatrix(hid_t _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

HDF5_SCILAB_IMPEXP int writeBooleanSparseMatrix(hid_t _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

HDF5_SCILAB_IMPEXP int writeVoid(hid_t _iFile, char* _pstDatasetName);
HDF5_SCILAB_IMPEXP int writeUndefined(hid_t _iFile, char* _pstDatasetName);

HDF5_SCILAB_IMPEXP void* openList(hid_t _iFile, char* pstDatasetName, int _iNbItem);
HDF5_SCILAB_IMPEXP int addItemInList(hid_t _iFile, void* _pvList, int _iPos, char* _pstItemName);
HDF5_SCILAB_IMPEXP hid_t closeList(hid_t _iFile,  void* _pvList, char* _pstListName, int _iNbItem, int _iVarType);


/*scilab 6*/

HDF5_SCILAB_IMPEXP int writeDoubleMatrix6(hid_t parent, const char* name, int dims, int* pdims, double* data, hid_t xfer_plist);
HDF5_SCILAB_IMPEXP int writeDoubleComplexMatrix6(hid_t parent, const char* name, int dims, int* pdims, double* real, double* img, hid_t xfer_plist);
HDF5_SCILAB_IMPEXP int writeStringMatrix6(hid_t parent, const char* name, int dims, int* pdims, char** data, hid_t xfer_plist);
HDF5_SCILAB_IMPEXP int writeBooleanMatrix6(hid_t parent, const char* name, int dims, int* pdims, int* data, hid_t xfer_plist);
HDF5_SCILAB_IMPEXP int writeIntegerMatrix6(hid_t parent, const char* name, hid_t type, const char* prec, int dims, int* pdims, void* data, hid_t xfer_plist);

HDF5_SCILAB_IMPEXP hid_t openList6(hid_t parent, const char* name, const char* type);
HDF5_SCILAB_IMPEXP int closeList6(hid_t lst);
HDF5_SCILAB_IMPEXP int addItemStruct6(hid_t dataset, hobj_ref_t* refs, int pos, const char* name);
HDF5_SCILAB_IMPEXP int writeStructField6(hid_t parent, const char* name, int dims, int* pdims, hobj_ref_t* refs, hid_t xfer_plist);
HDF5_SCILAB_IMPEXP int writeVoid6(hid_t parent, const char* name, hid_t xfer_plist);
HDF5_SCILAB_IMPEXP int writeUndefined6(hid_t parent, const char* name, hid_t xfer_plist);
#endif /* !__H5_WRITEDATATOFILE_H__ */
