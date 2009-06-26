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

#ifndef __H5_READDATATOFILE_H__
#define __H5_READDATATOFILE_H__

#include <hdf5.h>

int isComplexData(int _iDatasetId);

int getVariableNames(int _iFile, char **pstNameList);

int getDataSetId(int  _iFile);

int getDataSetIdFromName(int _iFile, char *_pstName);

int getDataSetDims(int _iDatasetId, int *_piRows, int *_piCols);

int getScilabTypeFromDataSet(int _iDatasetId);

int getDatasetPrecision(int _iDatasetId, int* _piPrec);

int readDoubleMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblData);
int readDoubleComplexMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg);

int readStringMatrix(int _iDatasetId, int _iRows, int _iCols, char **_pstData);

int readBooleanMatrix(int _iDatasetId, int _iRows, int _iCols, int* _piData);

int readPolyMatrix(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblData);
int readPolyComplexMatrix(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblReal, double **_pdblImg);

int readInterger8Matrix(int _iDatasetId, int _iRows, int _iCols, char* _pcData);
int readInterger16Matrix(int _iDatasetId, int _iRows, int _iCols, short* _psData);
int readInterger32Matrix(int _iDatasetId, int _iRows, int _iCols, int* _piData);
int readInterger64Matrix(int _iDatasetId, int _iRows, int _iCols, long long* _pllData);

int getListItemReferences(int _iDatasetId, hobj_ref_t** _piItemRef);

int getListItemDataset(int _iDatasetId, void* _piItemRef, int _iItemPos, int* _piItemDataset);

int deleteListItemReferences(int _iDatasetId, void* _piItemRef);

#endif /* !__H5_READDATATOFILE_H__ */
