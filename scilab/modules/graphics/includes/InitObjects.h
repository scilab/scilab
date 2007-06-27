/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
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

void destroyDefaultObjects( void ) ;

#endif /* __SCI_INIT_GRAPHICS__ */
