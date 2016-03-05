/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
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

#ifndef __SPBUILD_H__
#define __SPBUILD_H__

#include "spDefs.h"
/* TODO: move comments from spBuild.c */

ElementPtr spcFindElementInCol( MatrixPtr Matrix, register ElementPtr *LastAddr, register int Row, int Col, SPBOOLEAN CreateIfMissing );

ElementPtr spcCreateElement( MatrixPtr Matrix, int Row, register int Col, register ElementPtr  * LastAddr, SPBOOLEAN Fillin );

int spcLinkRows( MatrixPtr Matrix );

#endif /* __SPBUILD_H__ */
