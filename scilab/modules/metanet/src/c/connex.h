/*
**  -*- C -*-
**
** connex.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Oct 25 15:30:18 2007 bruno
** Last update Fri Oct 26 15:13:13 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __CONNEX_H__
#define __CONNEX_H__

#include <string.h>
#include <stdlib.h>

#include "MALLOC.h"

#include "cerro.h"

#include "machine.h"

void C2F(concom)(int *icomp, int *n, int *ncomp, int *nsize, int **ns);

#endif /* ! __CONNEX_H__ */
