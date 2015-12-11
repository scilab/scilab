/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2012 - 2012 - Scialb Enterprises - Bruno JOFRET
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
