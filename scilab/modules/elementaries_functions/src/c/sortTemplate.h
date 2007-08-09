/*------------------------------------------------------------------------*/
/* file: sortTemplate.h                                                   */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc :  The .h file for the gsort                                      */
/*------------------------------------------------------------------------*/

#ifndef _SORT_TEMPLATE_H_
#define _SORT_TEMPLATE_H_
#ifdef __cplusplus
extern "C" {
#endif
void sortTemplate( void);
void lgsortd(double *,int *,int ,int ,char *,char *);
void lgsorts(char **,int *,int ,int ,char *,char *);
void rowcolsortd(double *, int *, int , int , char *, char *);
void wholesortd(double *, int *, int , int , char *, char *);
void wholesorts(char **, int *, int , int , char *, char *);
void mywhole_swapd(double *v,int i, int j, int n);
void my_qsortd(double *,int *, int , int ,char *,int ,int );
void my_qsortdcol(double *v,int *, int , int ,char *,int ,int );
void rowcolsorts(char **, int *, int , int , char *, char *);

#ifdef __cplusplus
}
#endif
#endif /* _SORT_TEMPLATE_H_  */
