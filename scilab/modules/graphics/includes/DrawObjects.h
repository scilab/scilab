/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 --------------------------------------------------------------------------*/

#ifndef __SCI_DRAWINGS__
#define __SCI_DRAWINGS__

#include "dynlib_graphics.h"

GRAPHICS_IMPEXP void sciGetDisplayedBounds(int iSubWinUID,
        double      * xmin   ,
        double      * xmax   ,
        double      * ymin   ,
        double      * ymax   ,
        double      * zmin   ,
        double      * zmax);

#endif /* __SCI_DRAWINGS__ */
