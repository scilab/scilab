/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enteprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
