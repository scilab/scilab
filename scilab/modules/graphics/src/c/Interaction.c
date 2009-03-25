/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
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
 *    Comment:
 *    This file contains all functions used to INTERACT with the graphic
 *    window (zoom by pressing button, callbacks...)
 --------------------------------------------------------------------------*/

#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "DrawingBridge.h"
#include "JavaInteraction.h"
#include "axesScale.h"
#include "math_graphics.h"
#include "HandleManagement.h"

/*-------------------------------------------------------------------------------------*/
static int moveObj(sciPointObj * pobj, double displacement[], int displacementSize);
/*-------------------------------------------------------------------------------------*/

/**sciAddCallback
 * exec the callback associated with pthis
 */
int
sciAddCallback (sciPointObj * pthis,char *code, int len, int mevent )
{
  /* Destruction de l'ancienne callback si elle existait */
  sciDelCallback (pthis);

  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      if ((pSUBWIN_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"),"sciAddCallback");
	  return -1;
	}
      else 
	{
	  strncpy(pSUBWIN_FEATURE (pthis)->callback, code, len);
	  pSUBWIN_FEATURE (pthis)->callbacklen = len; 
	  pSUBWIN_FEATURE (pthis)->callbackevent = mevent;
	}
      break;
    case SCI_ARC:
      if ((pARC_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"),"sciAddCallback");
	  return -1;
	}
      else 
	{
	  strncpy(pARC_FEATURE (pthis)->callback, code, len);
	  pARC_FEATURE (pthis)->callbacklen = len;
	  pARC_FEATURE (pthis)->callbackevent = mevent;
                     
	}
      break;
    case SCI_RECTANGLE:
      if ((pRECTANGLE_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"),"sciAddCallback");
	  return -1;
	}
      else 
	{
	  strncpy(pRECTANGLE_FEATURE (pthis)->callback, code, len);
	  pRECTANGLE_FEATURE (pthis)->callbacklen = len;
	  pRECTANGLE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_SEGS:  
      if ((pSEGS_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"),"sciAddCallback");
	  return -1;
	}
      else 
	{
	  strncpy(pSEGS_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_FEC:  
      if ((pFEC_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"),"sciAddCallback");
	  return -1;
	}
      else 
	{
	  strncpy(pFEC_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_GRAYPLOT: 
      if ((pGRAYPLOT_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"),"sciAddCallback");
	  return -1;
	}
      else 
	{
	  strncpy(pGRAYPLOT_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_POLYLINE:
      if ((pPOLYLINE_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  Scierror(9999, _("%s: No more memory.\n"),"sciAddCallback");
	  return -1;
	}
      else 
	{
	  strncpy(pPOLYLINE_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                        
	}
      break;
    case SCI_UICONTROL:
    case SCI_UIMENU:
      break;
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_FIGURE:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	
    default:
      Scierror(999, _("No Callback is associated with this Entity.\n"));
      return -1;
      break;
    }
  return -1;
}




/**sciGetCallback
 * returns the scilab callback code
 */
char *sciGetCallback(sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      return (char *)(pSUBWIN_FEATURE(pthis)->callback);
      break;
    case SCI_ARC:
      return (char *)(pARC_FEATURE(pthis)->callback);
      break;
    case SCI_SEGS: 
      return (char *)(pSEGS_FEATURE(pthis)->callback);
      break; 
    case SCI_FEC: 
      return (char *)(pFEC_FEATURE(pthis)->callback);
      break; 
    case SCI_GRAYPLOT: 
      return (char *)(pGRAYPLOT_FEATURE(pthis)->callback);
      break;
    case SCI_POLYLINE:
      return (char *)(pPOLYLINE_FEATURE(pthis)->callback);
      break;
    case SCI_RECTANGLE:
      return (char *)(pRECTANGLE_FEATURE(pthis)->callback);
      break;
    case SCI_TEXT:
      return (char *)(pTEXT_FEATURE(pthis)->callback);
      break;
    case SCI_SURFACE:
      return (char *)(pSURFACE_FEATURE(pthis)->callback);
      break;
    case SCI_AXES:	
      return (char *)(pAXES_FEATURE(pthis)->callback);
      break;
    case SCI_UIMENU:
      return (char *)(pUIMENU_FEATURE(pthis)->callback);
      break;
    case SCI_UICONTROL:
      return (char *)(pUICONTROL_FEATURE(pthis)->callback);
      break;
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      Scierror(999, _("No Callback is associated with this Entity.\n"));
      return (char *)NULL;
      break;
    }
}

/* 07/11/2002*/
/**sciGetCallbackMouseEvent
 * @author Djalel Abdemouche
 */
int sciGetCallbackMouseEvent(sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE(pthis)->callbackevent;
      break;
    case SCI_ARC:
      return pARC_FEATURE(pthis)->callbackevent;
      break;
    case SCI_SEGS: 
      return pSEGS_FEATURE(pthis)->callbackevent;
      break; 
    case SCI_FEC: 
      return pFEC_FEATURE(pthis)->callbackevent;
      break; 
    case SCI_GRAYPLOT: 
      return pGRAYPLOT_FEATURE(pthis)->callbackevent;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE(pthis)->callbackevent;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE(pthis)->callbackevent;
      break;
    case SCI_TEXT:
      return pTEXT_FEATURE(pthis)->callbackevent;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE(pthis)->callbackevent;
      break;
    case SCI_AXES:	
      return pAXES_FEATURE(pthis)->callbackevent;
      break;
    case SCI_UIMENU:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      Scierror(999, _("No Callback is associated with this Entity.\n"));
      return 100;
      break;
    }

}

/**sciSetCallbackMouseEvent
 * @author Djalel Abdemouche
 */
int sciSetCallbackMouseEvent(sciPointObj * pthis, int mevent)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      pSUBWIN_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_ARC:
      pARC_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_SEGS: 
      pSEGS_FEATURE(pthis)->callbackevent = mevent;
      break; 
    case SCI_FEC: 
      pFEC_FEATURE(pthis)->callbackevent = mevent;
      break; 
    case SCI_GRAYPLOT: 
      pGRAYPLOT_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_TEXT:
      pTEXT_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_SURFACE:
      pSURFACE_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_AXES:	
      pAXES_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_AGREG:
    case SCI_UIMENU:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      Scierror (999, _("No Callback is associated with this Entity.\n"));
      return 100;
      break;
    }
  return 100;
}



/**sciDelCallback
 * returns the length of the scilab callback code
 */
int
sciDelCallback (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pthis)->callbacklen = 0;
      pSUBWIN_FEATURE (pthis)->callbackevent = 100;
      FREE(pSUBWIN_FEATURE (pthis)->callback);
      pSUBWIN_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_ARC:
      pARC_FEATURE (pthis)->callbacklen = 0;
      pARC_FEATURE (pthis)->callbackevent = 100;
      FREE(pARC_FEATURE (pthis)->callback);
      pARC_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pthis)->callbacklen = 0;
      pRECTANGLE_FEATURE (pthis)->callbackevent = 100;
      FREE(pRECTANGLE_FEATURE (pthis)->callback);
      pRECTANGLE_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_SEGS:  
      pSEGS_FEATURE (pthis)->callbacklen = 0;
      pSEGS_FEATURE (pthis)->callbackevent = 100;
      FREE(pSEGS_FEATURE (pthis)->callback);
      pSEGS_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_FEC:
      pFEC_FEATURE (pthis)->callbacklen = 0;
      pFEC_FEATURE (pthis)->callbackevent = 100;
      FREE(pFEC_FEATURE (pthis)->callback);
      pFEC_FEATURE (pthis)->callback = NULL;
      break;  
    case SCI_GRAYPLOT:
      pGRAYPLOT_FEATURE (pthis)->callbacklen = 0; 
      pGRAYPLOT_FEATURE (pthis)->callbackevent = 100;
      FREE(pGRAYPLOT_FEATURE (pthis)->callback);
      pGRAYPLOT_FEATURE (pthis)->callback = NULL;
      break; 
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE (pthis)->callbacklen = 0;
      pPOLYLINE_FEATURE (pthis)->callbackevent = 100;
      FREE(pPOLYLINE_FEATURE (pthis)->callback);
      pPOLYLINE_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_UIMENU:
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_FIGURE:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      Scierror(999, _("No Callback is associated with this Entity.\n"));
      return -1;
      break;
    }
  return 0;
}


/************************************ End of callback Functions ************************************/


/*----------------------------------------------------------------------------------------*/
/* Objmove                                                                                         */
/* move a handle in the graphic window                                                             */
/*----------------------------------------------------------------------------------------*/

static int moveObj(sciPointObj * pobj, double displacement[], int displacementSize)
{
  int i,n;
  double x = displacement[0];
  double y = displacement[1];
  double z = (displacementSize == 3? displacement[2] : 0.0);

  switch (sciGetEntityType (pobj))
  {    
  case SCI_SUBWIN:
    {
      sciSons * psonstmp = sciGetSons(pobj);
      while ((psonstmp != NULL) && (psonstmp->pointobj != NULL))
      {
        moveObj(psonstmp->pointobj, displacement, displacementSize);
        psonstmp = psonstmp->pnext;
      }
    }
    break;
  case SCI_ARC:
    pARC_FEATURE(pobj)->x +=x;
    pARC_FEATURE(pobj)->y += y; 
    if (displacementSize == 3) pARC_FEATURE(pobj)->z += z;
    break;
  case SCI_RECTANGLE: 
    pRECTANGLE_FEATURE(pobj)->x += x;  
    pRECTANGLE_FEATURE(pobj)->y += y; 
    if (displacementSize == 3) pRECTANGLE_FEATURE(pobj)->z += z;
    break; 
  case SCI_AGREG: 
    {
      sciSons * psonstmp = sciGetSons(pobj);
      while ((psonstmp != NULL) && (psonstmp->pointobj != NULL))
      {
        moveObj(psonstmp->pointobj, displacement, displacementSize);
        psonstmp = psonstmp->pnext;
      }
    }
    break;
  case SCI_TEXT:  
    pTEXT_FEATURE(pobj)->x += x; 
    pTEXT_FEATURE(pobj)->y += y;
    if (displacementSize == 3) pTEXT_FEATURE(pobj)->z += z;
    break;
  case SCI_SEGS:
    n=pSEGS_FEATURE(pobj)->Nbr1;   
    for (i=0;i<n;i++) {
      pSEGS_FEATURE(pobj)->vx[i] += x; 
      pSEGS_FEATURE(pobj)->vy[i] += y;
    }
    if (displacementSize == 3) {
      if  (pSEGS_FEATURE(pobj)->vz == (double *)NULL) {
        if ((pSEGS_FEATURE(pobj)->vz = MALLOC (n * sizeof (double)))==NULL) return -1;
        for (i=0;i<n;i++) 
          pSEGS_FEATURE(pobj)->vz[i] = z; 
      }
      else
        for (i=0;i<n;i++) 
          pSEGS_FEATURE(pobj)->vz[i] += z; 
    }
    break;
  case SCI_POLYLINE: 
    n=pPOLYLINE_FEATURE(pobj)->n1;
    for (i=0;i<n;i++) {
      pPOLYLINE_FEATURE(pobj)->pvx[i] += x; 
      pPOLYLINE_FEATURE(pobj)->pvy[i] += y;
    }
    if (displacementSize == 3) {
      if  (pPOLYLINE_FEATURE(pobj)->pvz == (double *)NULL) {
        if ((pPOLYLINE_FEATURE(pobj)->pvz = MALLOC (n * sizeof (double)))==NULL) return -1;
        for (i=0;i<n;i++) 
          pPOLYLINE_FEATURE(pobj)->pvz[i] = z; 
      }
      else
        for (i=0;i<n;i++) 
          pPOLYLINE_FEATURE(pobj)->pvz[i] += z; 
    }
    break;
  case SCI_FEC: 
    for (i=0;i<pFEC_FEATURE(pobj)->Nnode;i++) {
      pFEC_FEATURE(pobj)->pvecx[i] += x; 
      pFEC_FEATURE(pobj)->pvecy[i] += y;
    }
    break;
  case SCI_GRAYPLOT:
    if (pGRAYPLOT_FEATURE(pobj)->type == 2)
    {
      /* Matplot 1, it is not possible to move a Matplot*/
      pGRAYPLOT_FEATURE(pobj)->pvecx[0] += x;
      pGRAYPLOT_FEATURE(pobj)->pvecx[2] += x;
      pGRAYPLOT_FEATURE(pobj)->pvecx[1] += y;
      pGRAYPLOT_FEATURE(pobj)->pvecx[3] += y;
    }
    else if (pGRAYPLOT_FEATURE(pobj)->type == 0)
    {
      /* Grayplot */
      for (i=0;i<pGRAYPLOT_FEATURE(pobj)->nx;i++)
        pGRAYPLOT_FEATURE(pobj)->pvecx[i] += x; 
      for (i=0;i<pGRAYPLOT_FEATURE(pobj)->ny;i++)
        pGRAYPLOT_FEATURE(pobj)->pvecy[i] += y;
    }
    else
    {
      Scierror(999, _("This object can not be moved.\n"));
      return -1;
    }
    
    break;
  case SCI_SURFACE: 
    switch(pSURFACE_FEATURE (pobj)->typeof3d)
    {
    case SCI_FAC3D: 
      n= pSURFACE_FEATURE (pobj)->dimzx* pSURFACE_FEATURE (pobj)->dimzy;
      for (i=0;i<n;i++) {
        pSURFACE_FEATURE(pobj)->pvecx[i] += x; 
        pSURFACE_FEATURE(pobj)->pvecy[i] += y;
      }
      if (displacementSize == 3) {
        if  (pSURFACE_FEATURE(pobj)->pvecz == (double *)NULL) {
          if ((pSURFACE_FEATURE(pobj)->pvecz = MALLOC (n * sizeof (double)))==NULL) return -1;
          for (i=0;i<n;i++) 
            pSURFACE_FEATURE(pobj)->pvecz[i] = z; 
        }
        else
          for (i=0;i<n;i++) 
            pSURFACE_FEATURE(pobj)->pvecz[i] += z; 
      }
      break;
    case SCI_PLOT3D:
      for (i=0;i<pSURFACE_FEATURE (pobj)->dimzx;i++) 
        pSURFACE_FEATURE(pobj)->pvecx[i] += x; 
      for (i=0;i<pSURFACE_FEATURE (pobj)->dimzy;i++) 
        pGRAYPLOT_FEATURE(pobj)->pvecy[i] += y;
      if (displacementSize == 3) {
        n=pSURFACE_FEATURE (pobj)->dimzx*pSURFACE_FEATURE (pobj)->dimzy;
        if  (pSURFACE_FEATURE(pobj)->pvecz == (double *)NULL) {
          if ((pSURFACE_FEATURE(pobj)->pvecz = MALLOC (n * sizeof (double)))==NULL) return -1;
          for (i=0;i<n;i++) 
            pSURFACE_FEATURE(pobj)->pvecz[i] = z; 
        }
        else
          for (i=0;i<n;i++) 
            pSURFACE_FEATURE(pobj)->pvecz[i] += z; 
      }
      break;
    case SCI_CONTOUR:
    case SCI_PARAM3D:
    case SCI_PARAM3D1: /* Nothing to be done */
      break;
    }
    break;
  case SCI_LABEL:
    {
      double pos[3];
      sciGetTextPos(pobj, pos) ;
      sciSetTextPos(pobj, pos[0] + x, pos[1] + y, pos[2] + z) ;
      pLABEL_FEATURE(pobj)->auto_position = FALSE;
      break;
    }
  case SCI_FIGURE:
  case SCI_AXES:
  case SCI_LEGEND:
  case SCI_UIMENU:
  default:
    Scierror(999, _("This object can not be moved.\n"));
    return -1;
    break;
  }    
  
  /* update the object */
  forceMove(pobj, x, y, z);

  return 0;
}

int Objmove (sciPointObj * pobj, double d[], int m,BOOL opt)
{   
  int status = moveObj(pobj, d, m);

	if (status < 0)
	{
		return status;
	}

  if (opt)
  {
    /* should be sci draw single obj */
    sciDrawSingleObj(pobj);
  }
  else
  {
    sciDrawObj(sciGetParentFigure(pobj));
  }
    
  return status;
}

/*---------------------------------------------------------------------------------*/
void pixelRubberBox(sciPointObj * pFigure, BOOL isClick,
                    const int initialRect[4], int endRect[4], int * usedButton)
{
  javaRubberBox(pFigure, isClick, initialRect, endRect, usedButton);
}
/*---------------------------------------------------------------------------------*/
void rubberBox(sciPointObj * pSubwin, BOOL isClick,
               const double initialRect[4], double endRect[4], int * usedButton)
{
  int endPixelRect[4];
  double endFirstCorner[2];
  double endSecondCorner[2];

  /* get up to date coordinates */
  updateSubwinScale(pSubwin);

  /* convert initial rect in  */

  if (initialRect != NULL)
  {
    int initialPixelRect[4];

    /* Convert initial rect in pixels */
    double firstCorner[2];
    double secondCorner[2];
    firstCorner[0] = initialRect[0];
    firstCorner[1] = initialRect[1]; /* upper left point */
    secondCorner[0] = initialRect[0] + initialRect[2];
    secondCorner[1] = initialRect[1] - initialRect[3];
    sciGet2dViewPixelCoordinates(pSubwin, firstCorner, initialPixelRect);
    sciGet2dViewPixelCoordinates(pSubwin, secondCorner, initialPixelRect + 2);

    pixelRubberBox(sciGetParentFigure(pSubwin), isClick, initialPixelRect, endPixelRect, usedButton);
  }
  else
  {
    pixelRubberBox(sciGetParentFigure(pSubwin), isClick, NULL, endPixelRect, usedButton);
  }

  /* here we get the two opposite points of the rectangle in pixels */
  /* convert them in user coordinates */
  sciGet2dViewCoordFromPixel(pSubwin, endPixelRect, endFirstCorner);
  sciGet2dViewCoordFromPixel(pSubwin, endPixelRect + 2, endSecondCorner);

  /* [x,y,w,h] array where (x,y) is the upper left point of the rectangle */
  endRect[0] = Min(endFirstCorner[0], endSecondCorner[0]);
  endRect[1] = Max(endFirstCorner[1], endSecondCorner[1]);
  endRect[2] = Abs(endFirstCorner[0] - endSecondCorner[0]);
  endRect[3] = Abs(endFirstCorner[1] - endSecondCorner[1]);

}
/*---------------------------------------------------------------------------------*/
void interactiveZoom(sciPointObj * pObj)
{
	if (sciGetEntityType(pObj) == SCI_FIGURE)
	{
		interactiveJavaZoom(pObj);
	}
	else if (sciGetEntityType(pObj) == SCI_SUBWIN)
	{
		interactiveJavaSubwinZoom(pObj);
	}
}
/*---------------------------------------------------------------------------------*/
void interactiveRotation(sciPointObj * pFigure)
{
	interactiveJavaRotation(pFigure);
}
/*---------------------------------------------------------------------------------*/
void interactiveSubwinRotation(sciPointObj * pSubwin)
{
  /* get coordinates of first mouse click */
  interactiveJavaSubwinRotation(pSubwin);
}
/*---------------------------------------------------------------------------------*/
void showWindow(sciPointObj * pFigure)
{
  javaShowWindow(pFigure);
}
/*---------------------------------------------------------------------------------*/
void updateViewingAngles(sciPointObj * pSubwin, double deltaAlpha, double deltaTheta)
{
	double newAlpha;
	double newTheta;

	/* Get current angles */
	sciGetViewingAngles(pSubwin, &newAlpha, &newTheta);

	/* update them */
	newAlpha += deltaAlpha;
	newTheta += deltaTheta;

	Obj_RedrawNewAngle(pSubwin, newAlpha, newTheta);

	/* Update info message */
	setInfoMessageWithRotationAngles(sciGetParentFigure(pSubwin), newAlpha, newTheta);
}
/*---------------------------------------------------------------------------------*/

