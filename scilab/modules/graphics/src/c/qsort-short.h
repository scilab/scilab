/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/

void ColSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void RowSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortshort(short *a,int *ind,int flag,int n,int p,char dir);

void ColSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void ColSortushort(short *a,int *ind,int flag,int n,int p,char dir);

void RowSortushort(short *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortushort(short *a,int *ind,int flag,int n,int p,char dir);

void LexiRowshort(short *a,int *ind,int flag,int n,int p,char dir);
void LexiRowushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

void RowSortushort(short *a,int *ind,int flag,int n,int p,char dir);

void LexiColshort(short *a,int *ind,int flag,int n,int p,char dir);
void LexiColushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);
