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

int writeDoubleMatrix(int file, char* dataSetName, double *data, int rows, int cols);

int writeStringMatrix(int file, char* dataSetName, char **data, int rows, int cols);

int writeBooleanMatrix(int _iFile, char* _pstDatasetName, int *_piData, int _iRows, int _iCols);

int writePolyMatrix(int _iFile, char* _pstDatasetName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

void* openList(int _iFile, char* pstDatasetName, int _iNbItem);
int addItemInList(int _iFile, void* _pvList, int _iPos, char* _pstItemName);
int closeList(int _iFile,  void* _pvList, char* _pstListName, int _iNbItem, int _iVarType);

#endif /* !__H5_WRITEDATATOFILE_H__ */
