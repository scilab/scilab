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

int getDataSetId(int  _iFile);

int getDataSetDims(int _iDataSetId, int *_piRows, int *_piCols);

int getScilabTypeFromDataSet(int _iDataSetId);

int readDoubleMatrix(int _iDataSetId, double *_pdblData, int _iRows, int _iCols);

int readStringMatrix(int _iDataSetId, char **_pstData, int _iRows, int _iCols);

int getListItemReferences(int _iDatasetId, void** _piItemRef);

int getListItemDataset(int _iDatasetId, void* _piItemRef, int _iItemPos, int* _piItemDataset);

#endif /* !__H5_READDATATOFILE_H__ */
