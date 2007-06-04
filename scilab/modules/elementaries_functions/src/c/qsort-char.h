/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/
#ifndef __QSORT_CHAR_H__
#define __QSORT_CHAR_H__

void ColSortchar(char *a,int *ind,int flag,int n,int p,char dir);
void RowSortchar(char *a,int *ind,int flag,int n,int p,char dir);

void GlobalSortchar(char *a,int *ind,int flag,int n,int p,char dir);

void ColSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
void RowSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);

void GlobalSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);


void LexiColuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
void LexiRowchar(char *a,int *ind,int flag,int n,int p,char dir);
void LexiRowuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
void LexiColchar(char *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_CHAR_H__ */
