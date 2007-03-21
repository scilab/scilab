/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to GET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "GetProperty.h"
#include "bcg.h"
#include "BuildObjects.h"
#include "SetProperty.h"
#include "DrawObjects.h"

#include "PloEch.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


/*#include "GraphicGlobals.h"*/

/* Global variables */
/* #include "Global_PENDOFHANDLETAB.h" */
/* #include "Global_pfiguremdl.h" */
/* #include "Global_paxesmdl.h" */
/* #include "Global_cf_type.h" */

extern sciHandleTab *PENDOFHANDLETAB;

extern int xinitxend_flag;

extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;

extern int versionflag; /* just used for version_flag() function */
extern int cf_type;

/* extern sciClipTab ptabclip[15]; */
static BOOL modereplay = FALSE;

/**sciGetPointerToFeature
 * @memo Returns the pointer to features structure from this object Used only for functions FREE or to use void pointer
 */
void *
sciGetPointerToFeature (sciPointObj * pobj)
{

  /* perhaps is it necessary to cast the returns value with void* type ! */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciFigure *) pFIGURE_FEATURE (pobj);
      break;
    case SCI_SUBWIN:
      return (sciSubWindow *) pSUBWIN_FEATURE (pobj);
      break;
    case SCI_TEXT:
      return (sciText *) pTEXT_FEATURE (pobj);
      break;
    case SCI_TITLE:
      return (sciTitle *) pTITLE_FEATURE (pobj);
      break;
    case SCI_LEGEND:
      return (sciLegend *) pLEGEND_FEATURE (pobj);
      break;
    case SCI_ARC:
      return (sciArc *) pARC_FEATURE (pobj);
      break;
    case SCI_POLYLINE:
      return (sciPolyline *) pPOLYLINE_FEATURE (pobj);
      break;
    case SCI_SEGS:  
      return (sciSegs *) pSEGS_FEATURE (pobj); 
      break;
    case SCI_FEC:
      return (sciFec *) pFEC_FEATURE (pobj); 
      break;
    case SCI_GRAYPLOT:
      return (sciGrayplot *) pGRAYPLOT_FEATURE (pobj);
      break;
    case SCI_RECTANGLE:
      return (sciRectangle *) pRECTANGLE_FEATURE (pobj);
      break;
    case SCI_SURFACE:
      return (sciSurface *) pSURFACE_FEATURE (pobj);
      break;
    case SCI_LIGHT:
      return (sciLightSource *) pLIGHT_FEATURE (pobj);
      break;
    case SCI_AXES:
      return (sciAxes *) pAXES_FEATURE (pobj);
      break;
    case SCI_PANNER:
      return (sciPanner *) pPANNER_FEATURE (pobj);
      break;
    case SCI_SBH:
      return (sciScrollBarHorz *) pSBH_FEATURE (pobj);
      break;
    case SCI_SBV:
      return (sciScrollBarVert *) pSBV_FEATURE (pobj);
      break;
    case SCI_MENU:
      return (sciMenu *) pMENU_FEATURE (pobj);
      break;
    case SCI_MENUCONTEXT:
      return (sciMenuContext *) pMENUCONTEXT_FEATURE (pobj);
      break;
    case SCI_STATUSB:
      return (sciStatusBar *) pSTATUSB_FEATURE (pobj);
      break;
    case SCI_AGREG:
      return (sciAgreg *) pAGREG_FEATURE (pobj);
      break;
    case SCI_MERGE:
      return (sciMerge *) pMERGE_FEATURE (pobj);
      break;
    case SCI_LABEL:
      return (sciLabel *) pLABEL_FEATURE (pobj);
      break;
	case SCI_UIMENU:
	  return (sciUimenu *) pUIMENU_FEATURE (pobj);
	  break;
    default:
      return (void *) NULL;
      break;
    }
  return (void *) NULL;
}



/**sciGetEntityType
 * @memo Gets the type of this Scilab graphic object return -1 if bad param !
 */
sciEntityType
sciGetEntityType (sciPointObj * pobj)
{
  if (pobj != (sciPointObj *) NULL)
    return pobj->entitytype;
  return (sciEntityType)-1;
}


/**sciGetCharEntityType
 * @memo Gets the type of this Scilab graphic object in char format return -1 if bad param !
 */
char *
sciGetCharEntityType (sciPointObj * pobj)
{
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return "Figure";
      break;
    case SCI_SUBWIN:
      return "Axes";
      break;
    case SCI_TEXT:
      return "Text";
      break;
    case SCI_TITLE:
      return "Title";
      break;
    case SCI_LEGEND:
      return "Legend";
      break;
    case SCI_ARC:
      return "Arc";
      break; 
    case SCI_POLYLINE:
      return "Polyline";
      break;
    case SCI_SEGS:
      return (pSEGS_FEATURE (pobj)->ptype == 0) ?  "Segs": "Champ";
      break; 
    case SCI_FEC: 
      return "Fec";
      break;
    case SCI_GRAYPLOT:
      return (pGRAYPLOT_FEATURE (pobj)->type ==0) ? "Grayplot":"Matplot";
      break;
    case SCI_RECTANGLE:
      return "Rectangle";
      break;
    case SCI_SURFACE:
      switch (pSURFACE_FEATURE (pobj)->typeof3d)
	{
	case SCI_FAC3D:
	  return "Fac3d";
	  break;
	case SCI_PLOT3D:
	  return "Plot3d";
	  break;/* DJ.A 2003 */
	case SCI_CONTOUR:
	default:
	  break;
	}
      break;
    case SCI_LIGHT:
      return "Light";
      break;
    case SCI_AXES:
      return "Axis";
      break;
    case SCI_PANNER:
      return "Panner";
      break;
    case SCI_SBH:
      return "Scollbar_hor    ";
      break;
    case SCI_SBV:
      return "Scollbar_ver";
      break;
    case SCI_MENU:
      return "Menu";
      break;
    case SCI_MENUCONTEXT:
      return "Contexte";
      break;
    case SCI_STATUSB:
      return "Statut_bar";
      break;
    case SCI_AGREG:
      return "Compound";
      break;
    case SCI_MERGE:
      return "Merge";
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      return "Label";
      break;
	case SCI_UIMENU: 
		return "Uimenu";
		break;

    default:
      return (char *)NULL;
      break;
    }
  return (char *)NULL;
}

 
/**sciGetGraphicContext
 * @memo Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
 */
sciGraphicContext *
sciGetGraphicContext (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return &(pFIGURE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_SUBWIN:
      return  &(pSUBWIN_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_ARC:
      return  &(pARC_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_SEGS: 
      return  &(pSEGS_FEATURE (pobj)->graphiccontext);
      break; 
    case SCI_FEC:
      return  &(pFEC_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_GRAYPLOT: 
      return  &(pGRAYPLOT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_POLYLINE:
      return  &(pPOLYLINE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_RECTANGLE:
      return  &(pRECTANGLE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_SURFACE:
      return  &(pSURFACE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_LIGHT:
      return  &(pLIGHT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_AXES:
      return  &(pAXES_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_MENU:
      return  &(pMENU_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_MENUCONTEXT:
      return  &(pMENUCONTEXT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_STATUSB:
      return  &(pSTATUSB_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_LEGEND:
      return  &(pLEGEND_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_TEXT:
      return  &(pTEXT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_AGREG:
    case SCI_TITLE:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return (sciGraphicContext *) NULL;
      break;
    }
  return (sciGraphicContext *) NULL;
}


/**sciGetNumColors
 * @memo This function gets the number of the color defined in colormap
 */
int
sciGetNumColors (sciPointObj * pobj)
{ 
  if ( (pobj == pfiguremdl) || (pobj == paxesmdl) 
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_title)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label) ) /* Addings F.Leray 10.06.04 */
    return pFIGURE_FEATURE (pfiguremdl)->numcolors;
  else
    return sciGetScilabXgc (pobj)->Numcolors;
}


/**sciGetColormap
 * This function gets a colormap from the figure. It's the same for all sons
 * @memo Gets the colormap rgbmat must be a m x 3 double RGB matrix:  
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE 
 */
int
sciGetColormap (sciPointObj * pobj, double *rgbmat)
{
  int i;
  int m = sciGetNumColors (pobj);	/* the number of the color*/
    
  for (i = 0; i < m; i++)
  {
    rgbmat[i] = sciGetScilabXgc (pobj)->Red[i];
    rgbmat[i + m] = sciGetScilabXgc (pobj)->Green[i];
    rgbmat[i + 2 * m] = sciGetScilabXgc (pobj)->Blue[i];
  }
  return 0;
}

/* This function */
int sciGetGoodIndex(sciPointObj * pobj, int colorindex) /* return colorindex or m (Default Black) or m+1 (Default White)*/
{
  int m = sciGetNumColors (pobj);	/* the number of the color*/

  if(colorindex == -1) /* Black */
    return m+1;
  else if(colorindex == -2) /* White */
    return m+1 +1;
  else
    return colorindex;
}



/**sciGetForeground
 * @memo Gets the color number of the Foreground
 */
int
sciGetForeground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  /** the foreground is a solid line style in b&w */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SEGS:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
	case SCI_UIMENU:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}


int
sciGetForegroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);

  /** the foreground is a solid line style in b&w */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LABEL:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
	case SCI_UIMENU:
    case SCI_SEGS:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      sciprint ("\r\nNo Foreground is associated with this Entity");
      return -1;
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
    
  return colorindex;
}


/**sciGetBackground
 * @memo Gets the color number of the Background. Be carreful the return of the subwindow 
 * is the feature of its parent figure.
 */
int
sciGetBackground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default
	  +1 is due to the 
	  sciGetBackground that returns the "stored color + 1" 
          that is equal to the color set by the user
      */
      colorindex = (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_PANNER:		/* pas de context graphics */
      break;
    case SCI_SBH:		/* pas de context graphics */
      break;
    case SCI_SBV:		/* pas de context graphics */
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;  
    case SCI_LABEL: /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
   case SCI_UIMENU:
    default:
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}



/* F.Leray 01.04.04*/
/* Adding sciGet____groundToDisplay to display the correct index*/
int
sciGetBackgroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default
	  +1 is due to the 
	  sciGetBackground that returns the "stored color + 1" 
          that is equal to the color set by the user
      */
      colorindex = (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;  
    case SCI_LABEL: /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_PANNER:	/* pas de context graphics */
    case SCI_SBH:	/* pas de context graphics */
    case SCI_SBV:	/* pas de context graphics */
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      sciprint ("\r\nNo Background is associated with this Entity");
      return -1;
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex);

  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}


/**sciGetMarkForeground
 * @memo Gets the color number of the Marks'Foreground
 */
int
sciGetMarkForeground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  /** the foreground is a solid line style in b&w */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_SEGS:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_MENU:
    case SCI_LIGHT:
    case SCI_LABEL:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
	case SCI_UIMENU:
    default:
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}


int
sciGetMarkForegroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);

  /** the foreground is a solid line style in b&w */
  switch (sciGetEntityType (pobj))
    {
   case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_SEGS:
      colorindex =  (sciGetGraphicContext(pobj))->markforeground + 1;
      break;
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_MENU:
    case SCI_LIGHT:
    case SCI_LABEL:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
	case SCI_UIMENU:
    default:
      sciprint ("\r\nNo Mark Foreground is associated with this Entity");
      return -1;
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
    
  return colorindex;
}



/**sciGetMarkBackground
 * @memo Gets the color number of the Marks'Background. Be carreful the return of the subwindow 
 * is the feature of its parent figure.
 */
int
sciGetMarkBackground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_SEGS:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_MENU:
    case SCI_LIGHT:
    case SCI_LABEL:
	case SCI_UIMENU:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}



/* F.Leray 01.04.04*/
/* Adding sciGet____groundToDisplay to display the correct index*/
int
sciGetMarkBackgroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_SEGS:
      colorindex =  (sciGetGraphicContext(pobj))->markbackground + 1;
      break;
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_MENU:
    case SCI_LIGHT:
    case SCI_LABEL:
	case SCI_UIMENU:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      sciprint ("\r\nNo Mark Background is associated with this Entity");
      return -1;
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex);

  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}


/**sciGetLineWidth
 * @memo Gets the line width
 */
int
sciGetLineWidth (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_TEXT:
      return 0;
      break;
    case SCI_TITLE:
      return 0;
      break;
    case SCI_LEGEND:
      return 0;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_MENU:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_STATUSB:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_SEGS: 
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no Line Width\n");
      return -1;
      break;
    }
  return -1;
}


/**sciGetLineStyle
 * @memo Gets the line style
 */
int
sciGetLineStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_MENU:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_STATUSB:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_SEGS: 
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no Line Style\n");
      return -1;
      break;
    }
  return 0;
}


