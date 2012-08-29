/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __H5_WRITEDATATOFILE_H__
#define __H5_WRITEDATATOFILE_H__

#include "dynlib_hdf5_scilab.h"

HDF5_SCILAB_IMPEXP int updateFileVersion(int _iFile);
HDF5_SCILAB_IMPEXP int updateScilabVersion(int _iFile);

HDF5_SCILAB_IMPEXP char* createGroupName(char* _pstGroupName);
HDF5_SCILAB_IMPEXP char* createPathName(char* _pstGroupName, int _iIndex);


HDF5_SCILAB_IMPEXP int writeDoubleMatrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, double *_pdblData);
HDF5_SCILAB_IMPEXP int writeDoubleComplexMatrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, double *_pdblReal, double *_pdblImg);

HDF5_SCILAB_IMPEXP int writeStringMatrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, char **_pstData);

HDF5_SCILAB_IMPEXP int writeBooleanMatrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, int *_piData);

HDF5_SCILAB_IMPEXP int writePolyMatrix(int _iFile, char* _pstDatasetName, char* _pstVarName, int _iDims, int* _piDims, int* _piNbCoef, double** _pdblData);
HDF5_SCILAB_IMPEXP int writePolyComplexMatrix(int _iFile, char* _pstDatasetName, char* _pstVarName, int _iDims, int* _piDims, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

HDF5_SCILAB_IMPEXP int writeInteger8Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, char* _piData8);
HDF5_SCILAB_IMPEXP int writeInteger16Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, short* _piData16);
HDF5_SCILAB_IMPEXP int writeInteger32Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, int* _piData32);
HDF5_SCILAB_IMPEXP int writeInteger64Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, long long* _piData64);

HDF5_SCILAB_IMPEXP int writeUnsignedInteger8Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned char* _piData8);
HDF5_SCILAB_IMPEXP int writeUnsignedInteger16Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned short* _piData16);
HDF5_SCILAB_IMPEXP int writeUnsignedInteger32Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned int* _piData32);
HDF5_SCILAB_IMPEXP int writeUnsignedInteger64Matrix(int _iFile, char* _pstDatasetName, int _iDims, int* _piDims, unsigned long long* _piData64);

HDF5_SCILAB_IMPEXP int writeSparseMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);
HDF5_SCILAB_IMPEXP int writeSparseComplexMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

HDF5_SCILAB_IMPEXP int writeBooleanSparseMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

HDF5_SCILAB_IMPEXP int writeVoid(int _iFile, char* _pstDatasetName);
HDF5_SCILAB_IMPEXP int writeUndefined(int _iFile, char* _pstDatasetName);

HDF5_SCILAB_IMPEXP void* openList(int _iFile, char* pstDatasetName, int _iNbItem);
HDF5_SCILAB_IMPEXP int addItemInList(int _iFile, void* _pvList, int _iPos, char* _pstItemName);
HDF5_SCILAB_IMPEXP int closeList(int _iFile,  void* _pvList, char* _pstListName, int _iNbItem, int _iVarType);

#endif /* !__H5_WRITEDATATOFILE_H__ */
