/*
**  -*- C -*-
**
** rea2db.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Oct 11 13:41:16 2007 bruno
** Last update Thu Oct 11 13:42:19 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __READ2DB__
#define __READ2DB__

#include "machine.h"

/*-----------------------------------------------------------------------------------*/
/* cette subroutine traduit un vecteur dx, de taille n, sur un
   vecteur double precision dy.
   dans le cas de deux increments egaux a 1, cette fonction
   emploie des boucles "epanouies".
   dans le cas ou les increments sont negatifs cette
   fonction prend les composantes en ordre inverse.
*/
/*-----------------------------------------------------------------------------------*/
int C2F(rea2db)(int *n, float *dx, int *incx, double *dy, int *incy);

#endif /* !__READ2DB__ */