/**sciGetIsMark
 * @memo Gets the line style
 */
BOOL
sciGetIsMark (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_SEGS:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_LEGEND:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:	/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      /*sciprint ("This object has no ismark\n");*/
      return FALSE;
      break;
    }
  return 0;
}


/**sciGetMarkStyle
 * @memo Gets the mark style
 */
int
sciGetMarkStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_SUBWIN:
      /*      return sciGetMarkStyle (sciGetParent (pobj));*/ /* F.Leray 11.04.05 : to fix bug 1296 */
      return (sciGetGraphicContext(pobj))->markstyle; /* and replaced by this line */
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_SEGS:
      return (sciGetGraphicContext(pobj))->markstyle;
      break; 
    case SCI_LEGEND:
     return (sciGetGraphicContext(pobj))->markstyle;
     break; 
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciGetMarkSize
 * @memo Gets the mark size
 */
int
sciGetMarkSize (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->marksize;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->marksize;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->marksize;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->marksize;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->marksize;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->marksize;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->marksize;
      break;
    case SCI_SEGS:
      return (sciGetGraphicContext(pobj))->marksize;
      break; 
    case SCI_LEGEND:
      return (sciGetGraphicContext(pobj))->marksize;
      break; 
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return -1;
      break;
    }
  return 0;
}


/**sciGetMarkSizeUnit
 * @memo Gets the mark size unit
 * 1 : points, 2 : tabulated
 */
int
sciGetMarkSizeUnit (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break;
    case SCI_SEGS:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break; 
    case SCI_LEGEND:
      return (sciGetGraphicContext(pobj))->marksizeunit;
      break; 
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return -1;
      break;
    }
  return 0;
}

/**sciGetIsLine
 * @memo Returns the line drawing existence
 */
BOOL
sciGetIsLine (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_FEC:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_SEGS:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_LEGEND:
      return (sciGetGraphicContext(pobj))->isline;
      break;
    case SCI_TEXT:
      return pTEXT_FEATURE(pobj)->isline ;
      break ;
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:	/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TITLE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no isline\n");
      return -1;
      break;
    }
  return 0;
}

/**sciGetIsFilled
 * @memo Returns the filled line existence
 */
BOOL
sciGetIsFilled (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE(pobj)->isfilled;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE(pobj)->fillflag;
      break;
    case SCI_ARC:
      return pARC_FEATURE(pobj)->fill;
      break;  
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return pLABEL_FEATURE(pobj)->isfilled ;
      break;
    case SCI_TEXT:
      return pTEXT_FEATURE(pobj)->isfilled ;
      break ;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_FEC:
    case SCI_SEGS:
    case SCI_LEGEND:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:	/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TITLE:
	case SCI_UIMENU:
    default:
      sciprint ("This object has no isfilled\n");
      return -1;
      break;
    }
  return 0;
}

/**sciGetFillStyle
 * @memo Gets the fill style
 */
int
sciGetFillStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_SUBWIN:
      return sciGetFillStyle (sciGetParent (pobj));
      break;
    case SCI_TEXT:
      return -1;
      break;
    case SCI_TITLE:
      return -1;
      break;
    case SCI_LEGEND:
      return -1;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LIGHT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no Fill Style\n");
      return -1;
      break;
    }
  return 0;
}

/**sciGetFontContext
 * @memo Returns the structure of the Font Context. Do not use this in the Consturctor Functions !
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the pointer to the font context structure if ok, NULL if not
 */
sciFont *
sciGetFontContext (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return  &(pTEXT_FEATURE (pobj)->fontcontext);
      break;
    case SCI_MENU:
      return  &(pMENU_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_MENUCONTEXT:
      return  &(pMENUCONTEXT_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_STATUSB:
      return  &(pSTATUSB_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_TITLE:
      return  &(pTITLE_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_LEGEND:
      return  &(pLEGEND_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
      return &(pSUBWIN_FEATURE (pobj)->axes.fontcontext);
      break;
    case SCI_AXES:
      /*axes uses the parent subwindow font context S Steer 04.06.04*/
       return &(pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->axes.fontcontext);
       break;  
    case SCI_FIGURE: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
      return &(pFIGURE_FEATURE (pobj)->fontcontext);
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      return &(pLABEL_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
	case SCI_UIMENU:
    default:
      return (sciFont *)NULL;
      break;
    }
}



/****************************************** TEXT ******************************
 **sciGetFontDeciWidth
 * @memo Gets the Font Size
 */
int
sciGetFontDeciWidth (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* return -1*/
      /* Adding F.Leray 08.04.04 */
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_SUBWIN:
      /* return -1;*/
      /* Adding F.Leray 08.04.04 */
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_TEXT:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_TITLE:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_LEGEND:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_ARC:
      return -1;
      break;
    case SCI_POLYLINE:
      return -1;
      break;
    case SCI_RECTANGLE:
      return -1;
      break;
    case SCI_SURFACE:
      return -1;
      break;
    case SCI_LIGHT:
      return -1;
      break;
    case SCI_AXES:  
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_MENU:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_STATUSB:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_LABEL:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      sciprint ("This object has no fontdeciwidth\n");
      return -1;
      break;
    }
  return 0;
}


/*sciGetFontOrientation
 * gets the Font Orientation
 */
int
sciGetFontOrientation (sciPointObj * pobj)
{
  int tmp;
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      tmp = (sciGetFontContext(pobj))->textorientation;
      return tmp;
      break;
    case SCI_TITLE:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_LEGEND:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_AXES:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_MENU:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_STATUSB:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_SUBWIN:
      /* Adding F.Leray 08.04.04 */
      /* Is text orientation usefull with Axes ??*/
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_FIGURE:
      /* Adding F.Leray 08.04.04 */
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_LABEL:
      /* Adding F.Leray 28.05.04 */
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      sciprint ("This object has no textorientation\n");
      return 0;
      break;
    }
  return 0;
}




/**sciGetText
 * @memo Gets the Text in TEXT, TITLE or LEGEND. Be Carreful, 
 * @memo this return a unterminated string (without \0)
 * @memo To obtain the length of the text, use sciGetTextLength@param  sciPointObj *pobj
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  point to char[] if OK or NULL if not 
 */
char *
sciGetText (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->ptextstring;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->text.ptextstring;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->text.ptextstring;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return pLABEL_FEATURE (pobj)->text.ptextstring;
      break;
	case SCI_UIMENU:
	  return pUIMENU_FEATURE (pobj)->label.ptextstring;
	  break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return (char *) NULL;
      break;
    }
}




/**sciGetTextLength
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
unsigned int
sciGetTextLength (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->textlen;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->text.textlen;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->text.textlen;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return pLABEL_FEATURE (pobj)->text.textlen;
      break;
	case SCI_UIMENU:
	  return pUIMENU_FEATURE (pobj)->label.textlen;
	  break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}


/**sciGetFontBackground
 * @memo Gets the background color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int color if OK, -1 if not 
 */
int
sciGetFontBackground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AXES:
    case SCI_STATUSB:
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
    case SCI_FIGURE: /* F.Leray 08.04.04 */
    case SCI_LABEL:  /* F.Leray 28.05.04 */
      colorindex = (sciGetFontContext(pobj))->backgroundcolor+1; /* +1 added by F.Leray 25.06.04 */
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      return -1;
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  return colorindex; 
}


/**sciGetFontBackgroundToDisplay
 */
int
sciGetFontBackgroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AXES:
    case SCI_STATUSB:
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
    case SCI_FIGURE: /* F.Leray 08.04.04 */
    case SCI_LABEL:  /* F.Leray 28.05.04 */
      colorindex = (sciGetFontContext(pobj))->backgroundcolor;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
       sciprint ("\r\nNo FontBackground is associated with this Entity");
      return -1;
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex; 
}



/**sciGetFontForeground
 * @memo Gets the foreground color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int foregroundcolor if OK, -1 if not
 */
int
sciGetFontForeground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_SUBWIN:  /* F.Leray 08.04.04 */
    case SCI_FIGURE:  /* F.Leray 08.04.04 */
    case SCI_LABEL:   /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor+ 1 ; /* Modif. F.Leray 31.03.04*/
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      return -1;
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  return colorindex;
}


/**sciGetFontForegroundToDisplay
 */
int
sciGetFontForegroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_SUBWIN:  /* F.Leray 08.04.04 */
    case SCI_FIGURE:  /* F.Leray 08.04.04 */
    case SCI_LABEL:   /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor+ 1 ; /* Modif. F.Leray 31.03.04*/
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      sciprint ("\r\nNo FontForeground is associated with this Entity");
      return -1;
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}





/**sciGetFontStyle
 * @memo Gets the font style 

 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 OK, -1 if not
 */
int
sciGetFontStyle (sciPointObj * pobj)
{


  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SUBWIN:
    case SCI_FIGURE:
    case SCI_LABEL: /* F.Leray 10.06.04 */
      return sciGetFontContext(pobj)->fonttype;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      return -1;
      break;
    }
}


/**sciGetFontName
 * @memo Gets the font name
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  char array of string font name if ok, NULL if not
 */
char *
sciGetFontName (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_TITLE:
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_LEGEND:
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      return (char *) NULL;
      break;
    }
}



/**sciGetFontNameLength
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int > 0 if OK, 0 if not
 */
unsigned int
sciGetFontNameLength (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_TITLE:
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_LEGEND:
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
	case SCI_UIMENU:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
}



/**sciGetTextPosX
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosX (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->x;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetTextPosWidth
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosWidth (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 0;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetTextPosHeight
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosHeight (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 0;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetTextPosY
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosY (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->y;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE: 
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetTitlePos
 * @memo Returns the Title Position in the graphique window. This function is actualy reserved for internal use, not for scilab users
 */
