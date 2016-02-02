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

#ifndef __H5_READDATATOFILE_V1_H__
#define __H5_READDATATOFILE_V1_H__

#include <hdf5.h>
#include "dynlib_hdf5_scilab.h"


#define unknow_type     -2
#define sci_undefined   -1
#define sci_void        0

HDF5_SCILAB_IMPEXP int isComplexData_v1(int _iDatasetId);

HDF5_SCILAB_IMPEXP int getVariableNames_v1(int _iFile, char **pstNameList);

HDF5_SCILAB_IMPEXP int getDataSetId_v1(int  _iFile);

HDF5_SCILAB_IMPEXP int getDataSetIdFromName_v1(int _iFile, char *_pstName);
HDF5_SCILAB_IMPEXP void closeDataSet_v1(int _id);

HDF5_SCILAB_IMPEXP int getDatasetDims_v1(int _iDatasetId, int *_piRows, int *_piCols);
HDF5_SCILAB_IMPEXP int getListDims_v1(int _iDatasetId, int *_piItem);

HDF5_SCILAB_IMPEXP int getScilabTypeFromDataSet_v1(int _iDatasetId);

HDF5_SCILAB_IMPEXP int getDatasetPrecision_v1(int _iDatasetId, int* _piPrec);

HDF5_SCILAB_IMPEXP int getSparseDimension_v1(int _iDatasetId, int* _piRows, int * _piCols, int* _piNbItem);

HDF5_SCILAB_IMPEXP int readEmptyMatrix_v1(int _iDatasetId);

HDF5_SCILAB_IMPEXP int readDoubleMatrix_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblData);
HDF5_SCILAB_IMPEXP int readDoubleComplexMatrix_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg);

HDF5_SCILAB_IMPEXP int readStringMatrix_v1(int _iDatasetId, int _iRows, int _iCols, char **_pstData);

HDF5_SCILAB_IMPEXP int readBooleanMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int* _piData);

HDF5_SCILAB_IMPEXP int readPolyMatrix_v1(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblData);
HDF5_SCILAB_IMPEXP int readPolyComplexMatrix_v1(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblReal, double **_pdblImg);

HDF5_SCILAB_IMPEXP int readInteger8Matrix_v1(int _iDatasetId, int _iRows, int _iCols, char* _pcData);
HDF5_SCILAB_IMPEXP int readInteger16Matrix_v1(int _iDatasetId, int _iRows, int _iCols, short* _psData);
HDF5_SCILAB_IMPEXP int readInteger32Matrix_v1(int _iDatasetId, int _iRows, int _iCols, int* _piData);
HDF5_SCILAB_IMPEXP int readInteger64Matrix_v1(int _iDatasetId, int _iRows, int _iCols, long long* _pllData);

HDF5_SCILAB_IMPEXP int readUnsignedInteger8Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned char* _pucData);
HDF5_SCILAB_IMPEXP int readUnsignedInteger16Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned short* _pusData);
HDF5_SCILAB_IMPEXP int readUnsignedInteger32Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned int* _puiData);
HDF5_SCILAB_IMPEXP int readUnsignedInteger64Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned long long* _pullData);

HDF5_SCILAB_IMPEXP int readSparseComplexMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal, double *_pdblImg);
HDF5_SCILAB_IMPEXP int readSparseMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal);

HDF5_SCILAB_IMPEXP int readBooleanSparseMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos);

HDF5_SCILAB_IMPEXP int getListItemReferences_v1(int _iDatasetId, hobj_ref_t** _piItemRef);

HDF5_SCILAB_IMPEXP int getListItemDataset_v1(int _iDatasetId, void* _piItemRef, int _iItemPos, int* _piItemDataset);

HDF5_SCILAB_IMPEXP int deleteListItemReferences_v1(int _iDatasetId, void* _piItemRef);

HDF5_SCILAB_IMPEXP  void reset_item_count();

#endif /* !__H5_READDATATOFILE_V1_H__ */
