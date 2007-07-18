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

/**
* TO DO : comments
*/
void sortTemplate( void);

/**
* TO DO : comments
*/
void lgsortd(double *,int *,int ,int ,char *,char *);

/**
* TO DO : comments
*/
void lgsorts1(char *,int *,int ,int ,char *,char *);

/**
* TO DO : comments
*/
void rowcolsortd(double *, int *, int , int , char *, char *);

/**
* TO DO : comments
*/
void wholesortd(double *, int *, int , int , char *, char *);

/**
* TO DO : comments
*/
void wholesorts(char *, int *, int , int , char *, char *);

/**
* TO DO : comments
*/
void rowcolsorts(char *, int *, int , int , char *, char *);
#ifdef __cplusplus
}
#endif
#endif /* _SORT_TEMPLATE_H_  */
