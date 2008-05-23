/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
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
 *    This file contains all functions used to CLONE an object, it means make
 *    a copy of an object under the same subwin.
 --------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "CloneObjects.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "SetProperty.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

/**sciCloneColormap
 * This function clone a colormap from the figure. It must be FREE a lesat.
 * It's the same for all sons
 * Setting the colormap rgbmat must be a m x 3 double RGB matrix:
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE
 */
double *
sciCloneColormap (sciPointObj * pobj)
{
  double *rgbmat;
  int m = sciGetNumColors (pobj);

  if ((rgbmat = MALLOC (m * 3 * sizeof (double))) == NULL)
  {
    return NULL;
  }

  sciGetColormap( pobj, rgbmat ) ;

  return rgbmat;
}



/**CloneText
 * 
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
CloneText (sciPointObj * pthis)
{
  sciPointObj * pobj        ;
  sciPointObj * subwinparent;
  sciText     * ppThisText  ;
  sciText     * ppCopyText  ;
  int foreground = sciGetForeground(pthis);
  int background = sciGetBackground(pthis);
  int nbRow ;
  int nbCol ;
  double textPos[3];
 
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && ((int) sciGetEntityType(subwinparent) != -1));
  if ((int) sciGetEntityType(subwinparent) == -1)
  {
    return (sciPointObj *)NULL;
  }
  
  sciGetTextSize( pthis, &nbRow, &nbCol ) ;
  sciGetTextPos(pthis, textPos);
  if (!(pobj = ConstructText (subwinparent, getStrMatData( sciGetText(pthis) ), nbRow, nbCol, 
			      textPos[0], textPos[1], sciGetAutoSize(pthis),
                              pTEXT_FEATURE(pthis)->userSize,pTEXT_FEATURE(pthis)->centeredPos,
			      &foreground,&background,pTEXT_FEATURE(pthis)->isboxed,
			      sciGetIsLine(pthis), sciGetIsFilled(pthis), sciGetAlignment(pthis))))
  {
    return (sciPointObj *)NULL;
  }
  else
  {
    sciSetCurrentObj(pobj);
  } /* F.Leray Adding 26.03.04*/
  
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  
  if (sciSetFontSize(pobj, sciGetFontSize(pthis)) < 0.0)
    return (sciPointObj *)NULL;
  
  if (sciSetFontOrientation(pobj, sciGetFontOrientation (pthis)) == -1)
    return (sciPointObj *)NULL;
  
  if (sciSetFontStyle(pobj,sciGetFontStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  
  if (sciSetFontName(pobj, sciGetFontName (pthis), sciGetFontNameLength(pthis)) == -1)
    return (sciPointObj *)NULL;
  
  /* get the pointer on features */
  ppThisText = pTEXT_FEATURE( pthis ) ;
  ppCopyText = pTEXT_FEATURE( pobj  ) ;

  ppCopyText->userSize[0] = ppThisText->userSize[0];
  ppCopyText->userSize[1] = ppThisText->userSize[1];

  
  if((ppThisText->size_of_user_data != 0) && (ppThisText->user_data != (int *) NULL))
  {
    int size = ppThisText->size_of_user_data;
    
    if((ppCopyText->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
      sciprint(_("Can not allocate user_data for cloned object.\n"));
      ppCopyText->user_data = (int *) NULL;
      ppCopyText->size_of_user_data = 0;
    }
    else{
      memcpy(ppCopyText->user_data, ppThisText->user_data,  size);
      ppCopyText->size_of_user_data = pTEXT_FEATURE (pthis)->size_of_user_data;
    }
  }
  
  return (sciPointObj *)pobj;
}



/**sciCloneObj
 */
sciPointObj *
sciCloneObj (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return CloneText(pobj);
      break;
    case SCI_POLYLINE:
      return ClonePolyline(pobj);
      break;
    case SCI_ARC:
      return CloneArc(pobj);
      break;
    case SCI_RECTANGLE:
      return CloneRectangle(pobj);
      break;
    case SCI_CONSOLE:
      return sciCloneConsole(pobj) ;
    case SCI_FRAME:
      return sciCloneFrame(pobj) ;
    case SCI_WINDOW:
      return sciCloneWindow(pobj) ;
    case SCI_WINDOWFRAME:
      return sciCloneWindowFrame(pobj) ;
    case SCI_SCREEN:
      return sciCloneScreen(pobj) ;
    case SCI_AGREG:

    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_LEGEND:
    case SCI_TITLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      sciprint (_("This object cannot be cloned.\n"));
      return (sciPointObj *)NULL;
      break;
    }
}



/**CloneRectangle
 * This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
CloneRectangle (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  int foreground = sciGetForeground(pthis);
  int background = sciGetBackground(pthis);
 
  subwinparent = pthis;
  
  
  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && ((int)sciGetEntityType(subwinparent) != -1));
  if ((int)sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructRectangle (subwinparent, pRECTANGLE_FEATURE(pthis)->x, 
				   pRECTANGLE_FEATURE(pthis)->y, pRECTANGLE_FEATURE(pthis)->height,pRECTANGLE_FEATURE(pthis)->width, 
				   pRECTANGLE_FEATURE(pthis)->horzcurvature, pRECTANGLE_FEATURE(pthis)->vertcurvature,
				   &foreground,&background,sciGetIsFilled(pthis),sciGetIsLine(pthis),0,pRECTANGLE_FEATURE(pthis)->flagstring))){
    return (sciPointObj *)NULL;
  }
  else {
    sciSetCurrentObj(pobj);}; /* F.Leray Adding 26.03.04*/
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetIsFilled(pobj, sciGetIsFilled (pthis)) == -1)
    return (sciPointObj *)NULL;
  
  if((pRECTANGLE_FEATURE (pthis)->size_of_user_data != 0) && (pRECTANGLE_FEATURE (pthis)->user_data != (int *) NULL))
    {
      int size = pRECTANGLE_FEATURE (pthis)->size_of_user_data;
      
      if((pRECTANGLE_FEATURE (pobj)->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
		  sciprint(_("%s: No more memory.\n"),"CloneRectangle");
		  pRECTANGLE_FEATURE (pobj)->user_data = (int *) NULL;
		  pRECTANGLE_FEATURE (pobj)->size_of_user_data = 0;
      }
      else{
	memcpy(pRECTANGLE_FEATURE (pobj)->user_data, pRECTANGLE_FEATURE (pthis)->user_data,  size);
	pRECTANGLE_FEATURE (pobj)->size_of_user_data = pRECTANGLE_FEATURE (pthis)->size_of_user_data;
      }
    }
  
  return (sciPointObj *)pobj;
}


