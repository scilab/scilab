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

/* Defined in SCI/modules/sparse/src/fortran/spt.f */
extern int C2F(spt)(int *m, int *n, int *nel, int *it, int *workArray, 
             double *A_R, double *A_I, int *A_mnel, int *A_icol,
             double *At_R, double *At_I, int *At_mnel, int *At_icol);

int CreateSparseVariable(int stkPos, matvar_t *matVariable)
{
  int K = 0;
  
  sparse_t *sparseData = NULL;
  SciSparse *scilabSparse = NULL;
  SciSparse *scilabSparseT = NULL; /* Transpose */
  int *colIndexes = NULL;
  int *rowIndexes = NULL;
  int *workArray = NULL;
  struct ComplexSplit *complexData = NULL;
  
  sparseData = (sparse_t*) matVariable->data;

  scilabSparse = (SciSparse*) MALLOC(sizeof(SciSparse));
  if (scilabSparse==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };

  /* Computes column indexes from Matlab indexes */
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

  /* Computes row indexes from Matlab indexes */
  rowIndexes = (int*) MALLOC(sizeof(int) * sparseData->nir);
  if (rowIndexes==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    }

  for (K=0; K<sparseData->nir; K++)
    {
      rowIndexes[K] = sparseData->ir[K] + 1;
    }

  /* Create sparse matrix to be transposed */
  scilabSparse->m = matVariable->dims[1];
  scilabSparse->n = matVariable->dims[0];
  scilabSparse->it = matVariable->isComplex;
  scilabSparse->nel = sparseData->ndata;
  scilabSparse->mnel = colIndexes;
  scilabSparse->icol = rowIndexes;
  if (scilabSparse->it == 0)
    {
      scilabSparse->R = (double*) sparseData->data;
      scilabSparse->I = NULL;
    }
  else
    {
      complexData = (struct ComplexSplit *) sparseData->data;
      scilabSparse->R = (double *) complexData->Re;
      scilabSparse->I = (double *) complexData->Im;
    }

  /* Create transpose */
  scilabSparseT = (SciSparse*) MALLOC(sizeof(SciSparse));
  if (scilabSparseT==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };
  scilabSparseT->m = scilabSparse->n;
  scilabSparseT->n = scilabSparse->m;
  scilabSparseT->it = scilabSparse->it;
  scilabSparseT->nel =  scilabSparse->nel;
  workArray = (int*) MALLOC(sizeof(int) * scilabSparseT->n);
  if (workArray==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    }
  scilabSparseT->mnel = (int*) MALLOC(sizeof(int) * scilabSparseT->m);
  if (scilabSparseT->mnel==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };
  scilabSparseT->icol = (int*) MALLOC(sizeof(int) * scilabSparseT->nel);
  if (scilabSparseT->icol==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };
  scilabSparseT->R = (double*) MALLOC(sizeof(double) * scilabSparseT->nel);
  if (scilabSparseT->R==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
      return FALSE;
    };
  if (scilabSparseT->it)
    {
      scilabSparseT->I = (double*) MALLOC(sizeof(double) * scilabSparseT->nel);
      if (scilabSparseT->I==NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "CreateSparseVariable");
          return FALSE;
        };
    }

  C2F(spt)(&scilabSparse->m, &scilabSparse->n, &scilabSparse->nel, &scilabSparse->it, workArray, 
             scilabSparse->R, scilabSparse->I, scilabSparse->mnel, scilabSparse->icol,
             scilabSparseT->R, scilabSparseT->I, scilabSparseT->mnel, scilabSparseT->icol);

  CreateVarFromPtr(stkPos, SPARSE_MATRIX_DATATYPE, &scilabSparseT->m, &scilabSparseT->n, scilabSparseT);

  /* Free all arrays */
  FREE(scilabSparse);
  FREE(colIndexes);
  FREE(rowIndexes);
  FREE(workArray);
  FREE(scilabSparseT->mnel);
  FREE(scilabSparseT->icol);
  FREE(scilabSparseT->R);
  if (scilabSparseT->it)
    {
      FREE(scilabSparseT->I);
    }
  FREE(scilabSparseT);
  
  return TRUE;
}
