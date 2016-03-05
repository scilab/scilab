/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2012 - 2012 - Scialb Enterprises - Bruno JOFRET
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

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to Init or Re-Init the window
 *    (Figure and/or Subwin) to the default graphics properties.
 --------------------------------------------------------------------------*/



#ifndef __SCI_INIT_GRAPHICS__
#define __SCI_INIT_GRAPHICS__

#include "dynlib_graphics.h"
#include "machine.h" /* C2F */

#define NUMCOLORS_SCI 32

GRAPHICS_IMPEXP int C2F(graphicsmodels) (void);

GRAPHICS_IMPEXP int sciInitGraphicMode (int iObjUID);

GRAPHICS_IMPEXP int initLabel(int iParentObjUID) ; /* INITOBJECT */

GRAPHICS_IMPEXP void InitFigureModel(int iFiguremdlUID); /* INITOBJECT */

GRAPHICS_IMPEXP int InitAxesModel(void);   /* INITOBJECT */

#endif /* __SCI_INIT_GRAPHICS__ */