/**ClonePolyline
 * This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
ClonePolyline (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  int foreground = sciGetForeground(pthis);
  int background = sciGetBackground(pthis);
  int mark_foreground = sciGetMarkForeground(pthis);
  int mark_background = sciGetMarkBackground(pthis);
  int mark_style = sciGetMarkStyle(pthis);
  
  subwinparent = pthis;
  
  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && ((int)sciGetEntityType(subwinparent) != -1));
  if ((int)sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  /* DJ.A 2003 */
  if (!(pobj = ConstructPolyline (subwinparent, pPOLYLINE_FEATURE(pthis)->pvx, pPOLYLINE_FEATURE(pthis)->pvy,pPOLYLINE_FEATURE(pthis)->pvz,
				  pPOLYLINE_FEATURE(pthis)->closed, pPOLYLINE_FEATURE(pthis)->n1,pPOLYLINE_FEATURE(pthis)->plot,
				  &foreground, &background,
				  &mark_style, &mark_foreground, &mark_background,
				  sciGetIsLine(pthis),  sciGetIsFilled(pthis), 
				  sciGetIsMark(pthis),pPOLYLINE_FEATURE(pthis)->isinterpshaded))){
    return (sciPointObj *)NULL;
  }
  else {
    sciSetCurrentObj(pobj);}; /* F.Leray Adding 26.03.04*/
  
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;

  if((pPOLYLINE_FEATURE (pthis)->size_of_user_data != 0) && (pPOLYLINE_FEATURE (pthis)->user_data != (int *) NULL))
    {
      int size = pPOLYLINE_FEATURE (pthis)->size_of_user_data;
      
      if((pPOLYLINE_FEATURE (pobj)->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
		  sciprint(_("%s: No more memory.\n"),"ClonePolyline");
		  pPOLYLINE_FEATURE (pobj)->user_data = (int *) NULL;
		  pPOLYLINE_FEATURE (pobj)->size_of_user_data = 0;
      }
      else{
	memcpy(pPOLYLINE_FEATURE (pobj)->user_data, pPOLYLINE_FEATURE (pthis)->user_data,  size);
	pPOLYLINE_FEATURE (pobj)->size_of_user_data = pPOLYLINE_FEATURE (pthis)->size_of_user_data;
      }
    }
  
  return (sciPointObj *)pobj;
}