POINT2D
sciGetTitlePos (sciPointObj * pobj)
{
  POINT2D tmppoint;
  if (sciGetEntityType (pobj) == SCI_TITLE)
    return pTITLE_FEATURE (pobj)->pos;
  else
    {
      tmppoint.x = -1;
      tmppoint.y = -1;
      sciprint ("Your are not using a title object !\n");
      return tmppoint;
    }
}



/**sciGetTitlePlace
 * @memo Returns the Title Position
 * @param sciPointObj * pobj: the pointer to the entity
 * @return sciTitlePlace if ok, -1 if not
 */
sciTitlePlace
sciGetTitlePlace (sciPointObj * pobj)
{
  if (sciGetEntityType (pobj) == SCI_TITLE)
    return pTITLE_FEATURE (pobj)->titleplace;
  else
    sciprint ("Your are not using a title object !\n");
  return (sciTitlePlace)-1;
}


/**sciGetLegendPlace
 * @memo Returns the Title place with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return sciTitlePlace
 */
sciTitlePlace
sciGetLegendPlace (sciPointObj * pobj)
{
  return pTITLE_FEATURE (pobj)->titleplace;
}



/**sciGetLegendPos
 * @memo Returns the Title Position
 */
POINT2D
sciGetLegendPos (sciPointObj * pobj)
{
  POINT2D tmppoint;
  if (sciGetEntityType (pobj) == SCI_TITLE)
    return pLEGEND_FEATURE (pobj)->pos;
  else
    {
      tmppoint.x = -1;
      tmppoint.y = -1;
      sciprint ("Your are not using a legend object !\n");
      return tmppoint;
    }
}




/**sciGetParentFigure
 * @memo Returns the the Figure parent
 */
sciPointObj *
sciGetParentFigure (sciPointObj * pobj)
{
  if ( (pobj == pfiguremdl) || (pobj == paxesmdl)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_title)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label) ) /* Addings F.Leray 10.06.04 */
    return (sciPointObj *) pfiguremdl;
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
      {
        sciPointObj * figure ;
        figure = sciGetScilabXgc ((sciPointObj *) pobj)->mafigure;
        return sciGetScilabXgc ((sciPointObj *) pobj)->mafigure;
      }  
      break;
    default:  
      return (sciPointObj *) NULL;
      break;
    }
  return (sciPointObj *) NULL;
}
/** 19/09/2002 ***/
/**sciGetParentSubwin
 * @memo Returns the the Subwin parent
 */
sciPointObj *
sciGetParentSubwin (sciPointObj * pobj)
{
  sciPointObj *subwin;

  subwin = pobj;
  switch (sciGetEntityType (pobj))
    { 
    case SCI_FIGURE:
      return (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ());
      break;
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
      while (sciGetEntityType(subwin) != SCI_SUBWIN)
	subwin=sciGetParent(subwin);      
      return (sciPointObj *) subwin;  
      break;                                                     
    default:  
      return (sciPointObj *) NULL;
      break;
    }
  return (sciPointObj *) NULL;
}

/**sciGetNumfigure
 * @memo Returns the the Number of parent figure
 */
int 
sciGetNumFigure (sciPointObj * pobj)
{
  sciPointObj *figure = pobj;
  
  switch (sciGetEntityType (pobj))
    { 
    case SCI_FIGURE:
      return  sciGetNum(figure) ;
      break;
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
      while (sciGetEntityType(figure) != SCI_FIGURE)
	figure=sciGetParent(figure);      
      return sciGetNum(figure); 
      break;                                                     
    default:  
      return -1;
      break;
    }
  return -1;
}

/**sciGetScilabXgc
 * @memo Returns the ScilabXgc of the root recursively...
 */
struct BCG *
sciGetScilabXgc (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->pScilabXgc;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:  
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE:  
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
      /* on recherche la root par recursivite 
	 puisque scilabxgc n'est connu que par le parent */
      return (struct BCG *) sciGetScilabXgc (sciGetParent (pobj));	
      break;
    default:
      return (struct BCG *) NULL;
      break;
    }
  return (struct BCG *) NULL;
}



 
/**sciGetGraphicMode
 * @memo Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
 */
scigMode *sciGetGraphicMode (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return &(pFIGURE_FEATURE (pobj)->gmode);
      break;
    case SCI_SUBWIN:
      return &(pSUBWIN_FEATURE (pobj)->gmode);
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_LEGEND:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return (scigMode *) NULL;
      break;
    }
  return (scigMode *) NULL;
}


/**sciGetIsClipRegionValuated*/
/* To know if clip_region has been set at least once */
/* (using sciSetClipping) */
int
sciGetIsClipRegionValuated (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->clip_region_set;
      break;   
    case SCI_SEGS: 
      return pSEGS_FEATURE (pobj)->clip_region_set;
      break;      
    case SCI_TEXT: 
      return pTEXT_FEATURE (pobj)->clip_region_set;
      break;   
    case SCI_LIGHT: 
      return pLIGHT_FEATURE (pobj)->clip_region_set;
      break;   
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_SURFACE:
    case SCI_LEGEND: 
    case SCI_TITLE:    
    case SCI_AGREG: 
    case SCI_FIGURE: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return -2;
      break;
    } 
  return -2;
}


/**sciGetIsClipping*/
int
sciGetIsClipping (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
   
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->isclip;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->isclip;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->isclip;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->isclip;
      break;   
    case SCI_SEGS: 
      return pSEGS_FEATURE (pobj)->isclip;
      break;      
    case SCI_TEXT: 
      return pTEXT_FEATURE (pobj)->isclip;
      break;   
    case SCI_LIGHT: 
      return pLIGHT_FEATURE (pobj)->isclip;
      break;   
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->isclip;
      break;
    case SCI_SURFACE:
    case SCI_LEGEND: 
    case SCI_TITLE:    
    case SCI_AGREG: 
    case SCI_FIGURE: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return -2;
      break;
    } 
  return -2;
}


/**sciGetClipping
 * @memo Gets the clipping area
 * 29/11/2002
 */
double *
sciGetClipping (sciPointObj * pobj)
{
  /*   int index; */

  /*   index = sciGetIsClipping (pobj); */
  /*   return ptabclip[index].clip; */
  switch (sciGetEntityType (pobj))
    {
   
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->clip_region;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->clip_region;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->clip_region;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->clip_region;
      break;   
    case SCI_SEGS: 
      return pSEGS_FEATURE (pobj)->clip_region;
      break;      
    case SCI_TEXT: 
      return pTEXT_FEATURE (pobj)->clip_region;
      break;   
    case SCI_LIGHT: 
      return pLIGHT_FEATURE (pobj)->clip_region; /* not used for now 04.04.2005 */
      break;   
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->clip_region;
      break;
      /* not used for now 04.04.2005 */
/*     case SCI_SURFACE: */
/*       return pSURFACE_FEATURE (pobj)->clip_region; */
/*       break; */
/*     case SCI_LEGEND:  */
/*       return pLEGEND_FEATURE (pobj)->clip_region; */
/*       break; */
/*     case SCI_TITLE:   */
/*       return pTITLE_FEATURE (pobj)->clip_region; */
/*       break; */
/*     case SCI_AGREG:  */
/*       return pAGREG_FEATURE (pobj)->clip_region; */
/*       break; */
/*     case SCI_FEC:  */
/*       return pFEC_FEATURE (pobj)->clip_region; */
/*       break; */
/*     case SCI_GRAYPLOT: */
/*       return pGRAYPLOT_FEATURE (pobj)->clip_region; */
/*       break; */
    case SCI_LABEL:
      return pLABEL_FEATURE (pobj)->clip_region;
      break;
	case SCI_UIMENU:
    case SCI_SURFACE:
    case SCI_LEGEND: 
    case SCI_TITLE:    
    case SCI_AGREG:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_FIGURE: 
    default:
      sciprint("Error: clip_region is NULL\n");
      return (double *) NULL;
      break;
    }   
  sciprint("Error: clip_region is NULL\n");
  return (double *) NULL;

}


/**sciGetHighLight
 * @memo Returns the highlight mode where the object are.
 */

BOOL
sciGetHighLight (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /*return (sciGetGraphicMode (pobj))->highlight; */
      break;
    case SCI_SUBWIN:
      /* the value is inhirated by the parent */
      /*return sciGetHighLight (sciGetParentFigure (pobj));*/
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object cannot be highlighted\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetAddPlot
 * @memo Returns the mode of the adding plot
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if no 
 * @author Djalel ABDEMOUCHE
 * 25/10/2002
 */
BOOL
sciGetAddPlot (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->addplot;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicMode (pobj))->addplot; 
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object cannot be addploted\n");
      return FALSE;
      break;
    } 
  return FALSE;
}


/**sciGetAutoScale
 * @memo Returns the mode of the autoscaling
 */
BOOL
sciGetAutoScale (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->autoscaling;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicMode (pobj))->autoscaling;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      sciprint ("This object cannot be autoscaled\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetZooming
 * @memo Returns the zooming state of the object is it or not zoomed.
 */
BOOL
sciGetZooming (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->zooming;
      break;
    case SCI_SUBWIN:
      /* the value is inherited by the parent */
      return (sciGetGraphicMode (pobj))->zooming;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
	case SCI_UIMENU:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("\r\nThis object cannot be zoomed \r\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetGraphicsStyle
 * @memo Returns the graphics style
 */
BOOL
sciGetGraphicsStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->oldstyle;
      break;
    case SCI_SUBWIN: 
      /* the value is inhirated by the parent */
      return sciGetGraphicsStyle (sciGetParentFigure (pobj));
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("\r\nNothing to do\n");
      return FALSE;
      break;
    }
  return FALSE;
}

/**sciGetXorMode
 * @memo Returns the drawing Xor mode
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0,1,... 15
 * @author Djalel ABDEMOUCHE
 * @see sciSetXorMode
 */
int
sciGetXorMode (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->xormode;
      break;
    case SCI_SUBWIN: 
      /* the value is inhirated by the parent */
      return sciGetXorMode (sciGetParentFigure (pobj));
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("\r\nNothing to do\n");
      return FALSE;
      break;
    }
  return FALSE;
}

/**
 * To know wether an object will be displayed on the screen or not.
 * Because if one of its parent is not visible then it won't be displayed.
 */
BOOL
sciGetRealVisibility (sciPointObj * pobj)
{

  if ( pobj == NULL )
  {
    return FALSE ;
  }

  if ( sciGetEntityType( pobj) == SCI_FIGURE )
  {
    return sciGetVisibility( pobj ) ;
  }

  if ( !sciGetVisibility( pobj ) )
  {
    return FALSE ;
  }

  return sciGetRealVisibility( sciGetParent( pobj ) ) ;

}

/**sciGetVisibility*/
BOOL
sciGetVisibility (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->visible;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->visible;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->visible;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->visible;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->visible;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->visible;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->visible;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE (pobj)->visible;
      break;    
    case SCI_SEGS: 
      return pSEGS_FEATURE (pobj)->visible;
      break;    
    case SCI_FEC: 
      return pFEC_FEATURE (pobj)->visible;
      break;    
    case SCI_GRAYPLOT: 
      return pGRAYPLOT_FEATURE (pobj)->visible;
      break;    
    case SCI_TEXT: 
      return pTEXT_FEATURE (pobj)->visible;
      break;   
    case SCI_LIGHT: 
      return pLIGHT_FEATURE (pobj)->visible;
      break;
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->visible;
      break;    
    case SCI_AGREG: 
      return pAGREG_FEATURE (pobj)->visible;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return pLABEL_FEATURE (pobj)->visible;
      break;
	case SCI_UIMENU:
	  return pUIMENU_FEATURE (pobj)->visible;
	  break;
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      return TRUE;
      break;
    }
}



