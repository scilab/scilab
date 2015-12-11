
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __COMMON_UMFPACK_H__
#define __COMMON_UMFPACK_H__

#include "scisparse.h"

/**
* Comments ???
* @param[in]
* @param[in]
* @param[in,out]
* @return
*/
int AddAdrToList(Adr adr, int it_flag, CellAdr **L);

/**
* Comments ???
* @param[in]
* @param[in,out]
* @param[in,out]
* @return
*/
int RetrieveAdrFromList(Adr adr, CellAdr **L, int *it_flag);

/**
* Comments ???
* @param[in]
* @param[in,out]
* @param[in,out]
* @return
*/
int IsAdrInList(Adr adr, CellAdr *L, int *it_flag);

/**
* Comments ???
* @param[in]
* @param[in]
* @param[in]
* @param[in]
*/
void TransposeMatrix(double A[], int ma, int na, double At[]);

/**
* Comments ???
* @param[in]
* @param[in,out]
* @param[in,out]
* @return
*/
int SciSparseToCcsSparse(SciSparse *A, CcsSparse *B);

/**
* Comments ???
* @param[in]
* @return
*/
char *UmfErrorMes(int num_error);

/**
* Comments ???
* @param[in,out]
* @return
*/
int is_sparse_upper_triangular(SciSparse *A);

/**
* Comments ???
* @param[in]
* @param[in,out]
* @param[in,out]
* @return
*/
int spd_sci_sparse_to_taucs_sparse(SciSparse *A, taucs_ccs_matrix *B);

/**
* Comments ???
* @param[in,out]
* @param[in]
* @param[in]
* @param[in]
* @param[in,out]
* @return
*/
void residu_with_prec(SciSparse *A, double x[], double b[], double r[], double *rn);

/**
* Comments ???
* @param[in,out]
* @param[in]
* @param[in]
* @param[in]
* @param[in,out]
* @param[in]
* @param[in]
* @return
*/
void residu_with_prec_for_chol(SciSparse *A, double x[], double b[], double r[],
                               double *rn, int A_is_upper_triangular, long double wk[]);

/**
* Comments ???
* @param[in,out]
* @param[in,out]
* @param[in,out]
* @param[in,out]
* @param[in,out]
* @param[in,out]
* @param[in,out]
* @return
*/
void cmplx_residu_with_prec(SciSparse *A,
                            double xr[], double xi[],
                            double br[], double bi[],
                            double rr[], double ri[],
                            double *rn);

/**
* Free array allocated in SciSparseToCcsSparse()
* @param[in] : CcsSparse sparse filled by SciSparseToCcsSparse()
* @return
*/
void freeCcsSparse(CcsSparse _Sp);

/**
* Free array allocated in spd_sci_sparse_to_taucs_sparse()
* @param[in] : CcsSparse sparse filled by spd_sci_sparse_to_taucs_sparse()
* @return
*/
void freeTaucsSparse(taucs_ccs_matrix _Sp);

#endif /* __COMMON_UMFPACK_H__ */
