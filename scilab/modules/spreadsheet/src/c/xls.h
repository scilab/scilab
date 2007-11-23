/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/ 
#ifndef __XLS_H__
#define __XLS_H__

/**
* xls_read
*
*/
void xls_read(int *fd, int *cur_pos,double **data, int **chainesind, int *N, int *M, int *err);

/**
* xls_open
*
*/
void xls_open(int *err, int *fd, char ***sst, int *ns, char ***Sheetnames, int** Abspos,int *nsheets);

#endif /* __XLS_H__ */
/*--------------------------------------------------------------------------*/ 
