/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/
#ifndef __QSORT_SHORT_H__
#define __QSORT_SHORT_H__

void ColSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void RowSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortshort(short *a,int *ind,int flag,int n,int p,char dir);

void ColSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void ColSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

void RowSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

void LexiRowshort(short *a,int *ind,int flag,int n,int p,char dir);
void LexiRowushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

void LexiColshort(short *a,int *ind,int flag,int n,int p,char dir);
void LexiColushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_SHORT_H__ */
