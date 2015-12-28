/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enteprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifndef __CELL_API__
#define __CELL_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_cell.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createCellMatrix     API_PROTO(createCellMatrix)
#define scilab_createCellMatrix2d   API_PROTO(createCellMatrix2d)
#define scilab_getCellValue         API_PROTO(getCellValue)
#define scilab_getCell2dValue       API_PROTO(getCell2dValue)
#define scilab_setCellValue         API_PROTO(setCellValue)
#define scilab_setCell2dValue       API_PROTO(setCell2dValue)

scilabVar API_PROTO(createCellMatrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createCellMatrix2d)(scilabEnv env, int row, int col);

scilabStatus API_PROTO(getCellValue)(scilabEnv env, scilabVar var, int* index, scilabVar* val);
scilabStatus API_PROTO(getCell2dValue)(scilabEnv env, scilabVar var, int row, int col, scilabVar* val);
scilabStatus API_PROTO(setCellValue)(scilabEnv env, scilabVar var, int* index, scilabVar val);
scilabStatus API_PROTO(setCell2dValue)(scilabEnv env, scilabVar var, int row, int col, scilabVar val);
#ifdef __cplusplus
}
#endif
#endif /* __CELL_API__ */
