
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
** connex.h
**
** Started on  Thu Oct 25 15:30:18 2007 bruno
** Last update Fri Oct 26 15:13:13 2007 bruno
*/

#ifndef __CONNEX_H__
#define __CONNEX_H__

#include <string.h>
#include <stdlib.h>

#include "MALLOC.h"

#include "cerro.h"

#include "machine.h"

void C2F(concom)(int *icomp, int *n, int *ncomp, int **ns, int *nsize);

#endif /* ! __CONNEX_H__ */
