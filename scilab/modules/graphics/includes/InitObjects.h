/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "ObjectStructure.h"
#include "BOOL.h"
#include "machine.h" /* C2F */

GRAPHICS_IMPEXP int C2F(graphicsmodels) (void);
GRAPHICS_IMPEXP int sciInitGraphicContext (sciPointObj * pobj); /* INITOBJECT */
GRAPHICS_IMPEXP int sciInitFontContext (sciPointObj * pobj); /* INITOBJECT */
GRAPHICS_IMPEXP int sciInitGraphicMode (sciPointObj * pobj);

GRAPHICS_IMPEXP sciPointObj * initLabel( sciPointObj * pParentObj ) ; /* INITOBJECT */

GRAPHICS_IMPEXP void reinitSubwin( void ) ;

GRAPHICS_IMPEXP FigureModelData * newFigureModelData( void ) ;
GRAPHICS_IMPEXP void destroyFigureModelData( FigureModelData * data ) ;

GRAPHICS_IMPEXP int InitFigureModel(void); /* INITOBJECT */
GRAPHICS_IMPEXP int InitAxesModel(void);   /* INITOBJECT */

GRAPHICS_IMPEXP int initFCfromCopy(  sciPointObj * pObjSource, sciPointObj * pObjDest );

GRAPHICS_IMPEXP int ResetFigureToDefaultValues(sciPointObj * pobj);  /* INITOBJECT */

GRAPHICS_IMPEXP sciPointObj * getFigureModel( void ) ;
GRAPHICS_IMPEXP sciPointObj * getAxesModel( void ) ;
GRAPHICS_IMPEXP BOOL isFigureModel(sciPointObj * pObj);
GRAPHICS_IMPEXP BOOL isAxesModel(sciPointObj * pObj);
GRAPHICS_IMPEXP BOOL isModelObject(sciPointObj * pObj);

GRAPHICS_IMPEXP void destroyDefaultObjects( void ) ;

GRAPHICS_IMPEXP void sciSetDefaultColorMap(sciPointObj * pFigure);

#endif /* __SCI_INIT_GRAPHICS__ */
