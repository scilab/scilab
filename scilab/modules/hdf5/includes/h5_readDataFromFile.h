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

#ifndef __H5_READDATATOFILE_H__
#define __H5_READDATATOFILE_H__

#include <hdf5.h>
#include "dynlib_hdf5_scilab.h"


#define unknow_type     -2
#define sci_undefined   -1
#define sci_void        0

char* getScilabVersionAttribute(int _iFile);
HDF5_SCILAB_IMPEXP int getSODFormatAttribute(int _iFile);

HDF5_SCILAB_IMPEXP int isComplexData(int _iDatasetId);

HDF5_SCILAB_IMPEXP int getVariableNames(int _iFile, char **pstNameList);

HDF5_SCILAB_IMPEXP int getDataSetId(int  _iFile);

HDF5_SCILAB_IMPEXP int getDataSetIdFromName(int _iFile, const char *_pstName);
HDF5_SCILAB_IMPEXP void closeDataSet(int _id);

HDF5_SCILAB_IMPEXP int getDatasetInfo(int _iDatasetId, int* _iComplex, int* _iDims, int* _piDims);
HDF5_SCILAB_IMPEXP int getDatasetDims(int _iDatasetId, int *_piRows, int *_piCols);
HDF5_SCILAB_IMPEXP int getListDims(int _iDatasetId, int *_piItem);

HDF5_SCILAB_IMPEXP int getScilabTypeFromDataSet(int _iDatasetId);

HDF5_SCILAB_IMPEXP int getDatasetPrecision(int _iDatasetId, int* _piPrec);

HDF5_SCILAB_IMPEXP int getSparseDimension(int _iDatasetId, int* _piRows, int * _piCols, int* _piNbItem);

HDF5_SCILAB_IMPEXP int readEmptyMatrix(int _iDatasetId);

HDF5_SCILAB_IMPEXP int readDoubleMatrix(int _iDatasetId, double *_pdblData);
HDF5_SCILAB_IMPEXP int readDoubleComplexMatrix(int _iDatasetId, double *_pdblReal, double *_pdblImg);

HDF5_SCILAB_IMPEXP int readStringMatrix(int _iDatasetId, char **_pstData);
HDF5_SCILAB_IMPEXP int freeStringMatrix(int _iDatasetId, char** _pstData);

HDF5_SCILAB_IMPEXP int readBooleanMatrix(int _iDatasetId, int* _piData);

HDF5_SCILAB_IMPEXP int readPolyMatrix(int _iDatasetId, char* _pstVarname, int _iDims, int* _piDims, int* _piNbCoef, double **_pdblData);
HDF5_SCILAB_IMPEXP int readPolyComplexMatrix(int _iDatasetId, char* _pstVarname, int _iDims, int* _piDims, int* _piNbCoef, double **_pdblReal, double **_pdblImg);

HDF5_SCILAB_IMPEXP int readInteger8Matrix(int _iDatasetId, char* _pcData);
HDF5_SCILAB_IMPEXP int readInteger16Matrix(int _iDatasetId, short* _psData);
HDF5_SCILAB_IMPEXP int readInteger32Matrix(int _iDatasetId, int* _piData);
HDF5_SCILAB_IMPEXP int readInteger64Matrix(int _iDatasetId, long long* _pllData);

HDF5_SCILAB_IMPEXP int readUnsignedInteger8Matrix(int _iDatasetId, unsigned char* _pucData);
HDF5_SCILAB_IMPEXP int readUnsignedInteger16Matrix(int _iDatasetId, unsigned short* _pusData);
HDF5_SCILAB_IMPEXP int readUnsignedInteger32Matrix(int _iDatasetId, unsigned int* _puiData);
HDF5_SCILAB_IMPEXP int readUnsignedInteger64Matrix(int _iDatasetId, unsigned long long* _pullData);

HDF5_SCILAB_IMPEXP int readSparseComplexMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal, double *_pdblImg);
HDF5_SCILAB_IMPEXP int readSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal);

HDF5_SCILAB_IMPEXP int readBooleanSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos);

HDF5_SCILAB_IMPEXP int getListItemReferences(int _iDatasetId, hobj_ref_t** _piItemRef);

HDF5_SCILAB_IMPEXP int getListItemDataset(int _iDatasetId, void* _piItemRef, int _iItemPos, int* _piItemDataset);

HDF5_SCILAB_IMPEXP int deleteListItemReferences(int _iDatasetId, void* _piItemRef);

HDF5_SCILAB_IMPEXP  void reset_item_count();

//Scilab 6
HDF5_SCILAB_IMPEXP int getVariableNames6(int _iFile, char **names);
HDF5_SCILAB_IMPEXP char* getScilabTypeFromDataSet6(int dataset);
HDF5_SCILAB_IMPEXP int getListDims6(int dataset, int* items);

#endif /* !__H5_READDATATOFILE_H__ */
