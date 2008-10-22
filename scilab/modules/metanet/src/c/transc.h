
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
/*
**  -*- C -*-
**
** transc.h
**
** Started on  Fri Oct 26 09:37:54 2007 bruno
** Last update Fri Oct 26 14:56:40 2007 bruno
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
