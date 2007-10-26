/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/
#ifndef __QSORT_INT__
#define __QSORT_INT__

void ColSortint(int *a,int *ind,int flag,int n,int p,char dir);
void ColSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void RowSortint(int *a,int *ind,int flag,int n,int p,char dir);
void RowSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortint(int *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void LexiRowint(int *a,int *ind,int flag,int n,int p,char dir);
void LexiRowuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void LexiColint(int *a,int *ind,int flag,int n,int p,char dir);
void LexiColuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_INT__ */