/**sciGetResize (replace sciGetwresize()in the next version)
 * @memo Returns if this object is in autoresizing mode (when the window is resized by user)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if no
 */
BOOL
sciGetResize (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->wresize;
      break;
    case SCI_SUBWIN:
      /* the value is inhirated by the parent */
      /*return sciGetResize (sciGetParent (pobj));*/
      return (sciGetGraphicMode (pobj))->wresize;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES: 
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      sciprint ("This object cannot be resized\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetName
 * @memo Returns the name of the Figure or SubWindow
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the pointer to the char name
 */
char *
sciGetName (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->name;
      break;
    case SCI_SUBWIN:
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can be named\n");
      return NULL;
      break;
    }
}


/**sciGetNameLength
 * @memo Returns the length of the name of the Figure or SubWindow
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the length of the Name
 */
int
sciGetNameLength (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->namelen;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->namelen;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure or SubWindow can be named\n");
      return 0;
      break;
    }
}


/**sciGetNum
 * @memo Returns the number of the Figure.
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the number of the windows contening this entity
 */
int
sciGetNum (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->number;
      break;
    case SCI_SUBWIN:
      return sciGetNum (sciGetParent(pobj));
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can be numerated\n");
      return -1;
      break;
    }
}


/**sciGetWidth
 * @memo Returns the width in pixel of the figure or subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the width of the dimension of the window or figure 
 * (the visibility dimension) in pixel dimension
 */
double
sciGetWidth (sciPointObj * pobj)
{
  struct BCG *Xgc;
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      Xgc=pFIGURE_FEATURE (pobj)->pScilabXgc;
      pFIGURE_FEATURE (pobj)->windowdimwidth= Xgc->CWindowWidth;
      return pFIGURE_FEATURE (pobj)->windowdimwidth;
      break;
    case SCI_SUBWIN:
       return pSUBWIN_FEATURE (pobj)->windimwidth;
      break;
    default:
      sciprint ("Only Figure is physical dimensioned\n");
      return -1;
      break;
    }
}


/**sciGetHeight
 * @memo Returns the height in pixel of the figure or subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the height of the dimension of the window or figure (the visibility dimension) in pixel dimension
 */
double 
sciGetHeight (sciPointObj * pobj)
{
  struct BCG *Xgc;
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      Xgc=pFIGURE_FEATURE (pobj)->pScilabXgc;
      pFIGURE_FEATURE (pobj)->windowdimheight= Xgc->CWindowHeight;
      return pFIGURE_FEATURE (pobj)->windowdimheight;
      break;
    case SCI_SUBWIN:
       return pSUBWIN_FEATURE (pobj)->windimheight;
      break;
    default:
      sciprint ("Only Figure is physical dimensioned\n");
      return -1;
      break;
    }
}



/**sciGetFigurePosX
 * @memo Returns the horizontal position of  the FIGURE (the window) in root, in pixels
 * @param sciPointObj * pobj
 * @return the position in pixel dimension
 */
int
sciGetFigurePosX (sciPointObj * pobj)
{
  integer x[2],y=0,cur,num,na;
  double d=0;
  
  switch (sciGetEntityType (pobj))
    { 
    case SCI_FIGURE:
      /* synchronize figure position with its actual value */
      num=pFIGURE_FEATURE(pobj)->number;
      C2F(dr)("xget","window",&y,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      C2F(dr)("xget","wpos",&y,x,&na,PI0,PI0,PI0,&d,PD0,PD0,PD0,4L,4L);
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      pFIGURE_FEATURE (pobj)->inrootposx=x[0];
      return pFIGURE_FEATURE (pobj)->inrootposx;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}


/**sciGetFigurePosY
 * @memo Returns the vertical position of the window in pixels
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the position in pixel dimension
 */
int
sciGetFigurePosY (sciPointObj * pobj)
{  
  integer x[2],y=0,cur,num,na;
  double d=0;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* synchronize figure position with its actual value */
      num=pFIGURE_FEATURE(pobj)->number; /* Adding this line to dix bug scf() F.Leray 08.12.04 */
      C2F(dr)("xget","window",&y,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      C2F(dr)("xget","wpos",&y,x,&y,PI0,PI0,PI0,&d,PD0,PD0,PD0,4L,4L);
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      pFIGURE_FEATURE (pobj)->inrootposy=x[1];
      return pFIGURE_FEATURE (pobj)->inrootposy;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}


/**sciGetIsFigureIconified
 * @memo Determines whether the specified Figure is minimized (iconic). 
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if no
 */
BOOL
sciGetIsFigureIconified (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* hWndParent de Type HWND (BCG) "WIN" */
      /*return (pFIGURE_FEATURE (pobj)->isiconified =
	IsIconic ((sciGetScilabXgc (pobj))->hWndParent)); */
      return FALSE;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return iconic status\n");
      return FALSE;
      break;
    }
}

/**sciGetSubwindowPosX
 * Gets scrollbar position; 
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int vertical position in pixel
 **/
int
sciGetSubwindowPosX (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      /* selectionner le xgc correspondant puis */
      return pSUBWIN_FEATURE (pobj)->infigureposx;
      /* C2F(getwindowpos)( width, height, PI0, PI0); */
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}


/**sciGetSubwindowPosY
 * @memo Gets subwindow position; 
 * @param sciPointObj * pobj: the pointer to the entity in pixel
 * @return  int vertical position 
 **/
int
sciGetSubwindowPosY (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      /* selectionner le xgc correspondant puis */
      return pSUBWIN_FEATURE (pobj)->infigureposy;
      /* C2F(getwindowpos)( width, height, PI0, PI0); */
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}



/**sciGetIsSelected
 * @memo Determines wether this object is selected or not.
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if not
 */
BOOL
sciGetIsSelected (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->isselected;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->isselected;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->isselected;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->isselected;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->isselected;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->isselected;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->isselected;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE (pobj)->isselected;
      break;    
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_TEXT:
    case SCI_LIGHT:
    case SCI_AXES: 
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return FALSE;
      break;
    }
}



/**sciGetSelectedSon
 * @memo Determines wether this object is selected or not. 
 * WARNING TO BE DEFINED. No sciSetSelected is coded
 * @param sciPointObj * pparent: the pointer to the entity
 * @return the pointer sciPointObj *poinson of the current selected son
 */
sciPointObj *
sciGetSelectedSon (sciPointObj * pparent)
{
  sciSons *psonstmp;

  psonstmp = sciGetSons (pparent);
  /* init */
  if (psonstmp != (sciSons *) NULL)
    {
      /* on peut commencer sur le next */
      /* tant que le fils  */
      while ((psonstmp->pnext != (sciSons *) NULL)
	     && (sciGetIsSelected (psonstmp->pointobj) != TRUE))
	psonstmp = psonstmp->pnext;
      if (sciGetIsSelected (psonstmp->pointobj) == FALSE)
	return (sciPointObj *) NULL;
      else
	return (sciPointObj *) psonstmp->pointobj;
    }
  else
    return (sciPointObj *) NULL;

}



/**sciGetSelectedSubWin
 * @memo Determines wich SubWin is selected or not. WARNING TO BE DEFINED.
 * @param sciPointObj * pparent: the pointer to the entity
 * @return the pointer sciPointObj *poinson of the current selected son
 */
sciPointObj *
sciGetSelectedSubWin (sciPointObj * pparent)
{
  sciSons *psonstmp;
  if (sciGetEntityType (pparent) != SCI_FIGURE)
    {	  
      sciprint("This Handle is not a Figure\n");
      return (sciPointObj *)NULL;
    }
  psonstmp = sciGetSons (pparent);
  /* init */
  if (psonstmp != (sciSons *) NULL)
    {
      /* on peut commencer sur le next */
      /* tant que le fils  */
      while ((psonstmp->pnext != (sciSons *) NULL)
	     && ((sciGetIsSelected (psonstmp->pointobj) != TRUE) 
		 || (sciGetEntityType (psonstmp->pointobj) != SCI_SUBWIN)))
	psonstmp = psonstmp->pnext;
      if (sciGetIsSelected (psonstmp->pointobj) == FALSE) {
	/*printf("cas 1 : (pSUBWIN_FEATURE(psonstmp->pointobj))->isselected = %d\n",(pSUBWIN_FEATURE(psonstmp->pointobj))->isselected); */ /* BUG The Hnadle is not a SubWindow passes through here F.Leray 23.07.04 */
	return (sciPointObj *) NULL;}
      else 
	if (sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) {
	/*   printf("cas 2 : (pSUBWIN_FEATURE(psonstmp->pointobj))->isselected = %d\n",(pSUBWIN_FEATURE(psonstmp->pointobj))->isselected); */
	  return (sciPointObj *) psonstmp->pointobj;}
	else return (sciPointObj *) NULL;
    }
  else
    return (sciPointObj *) NULL;
}


/**sciIsExistingSubWin
 * @memo Determines if this SubWindow is an existing one in the current SCI_FIGURE 
 * in association with the wrect and frect....
 * @param WRect: Window rectangle dimension in double, 
 * @param FRect: window user's scale dimension in double, 
 * @param logscale: flag for logarithmic window
 * @return the pointer to the existing SubWindow, or Null if no one is existing
 */
sciPointObj *
sciIsExistingSubWin (WRect)
     double WRect[4];
{
  sciPointObj *pparentfigure;
  sciSons *psonstmp;

  double WRectTmp[4];
  int stop = 0,i;


  /* Initialisation de WRectTmp a 0*/
  for(i=0;i<4;i++)
    WRectTmp[i] = 0.;

  pparentfigure = (sciPointObj *)sciGetCurrentFigure();
  if (sciGetEntityType (pparentfigure) != SCI_FIGURE)
    return (sciPointObj *)NULL;

  /**  15/03/2002 **/
  if ((WRect[0] == 0.)&&(WRect[1] == 0.)&&(WRect[2] == 1.)&&(WRect[3] == 1.))
    return (sciPointObj *) sciGetLastSons (pparentfigure)->pointobj;

  psonstmp = sciGetSons (pparentfigure);
  /* init */
  if (psonstmp != (sciSons *) NULL)
    {
      /* on peut commencer sur le next */
      /* tant que le fils  */
      /* j'utilise l'ordre d'evaluation normalise C pour */
      /* verifier d'abord qu'il s'agit d'une sous fenetre */
      /* puis si les WRect et FRect sont bons */
      stop = 0;
      while ((psonstmp->pnext != (sciSons *) NULL) && (stop == 0))
	{
	  WRectTmp[0] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[0];
	  WRectTmp[1] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[1];
	  WRectTmp[2] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[2];
	  WRectTmp[3] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[3];

	  if ((sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	      && (Abs(WRectTmp[0] - WRect[0]) < 1e-8)
	      && (Abs(WRectTmp[1] - WRect[1]) < 1e-8)
	      && (Abs(WRectTmp[2] - WRect[2]) < 1e-8)
	      && (Abs(WRectTmp[3] - WRect[3]) < 1e-8))
	    {
	      stop = 1;
	    }
	  else 
	    psonstmp = psonstmp->pnext;
	}
		
      if ((sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	  && (Abs(WRectTmp[0] - WRect[0]) < 1e-8)
	  && (Abs(WRectTmp[1] - WRect[1]) < 1e-8)
	  && (Abs(WRectTmp[2] - WRect[2]) < 1e-8)
	  && (Abs(WRectTmp[3] - WRect[3]) < 1e-8))

	return (sciPointObj *) psonstmp->pointobj;
      else return (sciPointObj *) NULL;
    }

  return (sciPointObj *)NULL;
}

sciPointObj *sciIsExistingFigure(value)
     int *value;
{
  struct BCG *figGC = NULL;
  integer v=0;
  double dv=0.0; 

  if(GetDriverId() == 0) /* driver Win32 or X11 F.Leray 26.08.04 */
      figGC=GetWindowXgcNumber(*value);
  else
      {
          /* drivers GIF, Pos or Xfig are always the current one (only window number "value" at a given time) (for now) F.Leray 26.08.04 */
          /* So let's get the current gc */
	      C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&figGC,&dv,&dv,&dv,5L,10L);
	  }

  if ((figGC != (struct BCG *) NULL) && (figGC->mafigure != (sciPointObj *) NULL)) /* ajout F.Leray 22.07.04 */
    return figGC->mafigure;
  else
    return  (sciPointObj *) NULL;    

}




/**sciGetScrollPosV
 * @memo Returns the vertical scrollbar position
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the scroll bar position
 */
int
sciGetScrollPosV (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SBV:
    default:
      /*   return GetScrollPos ((sciGetScilabXgc (pobj))->CWindow, SB_VERT);*/ /* GetScrollPos fct stand dans WIN */
      return -1;
      break;
    }
}



/**sciGetScrollPosH
 * @memo Returns the vertical scrollbar position
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the scroll bar position
 */
int
sciGetScrollPosH (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SBH:
    default:
      /* return GetScrollPos ((sciGetScilabXgc (pobj))->CWindow, SB_HORZ);*/ /* GetScrollPos fct stand dans WIN */
      return -1;
      break;
    }
}



