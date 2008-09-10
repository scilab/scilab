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

#include "ObjectStructure.h"
#include "HandleManagement.h"


#ifndef __SCI_INIT_GRAPHICS__
#define __SCI_INIT_GRAPHICS__

extern int C2F(graphicsmodels) (void);
extern int sciInitGraphicContext (sciPointObj * pobj); /* INITOBJECT */
extern int sciInitFontContext (sciPointObj * pobj); /* INITOBJECT */
extern int sciInitGraphicMode (sciPointObj * pobj);

extern sciPointObj * initLabel( sciPointObj * pParentObj ) ; /* INITOBJECT */

extern void initsubwin(void); /* INITOBJECT */
extern void reinitSubwin( void ) ;

FigureModelData * newFigureModelData( void ) ;
void destroyFigureModelData( FigureModelData * data ) ;

extern int InitFigureModel(void); /* INITOBJECT */
extern int InitAxesModel(void);   /* INITOBJECT */

int initFCfromCopy(  sciPointObj * pObjSource, sciPointObj * pObjDest );

int ResetFigureToDefaultValues(sciPointObj * pobj);  /* INITOBJECT */

sciPointObj * getFigureModel( void ) ;
sciPointObj * getAxesModel( void ) ;
BOOL isFigureModel(sciPointObj * pObj);
BOOL isAxesModel(sciPointObj * pObj);

void destroyDefaultObjects( void ) ;

void sciSetDefaultColorMap(sciPointObj * pFigure);

char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads);

#endif /* __SCI_INIT_GRAPHICS__ */
