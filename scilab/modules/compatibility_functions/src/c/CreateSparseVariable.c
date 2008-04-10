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

int CreateSparseVariable(int stkPos, matvar_t *matVariable)
{
  int K = 0;
  
  sparse_t *sparseData = NULL;
  SciSparse *scilabSparse = NULL;
  int *colIndexes = NULL;
  int *rowIndexes = NULL;
  
  sparseData = (sparse_t*) matVariable->data;

  scilabSparse = (SciSparse*) MALLOC(sizeof(SciSparse));
  if (scilabSparse==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };

  colIndexes = (int*) MALLOC(sizeof(int) *  (sparseData->njc-1));
  if (colIndexes==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };
      
  for (K=0; K<sparseData->njc-1; K++)
    {
      colIndexes[K] = sparseData->jc[K+1] - sparseData->jc[K];
    }
 
  rowIndexes = (int*) MALLOC(sizeof(int) *  sparseData->nir);
  if (rowIndexes==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };

  for (K=0; K<sparseData->nir; K++)
    {
      rowIndexes[K] = sparseData->ir[K] + 1;
    }

  scilabSparse->m = matVariable->dims[1];
  scilabSparse->n = matVariable->dims[0];
  scilabSparse->it = matVariable->isComplex;
  scilabSparse->nel = sparseData->ndata;
  scilabSparse->mnel = colIndexes;
  scilabSparse->icol = rowIndexes;
  scilabSparse->R = (double*) sparseData->data;
  if (scilabSparse->it)
    {
      scilabSparse->I = &(((double*) sparseData->data)[sparseData->ndata]);
    }
  else
    {
      scilabSparse->I = NULL;
    }
  CreateVarFromPtr(stkPos, SPARSE_MATRIX_DATATYPE, &scilabSparse->m, &scilabSparse->n, scilabSparse);

  return TRUE;
}
