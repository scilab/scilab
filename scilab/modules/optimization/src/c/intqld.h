/*
**  -*- C -*-
**
** intqld.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Mon Oct 29 17:38:05 2007 bruno
** Last update Mon Oct 29 17:48:25 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __INTQLD_H__
#define __INTQLD_H__

#include <string.h>
#include "stack-c.h"
#include "Scierror.h"

extern double C2F(dlamch)  __PARAMS((char *CMACH, unsigned long int));

extern int C2F(ql0001)(int *m,int *me,int *mmax,int *n,int *nmax,int *mnn,
            double *c,double *d,double *a,double *b,double *xl,
            double *xu,double *x,double *u,int *iout,int *ifail,
            int *iprint,double *war,int *lwar,int *iwar,int *liwar,
            double *eps1);

int C2F(intqld)(char *fname);

#endif /* !__INTQLD_H__ */
