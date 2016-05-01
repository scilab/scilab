
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
