/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateMatlabVariable.h"

int CreateMatlabVariable(int stkPos, matvar_t *matVariable)
{
  int nbRow = 0, nbCol = 0;
  int valueAdr = 0;
  
  /* To be sure isComplex is 0 or 1 */
  matVariable->isComplex =  matVariable->isComplex != 0;

  switch(matVariable->class_type)
    {
    case MAT_C_CELL: /* 1 */
      CreateCellVariable(stkPos, matVariable);
      break;
    case MAT_C_STRUCT: /* 2 */
      CreateStructVariable(stkPos, matVariable);
      break;
    case MAT_C_CHAR: /* 4 */
      CreateCharVariable(stkPos, matVariable);
      break;
    case MAT_C_SPARSE: /* 5 */
      CreateSparseVariable(stkPos, matVariable);
      break;
    case MAT_C_DOUBLE: /* 6 */
    case MAT_C_SINGLE: /* 7 */
      CreateDoubleVariable(stkPos, matVariable);
      break;
    case MAT_C_INT8: /* 8 */
      CreateIntegerVariable(stkPos, I_CHAR, matVariable);
      break;
    case MAT_C_UINT8: /* 9 */
      CreateIntegerVariable(stkPos, I_UCHAR, matVariable);
      break;
    case MAT_C_INT16: /* 10 */
      CreateIntegerVariable(stkPos, I_INT16, matVariable);
      break;
    case MAT_C_UINT16: /* 11 */
      CreateIntegerVariable(stkPos, I_UINT16, matVariable);
      break;
    case MAT_C_INT32: /* 12 */
      CreateIntegerVariable(stkPos, I_INT32, matVariable);
      break;
    case MAT_C_UINT32: /* 13 */
      CreateIntegerVariable(stkPos, I_UINT32, matVariable);
      break;
    case MAT_C_OBJECT: /* 3 to be written */
    case MAT_C_INT64: /* 14: no Scilab equivalent */
    case MAT_C_UINT64: /* 15: no Scilab equivalent */
    case MAT_C_FUNCTION: /* 16 to be written */
    default:
      /* Empty matrix returned */
      nbRow = 0; nbCol = 0;
      CreateVar(stkPos, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &valueAdr);
      return FALSE;
    }

  return TRUE;
}
