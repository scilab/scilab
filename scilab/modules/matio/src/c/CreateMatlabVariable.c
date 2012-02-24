/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateMatlabVariable.h"
#include "api_scilab.h"

int CreateMatlabVariable(int iVar, matvar_t *matVariable, int * parent, int item_position)
{
  SciErr _SciErr;

  /* To be sure isComplex is 0 or 1 */
  matVariable->isComplex =  matVariable->isComplex != 0;

  switch(matVariable->class_type)
    {
    case MAT_C_CELL: /* 1 */
      CreateCellVariable(iVar, matVariable, parent, item_position);
      break;
    case MAT_C_STRUCT: /* 2 */
      CreateStructVariable(iVar, matVariable, parent, item_position);
      break;
    case MAT_C_CHAR: /* 4 */
      CreateCharVariable(iVar, matVariable, parent, item_position);
      break;
    case MAT_C_SPARSE: /* 5 */
      CreateSparseVariable(iVar, matVariable, parent, item_position);
      break;
    case MAT_C_DOUBLE: /* 6 */
    case MAT_C_SINGLE: /* 7 */
      CreateDoubleVariable(iVar, matVariable, parent, item_position);
      break;
    case MAT_C_INT8: /* 8 */
      CreateIntegerVariable(iVar, I_CHAR, matVariable, parent, item_position);
      break;
    case MAT_C_UINT8: /* 9 */
      if (matVariable->isLogical != 0)
        {
          CreateBooleanVariable(iVar, matVariable, parent, item_position);
        }
      else
        {
          CreateIntegerVariable(iVar, I_UCHAR, matVariable, parent, item_position);
        }
      break;
    case MAT_C_INT16: /* 10 */
      CreateIntegerVariable(iVar, I_INT16, matVariable, parent, item_position);
      break;
    case MAT_C_UINT16: /* 11 */
      CreateIntegerVariable(iVar, I_UINT16, matVariable, parent, item_position);
      break;
    case MAT_C_INT32: /* 12 */
      CreateIntegerVariable(iVar, I_INT32, matVariable, parent, item_position);
      break;
    case MAT_C_UINT32: /* 13 */
      CreateIntegerVariable(iVar, I_UINT32, matVariable, parent, item_position);
      break;
    case MAT_C_OBJECT: /* 3 to be written */
    case MAT_C_INT64: /* 14: no Scilab equivalent */
    case MAT_C_UINT64: /* 15: no Scilab equivalent */
    case MAT_C_FUNCTION: /* 16 to be written */
    default:
      /* Empty matrix returned */
      if (parent==NULL)
	{
	  createEmptyMatrix(pvApiCtx, iVar);
	}
      else
	{
	  _SciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, 0, 0, NULL);
	}
      return FALSE;
    }

  return TRUE;
}
