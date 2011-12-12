/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
#include "CloneObjects.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "SetProperty.h"
#include "HandleManagement.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "Scierror.h"
#include "BasicAlgos.h"

#include "createGraphicObject.h" /* cloneGraphicObject */
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

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

  /* get the pointer on features */
  ppThisText = pTEXT_FEATURE( pthis ) ;
  ppCopyText = pTEXT_FEATURE( pobj  ) ;

  ppCopyText->userSize[0] = ppThisText->userSize[0];
  ppCopyText->userSize[1] = ppThisText->userSize[1];

  /* copy user data */
	cloneUserData(pthis, pobj);

  return (sciPointObj *)pobj;
}



/**sciCloneObj
 */
sciPointObj *sciCloneObj (sciPointObj * pobj)
{
    sciPointObj *pClone = MALLOC(sizeof(sciPointObj));

    pClone->UID = cloneGraphicObject(pobj->UID);
    return pClone;

#ifdef __OLD_IMPLEMENTATION__
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
    case SCI_AGREG:

    case SCI_SEGS:
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      Scierror (999, _("This object cannot be cloned.\n"));
      return (sciPointObj *)NULL;
      break;
    }
#endif
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

  cloneUserData(pthis, pobj);

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

	cloneUserData(pthis, pobj);

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

	cloneUserData(pthis, pobj);

  return (sciPointObj *)pobj;
}

/*--------------------------------------------------------------------------*/
/*
 * Copies the ContouredObject properties from the source object to the destination object
 * This code ought to be moved into the Java Model, either in the relevant
 * constructor (ContouredObject) or into an initialization function.
 */
int cloneGraphicContext(char* sourceIdentifier, char* destIdentifier)
{
    double dblTmp = 0.0;
    double *pdblTmp = &dblTmp;
    int iTmp = 0;
    int *piTmp = &iTmp;

    int lineMode;
    int foreground;
    int lineStyle;
    int fillMode;
    int background;
    int markForeground;
    int markBackground;
    int markStyle;
    int markSize;
    int markSizeUnit;
    double lineThickness;

    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_MODE__, jni_bool, &piTmp);
    lineMode = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_COLOR__, jni_int, &piTmp);
    foreground = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_THICKNESS__, jni_double, &pdblTmp);
    lineThickness = dblTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_STYLE__, jni_int, &piTmp);
    lineStyle = iTmp;

    /*
     * Commented out since there is a confusion between Axes' FILLED property
     * and the FILL_MODE ContouredObject property
     * To be corrected
     */
#if 0
    tmp = (int*) setGraphicObjectProperty(pobj->UID, __GO_FILL_MODE__, &fillMode, jni_bool, 1);
    fillMode = *tmp;
#endif

    getGraphicObjectProperty(sourceIdentifier, __GO_BACKGROUND__, jni_int, &piTmp);
    background = iTmp;

    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_FOREGROUND__, jni_int, &piTmp);
    markForeground = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_BACKGROUND__, jni_int, &piTmp);
    markBackground = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_STYLE__, jni_int, &piTmp);
    markStyle = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_SIZE__, jni_int, &piTmp);
    markSize = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_SIZE_UNIT__, jni_int, &piTmp);
    markSizeUnit = iTmp;

    setGraphicObjectProperty(destIdentifier, __GO_LINE_MODE__, &lineMode, jni_bool, 1);
    setGraphicObjectProperty(destIdentifier, __GO_LINE_COLOR__, &foreground, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_LINE_THICKNESS__, &lineThickness, jni_double, 1);
    setGraphicObjectProperty(destIdentifier, __GO_LINE_STYLE__, &lineStyle, jni_int, 1);

    /* Commented out due to the confusion between Axes' FILLED and the FILL_MODE Contoured property */
#if 0
    setGraphicObjectProperty(destIdentifier, __GO_FILL_MODE__, &fillMode, jni_bool, 1);
#endif

    setGraphicObjectProperty(destIdentifier, __GO_BACKGROUND__, &background, jni_int, 1);

    setGraphicObjectProperty(destIdentifier, __GO_MARK_FOREGROUND__, &markForeground, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_BACKGROUND__, &markBackground, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_STYLE__, &markStyle, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_SIZE__, &markSize, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);

    return 0;
}
/*--------------------------------------------------------------------------*/
int cloneUserData( sciPointObj * pObjSource, sciPointObj * pObjDest )
{



#if 0
  int ** srcUserData ;
  int *  srcSize   ;
  int ** dstUserData ;
  int *  dstSize     ;

	/* Get pointer and data of both source and destination */
  sciGetPointerToUserData( pObjSource, &srcUserData, &srcSize ) ;
  sciGetPointerToUserData( pObjDest  , &dstUserData, &dstSize ) ;

	/* Deallocate current user data */
	if (*dstUserData != NULL)
	{
		FREE(*dstUserData);
		*dstUserData = NULL;
		*dstSize = 0;
	}

	/* Reallocate if needed */
  if ( *srcSize > 0 )
  {
		/* update size */
    *dstSize = *srcSize ;

		/* reallocation */
    *dstUserData = MALLOC( *srcSize * sizeof(int) ) ;
    if ( *dstUserData == NULL )
    {
		  Scierror(999, _("%s: No more memory.\n"),"CloneUserData");
		  *dstSize     = 0 ;
		  *dstUserData = NULL ;
		  return -1 ;
    }

		/* copy */
    intArrayCopy( *dstUserData, *srcUserData, *srcSize ) ;
  }
#endif
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int cloneFontContext(char* sourceIdentifier, char* destIdentifier)
{
    double dblTmp = 0.0;
    double *pdblTmp = &dblTmp;
    double fontSize;
    int fontColor;
    int fontStyle;
    int fontFractional;
    int iTmp = 0;
    int *piTmp = &iTmp;

    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_COLOR__, jni_int, &piTmp);
    fontColor = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_STYLE__, jni_int, &piTmp);
    fontStyle = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_SIZE__, jni_double, &pdblTmp);
    fontSize = dblTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_FRACTIONAL__, jni_bool, &piTmp);
    fontFractional = iTmp;

    setGraphicObjectProperty(destIdentifier, __GO_FONT_COLOR__, &fontColor, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_FONT_STYLE__, &fontStyle, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_FONT_SIZE__, &fontSize, jni_double, 1);
    setGraphicObjectProperty(destIdentifier, __GO_FONT_FRACTIONAL__, &fontFractional, jni_bool, 1);

    return 0;
}
