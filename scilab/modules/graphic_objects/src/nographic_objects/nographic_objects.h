/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Allan CORNET
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