/**CloneArc
 * This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
CloneArc (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;

  int foreground = sciGetForeground(pthis);
  int background = sciGetBackground(pthis);

  subwinparent = pthis;
  
 
  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && ((int)sciGetEntityType(subwinparent) != -1));
  if ((int)sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructArc (subwinparent, pARC_FEATURE(pthis)->x, 
			     pARC_FEATURE(pthis)->y, pARC_FEATURE(pthis)->height,pARC_FEATURE(pthis)->width,
			     pARC_FEATURE(pthis)->alphabegin, pARC_FEATURE(pthis)->alphaend,
			     &foreground,&background,sciGetIsFilled(pthis),sciGetIsLine(pthis)))){
    return (sciPointObj *)NULL;
  }
  else {
    sciSetCurrentObj(pobj);}; /* F.Leray Adding 26.03.04*/
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetIsFilled(pobj, sciGetIsFilled (pthis)) == -1)
    return (sciPointObj *)NULL;

  if((pARC_FEATURE (pthis)->size_of_user_data != 0) && (pARC_FEATURE (pthis)->user_data != (int *) NULL))
    {
      int size = pARC_FEATURE (pthis)->size_of_user_data;
      
      if((pARC_FEATURE (pobj)->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
		  sciprint(_("%s: No more memory.\n"),"CloneArc");
		  pARC_FEATURE (pobj)->user_data = (int *) NULL;
		  pARC_FEATURE (pobj)->size_of_user_data = 0;
      }
      else{
	memcpy(pARC_FEATURE (pobj)->user_data, pARC_FEATURE (pthis)->user_data,  size);
	pARC_FEATURE (pobj)->size_of_user_data = pARC_FEATURE (pthis)->size_of_user_data;
      }
    }
 
  return (sciPointObj *)pobj;
}



/**sciCopyObj
 */
sciPointObj *
sciCopyObj (sciPointObj * pobj, sciPointObj * psubwinparenttarget )
{
  sciPointObj *pcopyobj;

  pcopyobj = sciCloneObj ((sciPointObj *)pobj);
  sciDelThisToItsParent ((sciPointObj *)pcopyobj, (sciPointObj *)sciGetParent(pcopyobj));
  sciAddThisToItsParent ((sciPointObj *)pcopyobj, (sciPointObj *)psubwinparenttarget);
  return (sciPointObj *)pcopyobj;
}

