
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
** issymmetric.h
**
** Started on  Thu Sep 13 09:48:03 2007 bruno
** Last update Thu Sep 13 10:05:59 2007 bruno
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
