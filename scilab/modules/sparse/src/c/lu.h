/*
**  -*- C -*-
**
** lu.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Tue Nov  6 14:01:51 2007 bruno
** Last update Tue Nov  6 14:30:11 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __LU_H__
#define __LU_H__

#define spINSIDE_SPARSE
#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"

#include "cerro.h"
#include "machine.h"
#include "localization.h"

void C2F(lufact1)(double *val, int *lln, int *col, int *n, int *nel,
		  long *fmat, double *eps, double *releps, int *nrank, int *ierr);

void C2F(lusolve1)(long *fmat, double *b, double *x);

void C2F(ludel1)(long *fmat);

void C2F(lusiz1)(long* fmat, int* lsize, int* usize);

void C2F(luget1)(long *fmat, int *indP, double *P,
		 int *indl, double *l, int *indu, double *u,
		 int *indQ, double *Q);
#endif /* !__LU_H__ */
