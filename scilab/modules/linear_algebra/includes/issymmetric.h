/*
**  -*- C -*-
**
** issymmetric.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Sep 13 09:48:03 2007 bruno
** Last update Thu Sep 13 10:05:59 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __ISSYMMETRIC__
#define __ISSYMMETRIC__

#include "stack-c.h"

#define SYMMETRIC	1
#define NOT_SYMMETRIC	0

/*
** Check wether or not a Matrix is Symmetric.
*/

int C2F(issymmetric)(int *stackPosition);

#endif /* !__ISSYMMETRIC__ */
