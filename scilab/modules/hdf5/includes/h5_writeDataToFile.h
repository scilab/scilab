/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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

HDF5_SCILAB_IMPEXP char* createGroupName(char* _pstGroupName);
HDF5_SCILAB_IMPEXP char* createPathName(char* _pstGroupName, int _iIndex);


HDF5_SCILAB_IMPEXP int writeDoubleMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, double *_pdblData);
HDF5_SCILAB_IMPEXP int writeDoubleComplexMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg);

HDF5_SCILAB_IMPEXP int writeStringMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, char **_pstData);

HDF5_SCILAB_IMPEXP int writeBooleanMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int *_piData);

HDF5_SCILAB_IMPEXP int writePolyMatrix(int _iFile, char* _pstDatasetName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblData);
HDF5_SCILAB_IMPEXP int writePolyComplexMatrix(int _iFile, char* _pstDatasetName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

HDF5_SCILAB_IMPEXP int writeInterger8Matrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, char* _piData8);
HDF5_SCILAB_IMPEXP int writeInterger16Matrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, short* _piData16);
HDF5_SCILAB_IMPEXP int writeInterger32Matrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int* _piData32);
HDF5_SCILAB_IMPEXP int writeInterger64Matrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, long long* _piData64);

HDF5_SCILAB_IMPEXP int writeSparseMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);
HDF5_SCILAB_IMPEXP int writeSparseComplexMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

HDF5_SCILAB_IMPEXP int writeBooleanSparseMatrix(int _iFile, char* _pstDatasetName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

HDF5_SCILAB_IMPEXP void* openList(int _iFile, char* pstDatasetName, int _iNbItem);
HDF5_SCILAB_IMPEXP int addItemInList(int _iFile, void* _pvList, int _iPos, char* _pstItemName);
HDF5_SCILAB_IMPEXP int closeList(int _iFile,  void* _pvList, char* _pstListName, int _iNbItem, int _iVarType);

#endif /* !__H5_WRITEDATATOFILE_H__ */
