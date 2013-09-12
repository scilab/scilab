/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#ifndef __QSORT_DOUBLE_H__
#define __QSORT_DOUBLE_H__

void ColSortdouble(double *a, int *ind, int flag, int n, int p, char dir);
void RowSortdouble(double *a, int *ind, int flag, int n, int p, char dir);
void GlobalSortdouble(double *a, int *ind, int flag, int n, int p, char dir);
void LexiRowdouble(double *a, int *ind, int flag, int n, int p, char dir);
void LexiColdouble(double *a, int *ind, int flag, int n, int p, char dir);

#endif /* __QSORT_DOUBLE_H__ */
