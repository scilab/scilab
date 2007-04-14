/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/
#ifndef __FILES_H__
#define __FILES_H__

#include <stdio.h> /* FILE */
#include "machine.h" /* C2F() */

void C2F(getfiledesc)(integer *fd);

void C2F(addfile)(integer *fd, FILE *fa, integer *swap, integer *type, integer *mode, char *filename, integer *ierr);

void C2F(getfileinfo)(integer *fd, FILE *fa, integer *swap, integer *type, integer *mode, char *filename, integer *lf, integer *ierr);

void C2F(getfiletype)(integer *fd, integer *type, integer *ierr);

void C2F(delfile)(integer *fd);

int Status2Mode(char *status);

FILE *GetFile(integer *fd);

int GetSwap(integer *fd);

int islittle_endian(void);

int C2F(getendian)(void);

void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error);

void C2F(mclose) (integer *fd, double *res);

void C2F(meof) (integer *fd, double *res);

void C2F(merror) (integer *fd, double *res);

void C2F(mclearerr) (integer *fd);

void C2F(mseek) (integer *fd, integer *offset, char *flag, integer *err);

void C2F(mtell) (integer *fd, double *offset, integer *err);

void C2F(mputnc) (integer *fd, void * res, integer *n1, char *type, integer *ierr);

void mput2 (FILE *fa, integer swap2, double *res, integer n, char *type, integer *ierr);

void C2F(mput) (integer *fd, double *res, integer *n, char *type, integer *ierr);

void C2F(mgetnc)(integer *fd, void * res, integer *n1, char *type, integer *ierr);

void mget2(FILE *fa, integer swap, double *res, integer n, char *type, integer *ierr);

void C2F(mget) (integer *fd, double *res, integer *n, char *type, integer *ierr);

void C2F(mgetstr) (integer *fd, char **start, integer *n, integer *ierr);

void C2F(mgetstr1) (integer *fd, char *start, integer *n, integer *ierr);

void C2F(mputstr) (int *fd, char *str, double *res, int *ierr);

#endif /* __FILES_H__ */
