/*
**  -*- C -*-
**
** transc.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Fri Oct 26 09:37:54 2007 bruno
** Last update Fri Oct 26 14:56:40 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __TRANSC_H__
#define __TRANSC_H__

#include <string.h>
#include <stdlib.h>

#include "machine.h"
#include "MALLOC.h"

#include "cerro.h"

extern int F2C(frmtrs) ();

void C2F(transc)(int *lp1,int **lpft,int *ls1,int **lsft,int *m,int *lsftdim,int *lpftdim,int *n);

#endif /* !__TRANSC_H__ */
