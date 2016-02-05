
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
#include "dynlib_linear_algebra.h"



#define SYMMETRIC	1
#define NOT_SYMMETRIC	0

/*
** Check whether or not a Matrix is Symmetric.
*/

LINEAR_ALGEBRA_IMPEXP int isSymmetric(double* _pdblReal, double* _pdblImg, int _iIsComplex, int _iRows, int _iCols);

#endif /* !__ISSYMMETRIC__ */