/**sciGetCurrentFigure
 * @memo Returns the pointer to the current selected figure. 
 */
sciPointObj *
sciGetCurrentFigure ()
{ 
  /* debug F.Leray 22.07.04 */
  BCG * moncurScilabXgc = NULL;
  sciPointObj * pfigure = NULL;

  
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;  

  moncurScilabXgc = sciGetCurrentScilabXgc();
  if(moncurScilabXgc == (struct BCG *) NULL)
	  return (sciPointObj *) NULL;

  pfigure = (sciPointObj *) (moncurScilabXgc->mafigure);

  if(pfigure == (sciPointObj *) NULL )
    {
      /* it would mean that we have change the driver to GIF,Pos or PPM and perform a xinit F.Leray 22.07.04 */
      if ((mafigure = ConstructFigure (moncurScilabXgc)) != NULL)
	{
	  sciSetCurrentObj (mafigure); 
	  moncurScilabXgc->mafigure = mafigure;
          moncurScilabXgc->graphicsversion = 0;
	  if ((masousfen = ConstructSubWin (mafigure, moncurScilabXgc->CurWindow)) != NULL) {
	    sciSetCurrentObj (masousfen);
	    sciSetOriginalSubWin (mafigure, masousfen);
	    cf_type=1;/* current figure is a graphic one */
	  }
	}
    }
  
  return (sciPointObj *) sciGetCurrentScilabXgc ()->mafigure  ;
  cf_type=1;/* current figure is a graphic one */
}                                                               


struct BCG *
sciGetCurrentScilabXgc ()
{ 
  integer v=0;
  double *XGC, dv=0;
  struct BCG *CurrentScilabXgc = (struct BCG *) NULL;
  
  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;
  return (struct BCG *) CurrentScilabXgc;
}


/**sciGetReplay
 * @memo returns the variable "we are in replay mode"
 */
BOOL
sciGetReplay ()
{
  return modereplay;

}


/**sciGetPoint
 * @memo returns pointer to the points of the entity, and a pointer to the number of points. This function allocates memory for the tab of point, so after using the tab don't forget to free it
 */

/**MAJ pour le 3D DJ.Abdemouche 2003**/
double *sciGetPoint(sciPointObj * pthis, int *numrow, int *numcol)
{
  double *tab;
  int i,k;
  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:
      *numrow = 2;
      *numcol = 2;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = (double) sciGetFigurePosX (pthis);
      tab[1] = (double) sciGetFigurePosY (pthis);
      tab[2] = (double)sciGetWidth (pthis);
      tab[3] = (double)sciGetHeight (pthis);
      return (double*)tab;
      break;
    case SCI_SUBWIN:
      *numrow = 3;
      *numcol = 2;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] =  (double) sciGetSubwindowPosX (pthis);
      tab[1] =  (double) sciGetSubwindowPosY (pthis);
      tab[2] = (double)sciGetWidth (pthis);
      tab[3] = (double)sciGetHeight (pthis);
      tab[4] = (double)sciGetScilabXgc(pthis)->CWindowWidthView; 
      tab[5] = (double)sciGetScilabXgc(pthis)->CWindowHeightView; 
      return (double*)tab;
      break;
    case SCI_POLYLINE:
      *numrow = pPOLYLINE_FEATURE (pthis)->n1;
      *numcol=(pPOLYLINE_FEATURE (pthis)->pvz != NULL)? 3:2;

      if ( (*numrow)*(*numcol) == 0 )
      {
        return NULL ;
      }

      if(*numcol==2 && pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  *numcol = (*numcol)+1; /* colonne de 0. a prendre en compte / afficher => numcol+1*/
	  if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
          {
	    return NULL ;
            sciprint("Cannot return point, memory full.\n") ;
          }
	  for (i=0;i < *numrow;i++)
	    {
	      tab[i] = pPOLYLINE_FEATURE (pthis)->pvx[i];	
	      tab[*numrow+i]= pPOLYLINE_FEATURE (pthis)->pvy[i];
	      tab[(2*(*numrow))+i]= 0.;
	    }
	}
      else
      {
	if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
        {
          sciprint("Cannot return point, memory full.\n");
	  return NULL;
        }
	for (i=0;i < *numrow;i++)
	  {
	    tab[i] = pPOLYLINE_FEATURE (pthis)->pvx[i];	
	    tab[*numrow+i]= pPOLYLINE_FEATURE (pthis)->pvy[i];
	    if (*numcol== 3)
	      tab[(2*(*numrow))+i]= pPOLYLINE_FEATURE (pthis)->pvz[i]; 
	  }
      }
      return (double*)tab;
      break;
    case SCI_RECTANGLE:
      *numrow = 1;
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 5: 4;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = pRECTANGLE_FEATURE (pthis)->x;
      tab[1] = pRECTANGLE_FEATURE (pthis)->y;
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  tab[2] = pRECTANGLE_FEATURE (pthis)->z;
	  tab[3] = pRECTANGLE_FEATURE (pthis)->width;
	  tab[4] = pRECTANGLE_FEATURE (pthis)->height;
	}
      else
	{
	  tab[2] = pRECTANGLE_FEATURE (pthis)->width;
	  tab[3] = pRECTANGLE_FEATURE (pthis)->height; 
	}
      return (double*)tab;
      break;
    case SCI_ARC:
      *numrow = 1;
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 7: 6;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = pARC_FEATURE (pthis)->x;
      tab[1] =  pARC_FEATURE (pthis)->y;
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  tab[2] = pARC_FEATURE (pthis)->z;
	  tab[3] = pARC_FEATURE (pthis)->width;
	  tab[4] = pARC_FEATURE (pthis)->height;
	  tab[5] = pARC_FEATURE (pthis)->alphabegin;
	  tab[6] = pARC_FEATURE (pthis)->alphaend;
	}
      else
	{
	  tab[2] = pARC_FEATURE (pthis)->width;
	  tab[3] = pARC_FEATURE (pthis)->height;
	  tab[4] = pARC_FEATURE (pthis)->alphabegin;
	  tab[5] = pARC_FEATURE (pthis)->alphaend;
 
	}
      return (double*)tab;
      break;
    case SCI_AGREG:
      /*      *numrow = 2;
      *numcol = 2;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = pAGREG_FEATURE (pthis)->xmin
      tab[1] = pAGREG_FEATURE (pthis)->ymin
      tab[2] = pAGREG_FEATURE (pthis)->xmax - pAGREG_FEATURE (pthis)->xmin
      tab[3] = pAGREG_FEATURE (pthis)->ymax - pAGREG_FEATURE (pthis)->ymin
      return (double*)tab;*/
      return (double*)NULL;
      break;
    case SCI_TEXT:
      *numrow = 1;
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 3: 2;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = pTEXT_FEATURE (pthis)->x;
      tab[1] = pTEXT_FEATURE (pthis)->y;
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	tab[2] =  pTEXT_FEATURE (pthis)->z;
      return (double*)tab;
      break;
    case SCI_SBV:
      return (double*)NULL;
      break;
    case SCI_SBH:
      return (double*)NULL;			/* les coordonnees sont (x,0) */
    
    case SCI_SEGS:
      if (pSEGS_FEATURE (pthis)->ptype == 0) {
	*numrow = pSEGS_FEATURE (pthis)->Nbr1;
        
        /* only two coordinates are displayed if the axe is in 2d
           and the z coordinates has never been modified */
        if (   pSEGS_FEATURE(pthis)->vz != NULL
            || pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d )
        {
          *numcol = 3 ;
        }
        else
        {
          *numcol = 2 ;
        }
        /**numcol = ( pSEGS_FEATURE (pthis)->vz != NULL ? 3 : 2 ) ;*/
	if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
        {
	  return (double*)NULL;
        }
	for ( i = 0 ; i < *numrow ; i++ )
        {
          tab[i] = pSEGS_FEATURE (pthis)->vx[i];	
          tab[*numrow+i]= pSEGS_FEATURE (pthis)->vy[i];
          if ( *numcol == 3 )
          {
            if ( pSEGS_FEATURE (pthis)->vz == NULL )
            {
              /* default value */
              tab[2*(*numrow)+i] = 0.0 ;
            }
            else
            {
              tab[2*(*numrow)+i]= pSEGS_FEATURE (pthis)->vz[i];
            }
          }
        }
      }
      else {
	sciprint("Impossible case: champ object is now treated as a tlist. See set/getchampdata\r\n");
	return (double *) NULL;
      }
      return (double*)tab;
      break;
    case SCI_SURFACE:
      /* F.Leray 17.03.04*/
      sciprint("Impossible case happened ! Check if instruction in matdes.c inside gget function");
      return (double*) NULL;
      break;
    case SCI_GRAYPLOT:
      if (pGRAYPLOT_FEATURE (pthis)->type == 0) { /* gray plot */
	int ny=pGRAYPLOT_FEATURE (pthis)->ny,nx=pGRAYPLOT_FEATURE (pthis)->nx;
	*numrow = nx+1;
	*numcol = ny+1;
	if ((tab = CALLOC(*numrow * *numcol,sizeof(double))) == NULL)
	  return (double*)NULL;
	tab[0]=0;
	for (i=0;i < nx;i++) 
	  tab[i+1] = pGRAYPLOT_FEATURE (pthis)->pvecx[i];
	for (i=0;i < ny;i++) 
	  tab[*numrow*(i+1)] = pGRAYPLOT_FEATURE (pthis)->pvecy[i];

	for (i=0;i < ny;i++) 
	  for (k=0;k < nx;k++) 
	    tab[*numrow*(i+1)+k+1] = pGRAYPLOT_FEATURE (pthis)->pvecz[nx*i+k];
      }
      else  {/* Matplot */
	int ny=pGRAYPLOT_FEATURE (pthis)->ny-1,nx=pGRAYPLOT_FEATURE (pthis)->nx-1;
	*numrow = nx;	*numcol = ny;
	if ((tab = CALLOC(nx*ny,sizeof(double))) == NULL)
	  return (double*)NULL;
	for (i=0;i < nx*ny;i++) 
	  tab[i] = pGRAYPLOT_FEATURE (pthis)->pvecz[i];
      }
      return (double*)tab;
      break;
    case SCI_FEC: 
      *numcol = 3;
      *numrow = pFEC_FEATURE (pthis)->Nnode;
      if ((tab = CALLOC(*numrow * 3,sizeof(double))) == NULL)
	return (double*)NULL;

      for (i=0;i < *numrow;i++) {
	tab[i] = pFEC_FEATURE (pthis)->pvecx[i];
	tab[*numrow+i] = pFEC_FEATURE (pthis)->pvecy[i];
	tab[*numrow*2+i] = pFEC_FEATURE (pthis)->pfun[i];
      }
      return (double*)tab;
      break;
    case SCI_TITLE: 
    case SCI_LEGEND:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      sciprint ("This object has no points Y\n");
      return (double*)NULL;
      break;
    }
  return (double*)NULL;
}



