/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
enum {NOT_A_KNOT, NATURAL, CLAMPED, PERIODIC, FAST, FAST_PERIODIC,
MONOTONE, BY_ZERO, C0, LINEAR, BY_NAN, UNDEFINED};
/*--------------------------------------------------------------------------*/
typedef struct { char *str_type; int type; } TableType;
/*--------------------------------------------------------------------------*/
typedef struct realhypermat
{
	int dimsize;        /* number of dimensions of the hyper matrix */
	int size;           /* total number of elements : size = dims[0]x... x dims[dimsize-1] */
	int *dims;          /* number of elements in each dimension */
	double *R;          /* points to the elements  */
} RealHyperMat;
/*--------------------------------------------------------------------------*/
extern int get_rhs_scalar_string(int num, int *length, int **tabchar);
#define GetRhsScalarString(num,n,t) if (!get_rhs_scalar_string(num,n,t)) { return 0;}
/*--------------------------------------------------------------------------*/
extern int get_rhs_real_hmat(int num, RealHyperMat *H);
#define GetRhsRealHMat(pos,H) if (!get_rhs_real_hmat(pos,H)) { return 0;}
/*--------------------------------------------------------------------------*/

/**
 * TODO : comment
 * @param x
 * @param n
 * @return
 */
int good_order(double x[], int n);

/**
 * TODO : comment
 * @param Tab
 * @param dim_table
 * @param scistr
 * @param strlength
 * @return
 */
int get_type(TableType *Tab, int dim_table, int *scistr, int strlength);


