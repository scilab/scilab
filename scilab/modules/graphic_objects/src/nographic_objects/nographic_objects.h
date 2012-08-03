/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "dynlib_graphic_objects.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
/**
 * This function is a wrapper / fake when Scilab is compiled without xpad
 *
 * @return 0
 */
GRAPHIC_OBJECTS_IMPEXP int gw_graphic_objects(void);

GRAPHIC_OBJECTS_IMPEXP BOOL sciHasFigures(void);

GRAPHIC_OBJECTS_IMPEXP BOOL sciIsExistingFigure(int figNum);
/*--------------------------------------------------------------------------*/