/**sciGetObjClicked
 * @memo returns the sons' entity wich is cliked on x,y coordinates. If none, then returns this entity
 */
sciPointObj 
*sciGetObjClicked (sciPointObj *pthis,int x, int y)
{
  sciSons *sons;
  sciPointObj *result;

  /* 06/11/2002 */
  if (sciGetEntityType(pthis) != SCI_AGREG)/* We stop the recursivity at Compound */
    {
      sons = sciGetSons (pthis);
      while ((sons != (sciSons *) NULL) && (sons->pointobj != (sciPointObj *) NULL))
	{
	  result = sciGetObjClicked (sons->pointobj, x, y);

	  if (result == (sciPointObj *)NULL)
	    sons = sons->pnext;
	  else return result;
	}
    }
  if (sciIsClicked(pthis, x, y))
    return (sciPointObj *)pthis;
  else return (sciPointObj *)NULL;
  return (sciPointObj *)NULL;
}



/**sciIsClicked
 * @memo returns true if this entity wich is cliked on x,y coordinates. Here is the algorithm for click detection
 */
/* 06/11/2002 */
BOOL 
sciIsClicked(sciPointObj *pthis,int x, int y)
{
  double xtmp, ytmp, xtmp2, ytmp2;
  double DELTAX, DELTAY, vect[4];
  int n, xm[4];
  double *tab;
  int numrow, numcol, i;

  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      xtmp = (sciGetWidth (pthis)*(pSUBWIN_FEATURE (pthis)->WRect[0]));
      xtmp2 = xtmp + (sciGetWidth (pthis)*(pSUBWIN_FEATURE (pthis)->WRect[2]));
      ytmp = (sciGetHeight(pthis)*(pSUBWIN_FEATURE (pthis)->WRect[1]));
      ytmp2 = ytmp + (sciGetHeight(pthis)*(pSUBWIN_FEATURE (pthis)->WRect[3]));
      if (
	  (x >= xtmp)
	  && (y >= ytmp)
	  && (x <= xtmp2)
	  && (y <= ytmp2)
	  )
	{
	  /*set_scale ("tttfff", pSUBWIN_FEATURE (pthis)->WRect, 
	    pSUBWIN_FEATURE (pthis)->FRect, NULL, "nn", NULL);*/
	  set_scale ("tttftf", pSUBWIN_FEATURE (pthis)->WRect, 
		     pSUBWIN_FEATURE (pthis)->FRect, NULL, "nn", NULL);
	  return TRUE;
	}
      else
	return FALSE;
      break;
    case SCI_ARC:
      /* on recupere la dimension de la sous fenetre parente */
      /*set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
	pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);*/
      set_scale ("tttftf", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      n = 4;
      vect[0] = pARC_FEATURE (pthis)->x;
      vect[1] = pARC_FEATURE (pthis)->y;
      vect[2] = pARC_FEATURE (pthis)->width;
      vect[3] = pARC_FEATURE (pthis)->height;
      C2F(rect2d)(vect, xm, &n,"f2i");
      if (
	  ((abs(x - xm[0]) < DELTAX) && ((y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY))
	  )
	{
	  return TRUE;
	}
      return FALSE;
      break;
    case SCI_RECTANGLE:
      /* on recupere la dimension de la sous fenetre parente */
      set_scale ("tttftf", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      n=4;
      vect[0] = pRECTANGLE_FEATURE (pthis)->x;
      vect[1] = pRECTANGLE_FEATURE (pthis)->y;
      vect[2] = pRECTANGLE_FEATURE (pthis)->width;
      vect[3] = pRECTANGLE_FEATURE (pthis)->height;
      C2F(rect2d)(vect, xm, &n,"f2i");
      if (
	  ((abs(x - xm[0]) < DELTAX) && ((y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY))
	  )
	{
	  return TRUE;
	}
      return FALSE;
      break;
    case SCI_POLYLINE:
      set_scale ("tttftf", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      tab = sciGetPoint(pthis, &numrow, &numcol);
      i   = 0;
      while((i < numrow) && 
	    ((abs(XDouble2Pixel(tab[2*i]) - x) > DELTAX) || 
	     (abs(YDouble2Pixel(tab[2*i+1]) - y) > DELTAY)))
	{
	  i++;
	}
      if ((i < numrow)   && (abs(XDouble2Pixel(tab[2*i]) - x) < DELTAX) && 
	  (abs(YDouble2Pixel(tab[2*i+1]) - y) < DELTAY))
	{
	  FREE(tab);
	  return TRUE;
	}
      FREE(tab);
      return FALSE;
      break;
    case SCI_AGREG:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_UIMENU:
    default:
      return FALSE;
      break;
    }
  return FALSE;
}


BOOL
sciGetdrawmode (sciPointObj *pobj)
{
  static sciPointObj *subwin;

  subwin= (sciPointObj *) sciGetParentSubwin (pobj);
  if  (subwin != (sciPointObj *) NULL) 
    return pSUBWIN_FEATURE(subwin)->visible ;
  return FALSE;
}

/**sciGetAxes
 * @memo Gets SUBWIN of this Scilab graphic figure
 * @author Djalel ABDEMOUCHE
 */
sciPointObj *
sciGetAxes (pparentfigure,psubwin)
     sciPointObj *pparentfigure;
     sciPointObj *psubwin;
{
  sciSons *psonstmp; 
  
  psonstmp = sciGetSons (pparentfigure);
  
  
  if (psonstmp != (sciSons *) NULL)	
    {  
      /* tant que le fils ne corespond pas a l'entite */
      while ((psonstmp->pnext  != (sciSons *) NULL)
	     && (sciGetEntityType (psonstmp->pointobj) != SCI_SUBWIN))
	psonstmp = psonstmp->pnext;
      
      if  (psonstmp->pnext  == (sciSons *) NULL)
	return (sciPointObj *) NULL;
      else 
	if (sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	  return (sciPointObj *)psonstmp->pointobj;
	else
	  return (sciPointObj *) NULL; /* added SS ????*/
    }
  else
    return (sciPointObj *) NULL;
} 

/**sciGetPointerToToUserData
 * @memo Returns the pointer to the user_data and size_of_user_data fields associated with 
   the pobj object 
 */
void sciGetPointerToUserData (sciPointObj * pobj,int ***user_data_ptr, int **size_ptr)
{

  /* perhaps is it necessary to cast the returns value with void* type ! */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      *user_data_ptr = &(((sciFigure *) pFIGURE_FEATURE (pobj))->user_data);
      *size_ptr=&(((sciFigure *) pFIGURE_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_SUBWIN:
      *user_data_ptr = &(((sciSubWindow *) pSUBWIN_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciSubWindow *) pSUBWIN_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_TEXT:
      *user_data_ptr = &(((sciText *) pTEXT_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciText *) pTEXT_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_TITLE:
      *user_data_ptr = (int **)NULL;
      *size_ptr =  (int *)NULL;
      break;
    case SCI_LEGEND:
      *user_data_ptr = &(((sciLegend *) pLEGEND_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciLegend *) pLEGEND_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_ARC:
      *user_data_ptr = &(((sciArc *) pARC_FEATURE (pobj))->user_data);
      *size_ptr = &(((sciArc *) pARC_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_POLYLINE:
      *user_data_ptr = &(((sciPolyline *) pPOLYLINE_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciPolyline *) pPOLYLINE_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_SEGS:  
      *user_data_ptr = &(((sciSegs *) pSEGS_FEATURE (pobj))->user_data); 
	     *size_ptr = &(((sciSegs *) pSEGS_FEATURE (pobj))->size_of_user_data );
      break;
    case SCI_FEC:
      *user_data_ptr = &(((sciFec *) pFEC_FEATURE (pobj))->user_data); 
      *size_ptr =  &(((sciFec *) pFEC_FEATURE (pobj))->size_of_user_data) ;
      break;
    case SCI_GRAYPLOT:
      *user_data_ptr = &(((sciGrayplot *) pGRAYPLOT_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciGrayplot *) pGRAYPLOT_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_RECTANGLE:
      *user_data_ptr = &(((sciRectangle *) pRECTANGLE_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciRectangle *) pRECTANGLE_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_SURFACE:
      *user_data_ptr = &(((sciSurface *) pSURFACE_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciSurface *) pSURFACE_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_LIGHT:
      *user_data_ptr = &(((sciLightSource *) pLIGHT_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciLightSource *) pLIGHT_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_AXES:
      *user_data_ptr = &(((sciAxes *) pAXES_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciAxes *) pAXES_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_PANNER:
      *user_data_ptr = (int **)NULL;
      *size_ptr =  (int *)NULL;
      break;
    case SCI_SBH:
      *user_data_ptr = (int **)NULL;
      *size_ptr =  (int *)NULL;
      break;
    case SCI_SBV:
      *user_data_ptr = (int **)NULL;
      *size_ptr =  (int *)NULL;
      break;
    case SCI_MENU:
      *user_data_ptr = (int **)NULL;
      *size_ptr =  (int *)NULL;
      break;
    case SCI_MENUCONTEXT:
      *user_data_ptr = (int **)NULL;
      *size_ptr =  (int *)NULL;
      break;
    case SCI_STATUSB:
      *user_data_ptr = (int **)NULL;
      *size_ptr =  (int *)NULL;
      break;
    case SCI_AGREG:
      *user_data_ptr = &(((sciAgreg *) pAGREG_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciAgreg *) pAGREG_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_MERGE:
      *user_data_ptr = &(((sciMerge *) pMERGE_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciMerge *) pMERGE_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_LABEL:
      *user_data_ptr = &(((sciLabel *) pLABEL_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciLabel *) pLABEL_FEATURE (pobj))->size_of_user_data);
      break;

	case SCI_UIMENU:
    default:
      *user_data_ptr = (int **)NULL;
      *size_ptr =   (int *)NULL;
      break;
    }
}



int sciType (marker, pobj)
     char *marker;
     sciPointObj * pobj;
{ 
  if      (strcmp(marker,"arrow_size_factor") == 0) { return 1;}
  else if (strcmp(marker,"x_shift") == 0) { return 1;}
  else if (strcmp(marker,"y_shift") == 0) { return 1;}
  else if (strcmp(marker,"z_shift") == 0) { return 1;}
  else if (strcmp(marker,"bar_width") == 0) { return 1;}
  else if (strcmp(marker,"closed") == 0) { return 10;}	
  else if (strcmp(marker,"label") == 0) { return 10;}
  else if (strcmp(marker,"callback") == 0) { return 10;}
  else if (strcmp(marker,"closed") == 0) { return 10;}	
  else if (strcmp(marker,"background") == 0) { return 1;}	
  else if (strcmp(marker,"position") == 0) {return 1;}
  else if (strcmp(marker,"auto_position") == 0)   {return 10;}		
  else if (strcmp(marker,"auto_rotation") == 0)   {return 10;}		
  else if (strcmp(marker,"interp_color_vector") == 0) {return 1;}
  else if (strcmp(marker,"interp_color_mode") == 0) {return 10;}
  else if (strcmp(marker,"foreground") == 0) {return 1;}	
  else if (strcmp(marker,"thickness") == 0)   {return 1;}
  else if (strcmp(marker,"line_style") == 0) {return 1;}
  else if (strcmp(marker,"line_mode") == 0) {return 10;}
  else if (strcmp(marker,"fill_mode") == 0) {return 10;}
  else if (strcmp(marker,"surface_mode") == 0) {return 10;}
  else if (strcmp(marker,"mark_style") == 0) {return 1;}	
  else if (strcmp(marker,"mark_size") == 0) {return 1;}
  else if (strcmp(marker,"mark_size_unit") == 0) {return 10;}
  else if (strcmp(marker,"mark_mode") == 0)   {return 10;}
  else if (strcmp(marker,"mark_foreground") == 0)   {return 1;}
  else if (strcmp(marker,"mark_background") == 0)   {return 1;}
  else if (strcmp(marker,"figure_position") == 0) {return 1;}	 
  else if (strcmp(marker,"axes_size") == 0)   {return 1;}
  else if (strcmp(marker,"axes_visible") == 0)   {return 10;}
  else if (strcmp(marker,"hiddencolor") == 0)   {return 1;}/* DJ.A 2003 */
  else if (strcmp(marker,"isoview") == 0)   {return 10;}/**DJ.Abdemouche 2003**/
  else if (strcmp(marker,"view") == 0)   {return 10;}/**DJ.Abdemouche 2003**/	
  else if (strcmp(marker,"figure_size") == 0){return 1;}	
  else if (strcmp(marker,"figure_id") == 0)   {return 1;}	
  else if (strcmp(marker,"figure_name") == 0){return 10;}   
  else if (strcmp(marker,"figures_id") == 0)   {return 1;}
  else if (strcmp(marker,"pixmap") == 0)   {return 10;}/*Ajout A.Djalel*/
  else if (strcmp(marker,"polyline_style") == 0){return 1;} 
  else if (strcmp(marker,"font_size") == 0)   {return 1;}	
  else if (strcmp(marker,"font_angle") == 0) {return 1;}		
  else if (strcmp(marker,"font_foreground") == 0){return 1;}
  else if (strcmp(marker,"font_color") == 0)   {return 1;} /* F.Leray 09.04.04 : Adding to support font_color user interface */
  else if (strcmp(marker,"font_style") == 0) {return 1;}	      
  else if (strcmp(marker,"font_name") == 0)   {return 10;}
  else if (strcmp(marker,"textcolor") == 0)   {return 1;}
  else if (strcmp(marker,"labels_font_size") == 0)   {return 1;}
  else if (strcmp(marker,"labels_font_color") == 0)   {return 1;}
  else if (strcmp(marker,"text") == 0)        {return 10;}	 
  else if (strcmp(marker,"text_box") == 0)     {return 1;}	
  else if (strcmp(marker,"text_box_mode") == 0){return 10;}	
  else if (strcmp(marker,"old_style") == 0)   {return 10;}
  else if (strcmp(marker,"figure_style") == 0)   {return 10;}        
  else if (strcmp(marker,"visible") == 0)     {return 10;} 
  else if (strcmp(marker,"auto_resize") == 0){return 10;}
  else if (strcmp(marker,"pixel_drawing_mode") == 0) {return 10;}    
  else if (strcmp(marker,"default_values"    ) == 0) {return 1 ;} 
  else if (strcmp(marker,"color_map"         ) == 0) {return 1 ;}    
  else if (strcmp(marker,"x_location"        ) == 0) {return 10;} 
  else if (strcmp(marker,"y_location"        ) == 0) {return 10;}   
  else if (strcmp(marker,"tics_direction"    ) == 0) {return 10;}   
  else if (strcmp(marker,"tight_limits"      ) == 0) {return 10;} 
  else if (strcmp(marker,"box"               ) == 0) {return 10;}
  else if (strcmp(marker,"tics_color"        ) == 0) {return 1 ;}	 
  else if (strcmp(marker,"tics_textcolor"    ) == 0) {return 1 ;}	  
  else if (strcmp(marker,"tics_textsize"     ) == 0) {return 1 ;}
  else if (strcmp(marker,"xtics_coord"       ) == 0) {return 1 ;}	
  else if (strcmp(marker,"ytics_coord"       ) == 0) {return 1 ;}	 
  else if (strcmp(marker,"grid"              ) == 0) {return 1 ;}   
  else if (strcmp(marker,"tics_segment"      ) == 0) {return 10;} 
  else if (strcmp(marker,"tics_style"        ) == 0) {return 10;} 
  else if (strcmp(marker,"format_n"          ) == 0) {return 10;}    
  else if (strcmp(marker,"tics_labels"       ) == 0) {return 10;}  
  else if (strcmp(marker,"sub_tics"          ) == 0) {return 1 ;}
  else if (strcmp(marker,"sub_ticks"         ) == 0) {return 1 ;} /* new writing F.Leray 12.10.04 to be consistent with x,y,z _ticks*/
  else if (strcmp(marker,"zoom_box"          ) == 0) {return 1 ;}	
  else if (strcmp(marker,"zoom_state"        ) == 0) {return 10;}  
  else if (strcmp(marker,"clip_box"          ) == 0) {return 1 ;}	
  else if (strcmp(marker,"clip_state"        ) == 0) {return 10;} 
  else if (strcmp(marker,"auto_clear"        ) == 0) {return 10;}		
  else if (strcmp(marker,"auto_scale"        ) == 0) {return 10;}		  	 
  else if (strcmp(marker,"arrow_size"        ) == 0) {return 1 ;}
  else if (strcmp(marker,"segs_color"        ) == 0) {return 1 ;}
  else if (strcmp(marker,"colored"           ) == 0) {return 10;}
  else if (strcmp(marker,"data"              ) == 0)
    if((sciGetEntityType(pobj) == SCI_SURFACE) || 
       (sciGetEntityType(pobj) == SCI_SEGS     && pSEGS_FEATURE(pobj)->ptype == 1) || /* a champ */
       (sciGetEntityType(pobj) == SCI_GRAYPLOT && pGRAYPLOT_FEATURE(pobj)->type == 0))    /* a grayplot (case == 0) */
      return 16;
    else
      return 1;
  else if (strcmp(marker,"hdl"               ) == 0) {return 1 ;}		
  else if (strcmp(marker,"callbackmevent"    ) == 0) {return 1 ;}
  else if (strcmp(marker,"callback"          ) == 0) {return 10;} 	
  else if (strcmp(marker,"log_flags"         ) == 0) {return 10;}
  else if (strcmp(marker,"data_mapping"      ) == 0) {return 10;}
  else if (strcmp(marker,"surface_color"     ) == 0) {return 1 ;}
  else if (strcmp(marker,"rotation_angles"   ) == 0) {return 1 ;}
  else if (strcmp(marker,"color_mode"        ) == 0) {return 1 ;}/*DJ.A merge*/ 
  else if (strcmp(marker,"color_flag"        ) == 0) {return 1 ;}
  else if (strcmp(marker,"cdata_mapping"     ) == 0) {return 10;}
  else if (strcmp(marker,"axes_bounds"       ) == 0) {return 1 ;}
  else if (strcmp(marker,"data_bounds"       ) == 0) {return 1 ;}
  else if (strcmp(marker,"margins"           ) == 0) {return 1 ;}
  else if (strcmp(marker,"surface_color"     ) == 0) {return 1 ;}
  else if (strcmp(marker,"rotation_style"    ) == 0) {return 10;}
  else if (strcmp(marker,"triangles"         ) == 0) {return 1 ;}
  else if (strcmp(marker,"z_bounds"          ) == 0) {return 1 ;}
  else if (strcmp(marker,"current_axes"      ) == 0) {return 9 ;}
  else if (strcmp(marker,"current_figure"    ) == 0) {return 9 ;}
  else if (strcmp(marker,"current_obj"       ) == 0) {return 9 ;}
  else if (strcmp(marker,"current_entity"    ) == 0) {return 9 ;}
  else if (strcmp(marker,"default_axes"      ) == 0) {return 9 ;}/* DJ.A 08/01/04 */
  else if (strcmp(marker,"default_figure"    ) == 0) {return 9 ;}/* DJ.A 08/01/04 */
  else if (strcmp(marker,"children"          ) == 0) {return 9 ;}
  else if (strcmp(marker,"cube_scaling"      ) == 0) {return 10;} /* F.Leray 22.04.04 */
  else if (strcmp(marker,"x_label"           ) == 0) {return 9 ;}  /* F.Leray 27.05.04 */
  else if (strcmp(marker,"y_label"           ) == 0) {return 9 ;} 
  else if (strcmp(marker,"z_label"           ) == 0) {return 9 ;}
  else if (strcmp(marker,"title"             ) == 0) {return 9 ;} 
  else if (strcmp(marker,"x_ticks"           ) == 0) {return 16;} 
  else if (strcmp(marker,"y_ticks"           ) == 0) {return 16;} 
  else if (strcmp(marker,"z_ticks"           ) == 0) {return 16;} 
  else if (strcmp(marker,"auto_ticks"        ) == 0) {return 10;}
  else if (strcmp(marker,"axes_reverse"      ) == 0) {return 10;}
  else if (strcmp(marker,"immediate_drawing" ) == 0) {return 10;}
  else if (strcmp(marker,"handle_visible"    ) == 0) {return 10;}
  else if (strcmp(marker,"menu_enable"       ) == 0) {return 10;}
  else if (strcmp(marker,"callback_type"     ) == 0) {return 10;}
  else if (strcmp(marker,"bounding_rect"     ) == 0) {return 1 ;} /*JBS 16/11/05 */
  else {return -1;}
}

/**sciGetIdFigure
 */
void
sciGetIdFigure (int *vect, int *id, int *flag)
{
  sciHandleTab *hdl;
  sciPointObj  *pobj;
  
  hdl = PENDOFHANDLETAB;  
  *id=0;
  while (hdl != NULL)
    { 
      sciFigure * ppfigure = NULL; 
      pobj=(sciPointObj *) sciGetPointerFromHandle (hdl->index);
      ppfigure = pFIGURE_FEATURE(pobj);
      if (sciGetEntityType(pobj) == SCI_FIGURE)
	{
	  if (*flag) vect[*id] = sciGetNum(pobj);
	  (*id)++;
	} 
        
      hdl = hdl->pprev;
    }
}
int version_flag() 
{ 
  double *XGC,dv=0;
  struct BCG *CurrentScilabXgc = (struct BCG *) NULL;
  int v=0;

  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;
  /*  if (CurrentScilabXgc==(struct BCG *)NULL) return 1; */
  if (CurrentScilabXgc==(struct BCG *)NULL) 
    return versionflag; /* default versionflag is returned */
  
  return CurrentScilabXgc->graphicsversion;
}



sciPointObj *sciGetSurface(sciPointObj *psubwin)
{
  sciSons *psonstmp;
  
  psonstmp = sciGetSons (psubwin);
  while (psonstmp != (sciSons *) NULL)	
    {   
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SURFACE) 
	return (sciPointObj *) psonstmp->pointobj;
      psonstmp = psonstmp->pnext;
    }
  return (sciPointObj *) NULL;
}

/* DJ.A 2003 */
BOOL Check3DObjs(sciPointObj *pobj)
{  
  sciSons *psonstmp;

  psonstmp = sciGetSons (pobj); 
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) 
	{
	  if ((pobj= sciGetSurface(psonstmp->pointobj)) != (sciPointObj *) NULL)
	    return TRUE;
	}
      psonstmp = psonstmp->pnext;
    } 
  return FALSE;
}
sciPointObj *CheckClickedSubwin(integer x, integer y)
{ 
  integer box[4]; 
  sciSons *psonstmp;
  sciPointObj *psubwin; 
 
  psonstmp = sciGetSons (sciGetCurrentFigure());  
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) 
	{
	  psubwin= (sciPointObj *) psonstmp->pointobj;
	  sciSetSelectedSubWin(psubwin); 
	  box[0]= Cscale.WIRect1[0]; 
	  box[2]= Cscale.WIRect1[2]+Cscale.WIRect1[0];
	  box[1]= Cscale.WIRect1[1]; 
	  box[3]= Cscale.WIRect1[3]+Cscale.WIRect1[1];
	  if ((x >= box[0]) && (x <= box[2]) && (y >= box[1]) && (y <= box[3])) 
	    return (sciPointObj *) psubwin;                
	}
      psonstmp = psonstmp->pnext;
    } 
  return (sciPointObj *) NULL;                
}



/************************************ Compound *******************************************/
int CheckForCompound(long *handelsvalue, int number)
{
  sciPointObj *prevpparent;
  int i;
  long xtmp; 

  prevpparent = sciGetParent(sciGetPointerFromHandle((long) handelsvalue[0]));

  /* we verify if entities are all basic */
  for (i=0;i<number;i++)
    {
      xtmp =  handelsvalue[i];
       
      switch (sciGetEntityType(sciGetPointerFromHandle(xtmp)))
	{
	case SCI_ARC:
	case SCI_RECTANGLE:
	case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT: 
	case SCI_POLYLINE:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_SURFACE:
	case SCI_AXES:
	case SCI_LIGHT:
	  /* We verify  if the basics entities are not packed, and are all associated to a subwindow */
	  if (
	      (sciGetEntityType(sciGetParent(sciGetPointerFromHandle(xtmp))) != SCI_SUBWIN)
	      ||
	      (sciGetParent(sciGetPointerFromHandle(xtmp)) != prevpparent)
	      )
	    {
	      return i+1; /* invalid parent */
	    }
	  break;
	case SCI_AGREG:
	  /* We verify  if the Compound is packed, associated to a subwindow or have the same parent */
	  if (
	      (
	       (sciGetEntityType(sciGetParent(sciGetPointerFromHandle(xtmp))) != SCI_SUBWIN)
	       &&
	       (sciGetEntityType(sciGetParent(sciGetPointerFromHandle(xtmp))) != SCI_AGREG)
	       )
	      ||
	      (sciGetParent(sciGetPointerFromHandle(xtmp)) != prevpparent)
	      )
	    {
	      return i+1; /* invalid parent */
	    }
	  break;
	  /* we verify if the entity is not */
	case SCI_SUBWIN:
	case SCI_PANNER:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_FIGURE:
	case SCI_SBV:
	case SCI_SBH:
	case SCI_LABEL: /* F.Leray 28.05.04 A REVOIR...*/
	case SCI_UIMENU:
	default:
	  return -(i+1); /* not a basic entity*/
	}
      prevpparent = sciGetParent(sciGetPointerFromHandle(xtmp));
    }
  return 0;
}


sciPointObj *sciGetMerge(sciPointObj *psubwin)
{
  sciSons *psonstmp;
  
  psonstmp = sciGetSons (psubwin);
  while (psonstmp != (sciSons *) NULL)	
    {   
      if(sciGetEntityType (psonstmp->pointobj) == SCI_MERGE) 
	return (sciPointObj *) psonstmp->pointobj;
      psonstmp = psonstmp->pnext;
    }
  return (sciPointObj *) NULL;
}


/**sciGetOriginalSubWin
 * @memo PRIVATE.
 * @param sciPointObj * pfigure: the pointer to the parent figure
 * @return the pointer of the original subwindow
 */
sciPointObj *
sciGetOriginalSubWin (sciPointObj * pfigure)
{
  return (sciPointObj *)pFIGURE_FEATURE(pfigure)->originalsubwin0011;
}

/*sciGethPopMenu
 * This function returns the handle of the menu associated with this object and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
HMENU
sciGethPopMenu (sciPointObj * pthis)
{
  /* structure BCG periWIN ! winuser.h "Window"*/
  /*  HWND hwnd_window_pointed; */

  /* On cherche le handle de la fenetre selectionnnee */
  /* hwnd_window_pointed = WindowFromPoint(Point);    */
  switch(sciGetEntityType(pthis)) {
  case SCI_SUBWIN:
    return sciGethPopMenu(pSUBWIN_FEATURE (pthis)->pPopMenu);
    break;
  case SCI_MENUCONTEXT:
    return pMENUCONTEXT_FEATURE(pthis)->hPopMenu;
    break;
  case SCI_FIGURE:
  case SCI_TEXT:
  case SCI_TITLE:
  case SCI_LEGEND:
  case SCI_ARC:
  case SCI_SEGS: 
  case SCI_FEC: 
  case SCI_GRAYPLOT: 
  case SCI_POLYLINE:
  case SCI_RECTANGLE:
  case SCI_SURFACE: 
  case SCI_LIGHT:
  case SCI_AXES:
  case SCI_PANNER:
  case SCI_SBH:
  case SCI_SBV:
  case SCI_MENU:
  case SCI_STATUSB:
  case SCI_AGREG:
  case SCI_LABEL: /* F.Leray 28.05.04 */
  case SCI_UIMENU:
  default: 
    return (HMENU) NULL ;
    break;
  }
  return (HMENU) NULL;
}


/**sciGetIsBoxed
 * @memo Returns the box existence
 */
BOOL
sciGetIsBoxed (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE(pobj)->isboxed;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE(pobj)->axes.rect;
    case SCI_LABEL:
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_ARC:
    case SCI_FIGURE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_LEGEND:
    case SCI_SEGS:
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_TITLE:
	case SCI_UIMENU:
    default:
      sciprint ("This object has no isboxed \n");
      return 0;
      break;
    }
  return 0;
}

int *
sciGetInterpVector(sciPointObj * pobj)
{

  if(sciGetEntityType(pobj) != SCI_POLYLINE)
    return (int *) NULL;

  return pPOLYLINE_FEATURE(pobj)->scvector;
}


/**sciGetPosition
 * @memo Returns the position (in pixels) for the label object
 */
int
sciGetPosition (sciPointObj * pobj, double *x, double *y)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_LABEL:
      *x = pLABEL_FEATURE(pobj)->position[0];
      *y = pLABEL_FEATURE(pobj)->position[1];
      return 0;
      break;
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_ARC:
    case SCI_TEXT:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_FEC:
    case SCI_SEGS:
    case SCI_LEGEND:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:	/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TITLE:
	case SCI_UIMENU:
    default:
      sciprint ("This object has no position\n");
      return -1;
      break;
    }
  return 0;
}

BOOL sciGetAutoRotation ( sciPointObj * pObj )
{
   switch (sciGetEntityType (pObj))
    {
    case SCI_LABEL:
      return pLABEL_FEATURE(pObj)->auto_rotation ;
      break;
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_ARC:
    case SCI_TEXT:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_FEC:
    case SCI_SEGS:
    case SCI_LEGEND:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    case SCI_TITLE:
    case SCI_UIMENU:
    default:
      return FALSE;
      break;
    }
  return FALSE;
}

BOOL sciGetAutoPosition ( sciPointObj * pObj )
{
   switch (sciGetEntityType (pObj))
    {
    case SCI_LABEL:
      return pLABEL_FEATURE(pObj)->auto_position ;
      break;
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_ARC:
    case SCI_TEXT:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_FEC:
    case SCI_SEGS:
    case SCI_LEGEND:
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    case SCI_TITLE:
    case SCI_UIMENU:
    default:
      return FALSE;
      break;
    }
  return FALSE;
}

/*-------------------------------------------------------------------------------------------*/
/**
 * return if an object can be added to a merge object and then can be sorted for correct
 * drawing.
 * Normaly, each object shoul dbe mergeable. But it needs some development.
 */
BOOL sciIsMergeable( sciPointObj * pObj )
{
  switch (sciGetEntityType (pObj))
    {
    case SCI_SURFACE:
      return TRUE ;
    case SCI_POLYLINE:
      return TRUE ;
    case SCI_SEGS:
      return TRUE ;
    case SCI_RECTANGLE:
      return TRUE ;
    case SCI_MERGE:
      return TRUE ;
    default:
      return FALSE;
      break;
    }
  return FALSE;
}
/*-------------------------------------------------------------------------------------------*/
/**
 * return if the drawlater mode is on for the parent figure.
 */
BOOL sciGetIsAutoDrawable( sciPointObj * pobj )
{
  return pFIGURE_FEATURE(sciGetParentFigure(pobj))->auto_redraw ;
}
/*-------------------------------------------------------------------------------------------*/
/**
 * return the number of object of a certain type which can be found among the descendants
 * of an object.
 * To get the number of surfaces of a subwindow, it is much faster to use the
 * sciGetSubwinNbSurf funtion.
 */
int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type )
{
  int nbFound = 0 ;
  sciSons * curSon ;

  curSon = sciGetSons( pObj ) ;
  while( curSon != NULL )
  {
    if ( sciGetEntityType( curSon->pointobj ) == type )
    {
      nbFound++ ;
    }
    nbFound += sciGetNbTypedObjects( curSon->pointobj, type ) ;
    curSon = curSon->pnext ;
  }
  return nbFound ;
}
/*-------------------------------------------------------------------------------------------*/