/*--------------------------------------------------------------------------*/
int cloneGraphicContext( sciPointObj * pObjSource, sciPointObj * pObjDest )
{
  /* struct affectation */
  *(sciGetGraphicContext(pObjDest)) = *(sciGetGraphicContext(pObjSource)) ;
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int cloneUserData( sciPointObj * pObjSource, sciPointObj * pObjDest )
{
  int ** srcUserData ;
  int *  srcSize   ;
  int ** dstUserData ;
  int *  dstSize     ;
  sciGetPointerToUserData( pObjSource, &srcUserData, &srcSize ) ;
  sciGetPointerToUserData( pObjDest  , &dstUserData, &dstSize ) ;
  if ( *srcSize > 0 )
  {
    *dstSize = *srcSize ;
    if ( *dstUserData != NULL ) { FREE(*dstUserData) ; }
    *dstUserData = MALLOC( *srcSize * sizeof(int) ) ;
    if ( *dstUserData == NULL )
    {
		  sciprint(_("%s: No more memory.\n"),"CloneUserData");
		  *dstSize     = 0 ;
		  *srcUserData = NULL ;
		  return -1 ;
    }
    memcpy( *dstUserData, *srcUserData, *srcSize ) ;
  }
  else
  {
    *dstSize     = 0 ;
    *srcUserData = NULL ;
  }
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int cloneFontContext( sciPointObj * pObjSource, sciPointObj * pObjDest )
{
  
  /* struct affectation, doesn't copy the font name */
  /* *(sciGetFontContext(pObjDest)) = *(sciGetFontContext(pObjSource)) ; */

  sciFont * sourceFC = sciGetFontContext( pObjSource ) ;
  sciFont * destFC   = sciGetFontContext( pObjDest   ) ;

  if ( destFC->fontnamelen != 0 )
  {
    FREE( destFC->pfontname ) ;
    destFC->pfontname = NULL ;
  }
  
  /* copy the font name */
  if ( sourceFC->fontnamelen != 0 )
  {
    destFC->pfontname = MALLOC( sourceFC->fontnamelen * sizeof( char ) ) ;
    if ( destFC->pfontname == NULL )
    {
      return -1 ;
    }
    strcpy( destFC->pfontname, sourceFC->pfontname ) ;
  }
  
  destFC->fontnamelen          = sourceFC->fontnamelen         ;
  destFC->backgroundcolor      = sourceFC->backgroundcolor     ;
  destFC->foregroundcolor      = sourceFC->foregroundcolor     ;
  destFC->fonttype             = sourceFC->fonttype            ;
  destFC->fontSize             = sourceFC->fontSize            ;
  destFC->textorientation      = sourceFC->textorientation     ;
  destFC->useFractionalMetrics = sourceFC->useFractionalMetrics;
  return 0 ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciCloneConsole( sciPointObj * pthis )
{
  sciPointObj * newObj = NULL ;
  int posX ;
  int posY ;

  newObj = sciConstructConsole( sciGetParent(pthis) ) ;

  sciInitVisibility( newObj, sciGetVisibility(pthis) ) ;
  sciGetScreenPosition( pthis, &posX, &posY ) ;
  sciInitScreenPosition( newObj, posX, posY ) ;

  /* copy user_data */
  cloneUserData( pthis, newObj ) ;

  return newObj ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciCloneFrame( sciPointObj * pthis )
{
  sciPointObj * newObj = NULL ;
  int posX ;
  int posY ;

  newObj = sciConstructFrame( sciGetParent(pthis) ) ;

  sciInitVisibility( newObj, sciGetVisibility(pthis) ) ;
  sciGetScreenPosition( pthis, &posX, &posY ) ;
  sciInitScreenPosition( newObj, posX, posY ) ;

  /* copy user_data */
  cloneUserData( pthis, newObj ) ;

  return newObj ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciCloneWindow( sciPointObj * pthis )
{
  sciPointObj * newObj = NULL ;
  int posX ;
  int posY ;

  newObj = sciConstructWindow( sciGetParent(pthis) ) ;

  sciInitVisibility( newObj, sciGetVisibility(pthis) ) ;
  sciGetScreenPosition( pthis, &posX, &posY ) ;
  sciInitScreenPosition( newObj, posX, posY ) ;

  /* copy user_data */
  cloneUserData( pthis, newObj ) ;

  return newObj ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciCloneWindowFrame( sciPointObj * pthis )
{
  sciPointObj * newObj = NULL ;
  int posX ;
  int posY ;

  newObj = sciConstructWindowFrame( sciGetParent(pthis) ) ;

  sciInitVisibility( newObj, sciGetVisibility(pthis) ) ;
  sciGetScreenPosition( pthis, &posX, &posY ) ;
  sciInitScreenPosition( newObj, posX, posY ) ;

  /* copy user_data */
  cloneUserData( pthis, newObj ) ;

  return newObj ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciCloneScreen( sciPointObj * pthis )
{
  sciPointObj * newObj = NULL ;
  int posX ;
  int posY ;

  newObj = sciConstructScreen( sciGetParent(pthis) ) ;

  sciInitVisibility( newObj, sciGetVisibility(pthis) ) ;
  sciGetScreenPosition( pthis, &posX, &posY ) ;
  sciInitScreenPosition( newObj, posX, posY ) ;

  /* copy user_data */
  cloneUserData( pthis, newObj ) ;

  return newObj ;
}
/*--------------------------------------------------------------------------*/
