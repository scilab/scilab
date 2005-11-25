/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
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
#include "bcg.h"
#include "SetProperty.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

/**sciCloneColormap
 * This function clone a colormap from the figure. It must be FREE a lesat.
 * It's the same for all sons
 * @memo Setting the colormap rgbmat must be a m x 3 double RGB matrix:
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE
 */
double *
sciCloneColormap (sciPointObj * pobj)
{
  double *rgbmat;
  int m = sciGetNumColors (pobj);
  int i;

  if ((rgbmat = MALLOC (m * 3 * sizeof (double))) == NULL)
    return (double *) NULL;

  for (i = 0; i < m; i++)
    {
      rgbmat[i] = sciGetScilabXgc (pobj)->Red[i];
      rgbmat[i + m] = sciGetScilabXgc (pobj)->Green[i];
      rgbmat[i + 2 * m] = sciGetScilabXgc (pobj)->Blue[i];
    }
  return (double *) rgbmat;
}



/**CloneText
 * @memo 
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
 
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && ((int) sciGetEntityType(subwinparent) != -1));
  if ((int) sciGetEntityType(subwinparent) == -1)
  {
    return (sciPointObj *)NULL;
  }

  if (!(pobj = ConstructText (subwinparent, sciGetText(pthis), sciGetTextLength(pthis), 
			      sciGetTextPosX(pthis), sciGetTextPosY(pthis),pTEXT_FEATURE(pthis)->wh,pTEXT_FEATURE(pthis)->fill,
			      &foreground,&background,pTEXT_FEATURE(pthis)->isboxed,
			      sciGetIsLine(pthis), sciGetIsFilled(pthis))))
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
  
  if (sciSetFontDeciWidth(pobj, sciGetFontDeciWidth (pthis)) == -1)
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

  ppCopyText->wh[0] = ppThisText->wh[0];
  ppCopyText->wh[1] = ppThisText->wh[1];

  
  if((ppThisText->size_of_user_data != 0) && (ppThisText->user_data != (int *) NULL))
  {
    int size = ppThisText->size_of_user_data;
    
    if((ppCopyText->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
      sciprint("Can not allocate user_data for cloned object.\n");
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
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      sciprint ("This object cannot be cloned !\n");
      return (sciPointObj *)NULL;
      break;
    }
  return (sciPointObj *)NULL;
}



/**CloneRectangle
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
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
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetIsFilled(pobj, sciGetIsFilled (pthis)) == -1)
    return (sciPointObj *)NULL;
  
  if((pRECTANGLE_FEATURE (pthis)->size_of_user_data != 0) && (pRECTANGLE_FEATURE (pthis)->user_data != (int *) NULL))
    {
      int size = pRECTANGLE_FEATURE (pthis)->size_of_user_data;
      
      if((pRECTANGLE_FEATURE (pobj)->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
	sciprint("Can not allocate user_data for cloned object.\n");
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
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
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
				  pPOLYLINE_FEATURE(pthis)->closed, pPOLYLINE_FEATURE(pthis)->n1,pPOLYLINE_FEATURE(pthis)->n2,pPOLYLINE_FEATURE(pthis)->plot,
				  &foreground, &background,
				  &mark_style, &mark_foreground, &mark_background,
				  sciGetIsLine(pthis),  sciGetIsFilled(pthis), 
				  sciGetIsMark(pthis),pPOLYLINE_FEATURE(pthis)->isinterpshaded))){
    return (sciPointObj *)NULL;
  }
  else {
    sciSetCurrentObj(pobj);}; /* F.Leray Adding 26.03.04*/

  
  pPOLYLINE_FEATURE(pobj)->dim_icv = pPOLYLINE_FEATURE(pthis)->dim_icv; /* copy the dimension of the interp. color vector */
  
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;

  if((pPOLYLINE_FEATURE (pthis)->size_of_user_data != 0) && (pPOLYLINE_FEATURE (pthis)->user_data != (int *) NULL))
    {
      int size = pPOLYLINE_FEATURE (pthis)->size_of_user_data;
      
      if((pPOLYLINE_FEATURE (pobj)->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
	sciprint("Can not allocate user_data for cloned object.\n");
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
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
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
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetIsFilled(pobj, sciGetIsFilled (pthis)) == -1)
    return (sciPointObj *)NULL;

  if((pARC_FEATURE (pthis)->size_of_user_data != 0) && (pARC_FEATURE (pthis)->user_data != (int *) NULL))
    {
      int size = pARC_FEATURE (pthis)->size_of_user_data;
      
      if((pARC_FEATURE (pobj)->user_data = (int *) MALLOC(size*sizeof(int)))==NULL){
	sciprint("Can not allocate user_data for cloned object.\n");
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

