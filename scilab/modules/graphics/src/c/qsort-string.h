/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/
#ifndef __QSORT_STRING_H__
#define __QSORT_STRING_H__

void ColSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
void RowSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
void LexiRowstring(char * *a,int *ind,int flag,int n,int p,char dir);
void LexiColstring(char * *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_STRING_H__ */
