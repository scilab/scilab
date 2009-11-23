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

#include "GetMatlabVariable.h"

matvar_t *GetSparseVariable(int stkPos, const char *name)
{
  int K = 0, L = 0, Index = 0;
  int rank = 0;
  int *dims = NULL;
  double *data = NULL;
    
  matvar_t *createdVar = NULL;
  sparse_t *sparseData = NULL;
  SciSparse scilabSparse;
  int *colIndexes = NULL;
  int *rowIndexes = NULL;

  if (VarType(stkPos) == sci_sparse)
    {

      sparseData = (sparse_t*) MALLOC(sizeof(sparse_t));
      if (sparseData==NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
          return FALSE;
        }
      
      rank = 2;
      if ((dims = (int*) MALLOC (sizeof(int) * rank)) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
          return NULL;
        }

      GetRhsVar(stkPos, SPARSE_MATRIX_DATATYPE, &dims[1], &dims[0], (int*) &scilabSparse);

      /* colIndexes = (int*) MALLOC(sizeof(int) *  (scilabSparse.nel + 1));  */
      colIndexes = (int*) MALLOC(sizeof(int) *  (scilabSparse.m + 1)); 
      if (colIndexes==NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
          return FALSE;
        }
      
      colIndexes[0] = 0;
      /* for (K=0; K<scilabSparse.nel; K++) */
      for (K=0; K<scilabSparse.m; K++)
        {
          colIndexes[K+1] = colIndexes[K] + scilabSparse.mnel[K];
        }
 
      rowIndexes = (int*) MALLOC(sizeof(int) *  scilabSparse.nel);
      if (rowIndexes==NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
          return FALSE;
        }
      
      for (K=0; K<scilabSparse.nel; K++)
        {
          rowIndexes[K] = scilabSparse.icol[K] - 1;
        }

      if (scilabSparse.it==0) /* Real sparse */
        {
          /* for(K = 0; K < scilabSparse.nel; K++) */
          /*   { */
          /*     data[K] = scilabSparse.R[K]; */
          /*   } */
	  data = scilabSparse.R;
        }
      else
        {
          if((data = (double*) MALLOC(2 * sizeof(double) * scilabSparse.nel)) == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
              return FALSE;
            }

          for(K = 0; K < scilabSparse.nel; K++)
            {
              data[K] = scilabSparse.R[K];
            }

          for(K = 0; K < scilabSparse.nel; K++)
            {
              data[K + scilabSparse.nel] = scilabSparse.I[K];
            }
        }

      /* Create Matlab Sparse matrix data */
      sparseData->nzmax = scilabSparse.nel;
      sparseData->nir   = scilabSparse.nel;
      sparseData->ir    = rowIndexes;
      /* sparseData->njc   = scilabSparse.nel + 1; */
      sparseData->njc   = scilabSparse.m + 1;
      sparseData->jc    = colIndexes;
      sparseData->ndata = scilabSparse.nel;
      sparseData->data  = (void*) data;

      if (scilabSparse.it == 0)
        {
          createdVar = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, rank, dims, sparseData, 0 | MEM_CONSERVE);
        }
      else
        {
          createdVar = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, rank, dims, sparseData, MAT_F_COMPLEX | MEM_CONSERVE);
	  if (data) FREE(data);
        }

      if (dims) FREE(dims);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Sparse matrix expected.\n"), "GetSparseVariable");
    }
  
  return createdVar;
}
