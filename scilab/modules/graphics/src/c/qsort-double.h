/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/
#ifndef __QSORT_DOUBLE_H__
#define __QSORT_DOUBLE_H__

void ColSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
void RowSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
void LexiRowdouble(double *a,int *ind,int flag,int n,int p,char dir);
void LexiColdouble(double *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_DOUBLE_H__ */
