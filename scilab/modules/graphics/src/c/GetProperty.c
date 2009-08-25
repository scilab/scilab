/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2002 - 2004 - INRIA - Serge Steer
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
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
 *    This file contains all functions used to GET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/

#include "GetProperty.h"
#include "Scierror.h"
#include "InitObjects.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "GetJavaProperty.h"
#include "BasicAlgos.h"
#include "localization.h"
#include "Axes.h"
#include "stack-c.h"
#include "HandleManagement.h"

#include "MALLOC.h" /* MALLOC */



/**sciGetPointerToFeature
 * Returns the pointer to features structure from this object Used only for functions FREE or to use void pointer
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
    case SCI_AXES:
      return (sciAxes *) pAXES_FEATURE (pobj);
      break;
    case SCI_AGREG:
      return (sciAgreg *) pAGREG_FEATURE (pobj);
      break;
    case SCI_LABEL:
      return (sciLabel *) pLABEL_FEATURE (pobj);
      break;
    case SCI_UIMENU:
      return (sciUimenu *) pUIMENU_FEATURE (pobj);
      break;
    case SCI_UICONTROL:
      return (sciUicontrol *) pUICONTROL_FEATURE (pobj);
      break;
    default:
      return (void *) NULL;
      break;
    }
  return (void *) NULL;
}



/**sciGetEntityType
 * Gets the type of this Scilab graphic object return -1 if bad param !
 */
sciEntityType
sciGetEntityType (sciPointObj * pobj)
{
  if (pobj != (sciPointObj *) NULL)
    return pobj->entitytype;
  return (sciEntityType)-1;
}


/**sciGetCharEntityType
 * Gets the type of this Scilab graphic object in char format return -1 if bad param !
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
    case SCI_AXES:
      return "Axis";
      break;
    case SCI_AGREG:
      return "Compound";
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      return "Label";
      break;
    case SCI_UIMENU: 
      return "uimenu";
      break;
    case SCI_UICONTROL: 
      return "uicontrol";
      break;
    case SCI_WAITBAR:
      return "Waitbar" ;
      break ;
    case SCI_PROGRESSIONBAR:
      return "Progressionbar" ;
      break ;
    default:
      return (char *)NULL;
      break;
    }
  return (char *)NULL;
}

 
/**sciGetGraphicContext
 * Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
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
    case SCI_AXES:
      return  &(pAXES_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_LEGEND:
      return  &(pLEGEND_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_TEXT:
      return  &(pTEXT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_LABEL: /* F.Leray 28.05.04, modif JB.Silvy 03/06 */
      return sciGetGraphicContext( pLABEL_FEATURE(pobj)->text );
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      return (sciGraphicContext *) NULL;
      break;
    }
  return (sciGraphicContext *) NULL;
}


/**sciGetNumColors
 * This function gets the number of the color defined in colormap
 */
int
sciGetNumColors (sciPointObj * pobj)
{
  /* modified jb Silvy 06/2006 */
  switch (sciGetEntityType (pobj))
  {
  case SCI_FIGURE:
    return pFIGURE_FEATURE(pobj)->numcolors ;
  default:
    return sciGetNumColors( sciGetParentFigure( pobj ) ) ;
  }
  return -1 ;
}


/**sciGetColormap
 * This function gets a colormap from the figure. It's the same for all sons
 * Gets the colormap rgbmat must be a m x 3 double RGB matrix:  
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE 
 */
int sciGetColormap(sciPointObj * pobj, double rgbmat[] )
{
  if ( pobj == getFigureModel() )
  {
    doubleArrayCopy( rgbmat, pFIGURE_FEATURE(pobj)->pModelData->colorMap , pFIGURE_FEATURE(pobj)->pModelData->numColors ) ;
  }
  else
  {
    sciGetJavaColormap( pobj, rgbmat ) ;
  }
  
  return 0 ;
}

/* This function */
int sciGetGoodIndex(sciPointObj * pobj, int colorindex) /* return colorindex or m (Default Black) or m+1 (Default White)*/
{
  int m = sciGetNumColors (pobj);	/* the number of the color*/

  if(colorindex == -1) /* Black */
    return m + 1;
  else if(colorindex == -2) /* White */
    return m + 2;
  else
    return colorindex;
}



/**sciGetForeground
 * Gets the color number of the Foreground
 */
int
sciGetForeground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  if(sciGetGraphicContext(pobj) != NULL)
  {
    colorindex = (sciGetGraphicContext(pobj))->foregroundcolor + 1;
  }
  else
  {
    /*printSetGetErrorMessage("foreground");*/ /* rewrite updatebaw to renable this message */
    return -999;
  }

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}


int
sciGetForegroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);

  colorindex = sciGetForeground(pobj);
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
    
  return colorindex;
}


/**sciGetBackground
 * Gets the color number of the Background. Be carreful the return of the subwindow 
 * is the feature of its parent figure.
 */
int
sciGetBackground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  if(sciGetGraphicContext(pobj) != NULL)
  {
    colorindex = (sciGetGraphicContext(pobj))->backgroundcolor + 1;
  }
  else
  {
    /*printSetGetErrorMessage("background");*/ /* rewrite updatebaw to renable this message */
    return -999;
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
  
  colorindex = sciGetBackground(pobj);

  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}


/**sciGetMarkForeground
 * Gets the color number of the Marks'Foreground
 */
int
sciGetMarkForeground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  if (sciGetGraphicContext(pobj) != NULL)
  {
    colorindex = sciGetGraphicContext(pobj)->markforeground + 1;
  }
  else
  {
    printSetGetErrorMessage("mark_foreground");
    return -1;
  }

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}


int
sciGetMarkForegroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);

  colorindex = sciGetMarkForeground(pobj);
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
    
  return colorindex;
}



/**sciGetMarkBackground
 * Gets the color number of the Marks'Background. Be carreful the return of the subwindow 
 * is the feature of its parent figure.
 */
int
sciGetMarkBackground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  if (sciGetGraphicContext(pobj) != NULL)
  {
    colorindex = sciGetGraphicContext(pobj)->markbackground + 1;
  }
  else
  {
    printSetGetErrorMessage("mark_background");
    return -1;
  }
  
  return sciGetGoodIndex(pobj, colorindex);
  
}



/* F.Leray 01.04.04*/
/* Adding sciGet____groundToDisplay to display the correct index*/
int
sciGetMarkBackgroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  colorindex = sciGetMarkBackground(pobj);

  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}


/**sciGetLineWidth
 * Gets the line width
 */
double sciGetLineWidth (sciPointObj * pobj)
{
  if (sciGetGraphicContext(pobj) != NULL)
  {
    return sciGetGraphicContext(pobj)->linewidth;
  }
  printSetGetErrorMessage("thickness");
  return -1;
}


/**sciGetLineStyle
 * Gets the line style
 */
int
sciGetLineStyle (sciPointObj * pobj)
{
  if (sciGetGraphicContext(pobj) != NULL)
  {
    return sciGetGraphicContext(pobj)->linestyle;
  }
  printSetGetErrorMessage("line_style");
  return -1;
}


/**sciGetIsMark
 * Gets the line style
 */
BOOL
sciGetIsMark (sciPointObj * pobj)
{
  if(sciGetGraphicContext(pobj) != NULL)
  {
    return sciGetGraphicContext(pobj)->ismark;
  }
	printSetGetErrorMessage("mark_mode");
  return FALSE;
}


/**sciGetMarkStyle
 * Gets the mark style
 */
int
sciGetMarkStyle (sciPointObj * pobj)
{

  if (sciGetGraphicContext(pobj) != NULL)
  {
    return (sciGetGraphicContext(pobj))->markstyle;
  }

  printSetGetErrorMessage("mark_style");
  return -1;

}



/**sciGetMarkSize
 * Gets the mark size
 */
int
sciGetMarkSize (sciPointObj * pobj)
{

  if (sciGetGraphicContext(pobj) != NULL )
  {
    return sciGetGraphicContext(pobj)->marksize;
  }
  
  printSetGetErrorMessage("mark_size");
  return -1;
}


/**sciGetMarkSizeUnit
 * Gets the mark size unit
 * 1 : points, 2 : tabulated
 */
int
sciGetMarkSizeUnit (sciPointObj * pobj)
{
  if (sciGetGraphicContext(pobj) != NULL)
  {
    return (sciGetGraphicContext(pobj))->marksizeunit;
  }

  printSetGetErrorMessage("mark_size_unit");
  return -1;

}

/**sciGetIsLine
 * Returns the line drawing existence
 */
BOOL
sciGetIsLine (sciPointObj * pobj)
{

  if (sciGetGraphicContext(pobj) != NULL)
  {
    return sciGetGraphicContext(pobj)->isline;
  }
	else
	{
		printSetGetErrorMessage("line_mode");
		return FALSE;
	}
}

/**
 * @return TRUE if the object is actually displaying lines.
 */
BOOL sciGetIsDisplayingLines(sciPointObj * pObj)
{
	return sciGetIsLine(pObj) && (sciGetLineWidth(pObj) > 0.0);
}

/**sciGetIsFilled
 * Returns the filled line existence
 */
BOOL
sciGetIsFilled (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pobj)->axes.filled;
  default:
    if (sciGetGraphicContext(pobj) != NULL)
    {
      return sciGetGraphicContext(pobj)->isfilled;
    }
		else
		{
			printSetGetErrorMessage("fill_mode");
			return FALSE;
		}
  }
}



/**sciGetFontContext
 * Returns the structure of the Font Context. Do not use this in the Consturctor Functions !
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
    case SCI_LEGEND:
      return  &(pLEGEND_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
      return &(pSUBWIN_FEATURE (pobj)->axes.fontcontext);
      break;
    case SCI_AXES:
      return  &(pAXES_FEATURE (pobj)->fontcontext);
       break;  
    case SCI_FIGURE: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
      return &(pFIGURE_FEATURE (pobj)->fontcontext);
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      return sciGetFontContext( pLABEL_FEATURE(pobj)->text ) ;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      return (sciFont *)NULL;
      break;
    }
}



/****************************************** TEXT ******************************
 **sciGetFontSize
 * Gets the Font Size
 */
double sciGetFontSize(sciPointObj * pobj)
{
  if (sciGetFontContext(pobj) != NULL)
  {
    return (sciGetFontContext(pobj))->fontSize;
  }
  else
  {
    printSetGetErrorMessage("font_size");
    return -1.0;
  }
}


/*sciGetFontOrientation
 * gets the Font Orientation
 */
double
sciGetFontOrientation (sciPointObj * pobj)
{

  if (sciGetFontContext(pobj) != NULL)
  {
    return (sciGetFontContext(pobj))->textorientation;
  }
  printSetGetErrorMessage("font_angle");
  return 0.0;

}




/**sciGetText
 * Gets the Text in TEXT, TITLE or LEGEND. Be Carreful, 
 * this return a unterminated string (without \0)
 * To obtain the length of the text, use sciGetTextLength@param  sciPointObj *pobj
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  point to char[] if OK or NULL if not 
 */
StringMatrix * sciGetText( sciPointObj * pobj )
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->pStrings;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->text.pStrings;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return sciGetText( pLABEL_FEATURE (pobj)->text ) ;
      break;
    case SCI_UIMENU:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_AGREG:
    default:
      return  NULL;
      break;
    }
}

/**
 * Get the size of the string matrix of a text object
 * @param nbRow, number of row of the text conatianed in pobj
 *        If an error occures, the value is negative.
 * @param nbCol, same as nbRow for columns.
 */
void sciGetTextSize( sciPointObj * pobj, int * nbRow, int * nbCol )
{
  StringMatrix * text = sciGetText( pobj ) ;
  if ( text == NULL )
  {
    *nbRow = -1 ;
    *nbCol = -1 ;
  }
  else
  {
    *nbRow = getMatNbRow( text ) ;
    *nbCol = getMatNbCol( text ) ;
  }
}
/**
 * Checks if a text object is empty #rows*#columns==0 or #rows*#columns==1 and entry is  zero length
 */
BOOL sciisTextEmpty( sciPointObj * pobj)
{
  int nbElements;
  StringMatrix * text = sciGetText( pobj ) ;
  if ( text == NULL )
  {
    return TRUE;
  }
  nbElements = getMatNbRow(text) * getMatNbCol(text);
  if (nbElements == 0) {return TRUE;}
  if (nbElements == 1)
    {
    char * firstElement = getStrMatElement(text, 0, 0);
    if (firstElement == NULL)
    {
      return TRUE;
    }
    else if (firstElement[0] == 0)
    {
      /* empty string */
      return TRUE;
    }
  }
  return FALSE;
}


/**sciGetFontBackground
 * Gets the background color
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
    case SCI_LEGEND:
    case SCI_AXES:
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
    case SCI_LEGEND:
    case SCI_AXES:
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
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
       printSetGetErrorMessage("font_background");
      return -1;
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex; 
}



/**sciGetFontForeground
 * Gets the foreground color
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
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_SUBWIN:  /* F.Leray 08.04.04 */
    case SCI_FIGURE:  /* F.Leray 08.04.04 */
    case SCI_LABEL:   /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1 ; /* Modif. F.Leray 31.03.04*/
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("font_foreground");
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
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_SUBWIN:  /* F.Leray 08.04.04 */
    case SCI_FIGURE:  /* F.Leray 08.04.04 */
    case SCI_LABEL:   /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1 ; /* Modif. F.Leray 31.03.04*/
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
     printSetGetErrorMessage("font_foreground");
      return -1;
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}


/**sciGetFontStyle
 * Gets the font style 

 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 OK, -1 if not
 */
int
sciGetFontStyle (sciPointObj * pobj)
{


  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_SUBWIN:
    case SCI_FIGURE:
    case SCI_LABEL: /* F.Leray 10.06.04 */
    case SCI_AXES:
      return sciGetFontContext(pobj)->fonttype;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("font_style");
      return -1;
      break;
    }
}


/**sciGetLegendPlace
 * Returns the Title place with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return sciTitlePlace
 */
sciLegendPlace
sciGetLegendPlace (sciPointObj * pobj)
{
  if ( sciGetEntityType(pobj) == SCI_LEGEND )
  {
    return pLEGEND_FEATURE (pobj)->place ;
  }
  else
  {
    Scierror(999, _("You are not using a legend object.\n"));
    return SCI_LEGEND_OUT_LOWER_LEFT;
  }
  return SCI_LEGEND_OUT_LOWER_LEFT;
}



/**sciGetLegendPos
 * Returns the Title Position
 */

void sciGetLegendPos (sciPointObj * pobj, double position[2])
{
  POINT2D tmppoint;
  if (sciGetEntityType (pobj) == SCI_LEGEND) {
    tmppoint= pLEGEND_FEATURE (pobj)->pos;
    position[0] = tmppoint.x;
    position[1] = tmppoint.y;
  }

  else
    {
      position[0] = -1;
      position[1] = -1;
      Scierror(999, _("You are not using a legend object.\n"));
      return;
    }
}




/**sciGetParentFigure
 * Returns the the Figure parent
 */
sciPointObj *
sciGetParentFigure (sciPointObj * pobj)
{

  if (pobj == NULL)
  {
    return NULL;
  }

  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pobj ;
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_SUBWIN:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UICONTROL:
    case SCI_UIMENU:
      {
        return sciGetParentFigure( sciGetParent( pobj ) ) ; /* jbs 06/2006 */
      }  
      break;
    default:
      return NULL;
      break;
    }
  return NULL;
}
/** 19/09/2002 ***/
/**sciGetParentSubwin
 * Returns the the Subwin parent
 */
sciPointObj *
sciGetParentSubwin (sciPointObj * pobj)
{
  sciPointObj *subwin;

  subwin = pobj;
  switch (sciGetEntityType (pobj))
    { 
    case SCI_FIGURE:
      return sciGetFirstTypedSelectedSon( pobj, SCI_SUBWIN );
      break;
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
      while (sciGetEntityType(subwin) != SCI_SUBWIN)
	subwin=sciGetParent(subwin);      
      return (sciPointObj *) subwin;  
      break;                                                     
    default:
      return NULL;
      break;
    }
  return (sciPointObj *) NULL;
}

/**sciGetNumfigure
 * Returns the the Number of parent figure
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
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
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
 
/**sciGetGraphicMode
 * Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
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
    case SCI_AXES:
    case SCI_AGREG:
    case SCI_TEXT:
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
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_FEC:
      return pFEC_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_GRAYPLOT:
      return pGRAYPLOT_FEATURE (pobj)->clip_region_set;
      break;
    case SCI_LEGEND:
			return pLEGEND_FEATURE(pobj)->clip_region_set;
    case SCI_AGREG: 
    case SCI_FIGURE: 
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
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->isclip;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE(pobj)->isclip;
      break;
    case SCI_FEC:
      return pFEC_FEATURE(pobj)->isclip;
      break;
    case SCI_GRAYPLOT:
      return pGRAYPLOT_FEATURE(pobj)->isclip;
      break;
    case SCI_LEGEND:
			return pLEGEND_FEATURE(pobj)->isclip;
    case SCI_AGREG: 
    case SCI_FIGURE: 
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      return -2;
      break;
    } 
  return -2;
}


/**sciGetClipping
 * Gets the clipping area
 * 29/11/2002
 */
double *
sciGetClipping (sciPointObj * pobj)
{
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
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->clip_region;
      break;
    case SCI_LABEL:
      return sciGetClipping( pLABEL_FEATURE(pobj)->text ) ;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->clip_region;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE (pobj)->clip_region;
      break;
    case SCI_FEC:
      return pFEC_FEATURE (pobj)->clip_region;
      break;
    case SCI_GRAYPLOT:
      return pGRAYPLOT_FEATURE (pobj)->clip_region;
      break;
    case SCI_UIMENU:  
    case SCI_AGREG:
    case SCI_FIGURE: 
    default:
      printSetGetErrorMessage("clip_box");
      return (double *) NULL;
      break;
    }   
  printSetGetErrorMessage("clip_box");
  return (double *) NULL;

}


/**sciGetHighLight
 * Returns the highlight mode where the object are.
 */


/**sciGetAddPlot
 * Returns the mode of the adding plot
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
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("auto_clear");
      return FALSE;
      break;
    } 
  return FALSE;
}


/**sciGetAutoScale
 * Returns the mode of the autoscaling
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
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("auto_scale");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetZooming
 * Returns the zooming state of the object is it or not zoomed.
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
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:  
    case SCI_AGREG:
    case SCI_UIMENU:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      printSetGetErrorMessage("zoom");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetXorMode
 * Returns the drawing Xor mode
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
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:  
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("graphic_mode");
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

  if ( sciGetEntityType( pobj ) == SCI_FIGURE )
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
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->visible;
      break;    
    case SCI_AGREG: 
      return pAGREG_FEATURE (pobj)->visible;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return sciGetVisibility ( pLABEL_FEATURE (pobj)->text ) ;
      break;
    case SCI_UIMENU:
    default:
      return TRUE;
      break;
    }
}



/**sciGetResize (replace sciGetwresize()in the next version)
 * Returns if this object is in autoresizing mode (when the window is resized by user)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if no
 */
BOOL
sciGetResize (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (isFigureModel(pobj))
      {
        return pFIGURE_FEATURE(pobj)->pModelData->autoResizeMode;
      }
      else
      {
        return sciGetJavaAutoResizeMode(pobj);
      }
      break;
    case SCI_SUBWIN:
      /* the value is inhirated by the parent */
      return sciGetResize (sciGetParent (pobj));
      break;
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES: 
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("auto_resize");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetName
 * Returns the name of the Figure or SubWindow
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
      printSetGetErrorMessage("figure_name");
      return NULL;
      break;
    }
}

/**sciGetNum
 * Returns the number of the Figure.
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
      printSetGetErrorMessage("figure_id");
      return -1;
      break;
    }
}


/**sciGetWidth
 * Returns the width in pixel of the figure or subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the width of the dimension of the window or figure 
 * (the visibility dimension) in pixel dimension
 */
int sciGetWidth (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if ( pobj == getFigureModel() )
      {
        return pFIGURE_FEATURE(pobj)->pModelData->figureWidth;
      }
      else
      {
        int size[2] ;
        sciGetJavaFigureSize(pobj, size);
        return size[0];
      }      
      break;
    default:
      printSetGetErrorMessage("width");
      return -1;
      break;
    }
}


/**sciGetHeight
 * Returns the height in pixel of the figure or subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the height of the dimension of the window or figure (the visibility dimension) in pixel dimension
 */
int sciGetHeight (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if ( pobj == getFigureModel() )
      {
        return pFIGURE_FEATURE(pobj)->pModelData->figureHeight;
      }
      else
      {
        int size[2] ;
        sciGetJavaFigureSize(pobj, size);
        return size[1];
      }
      break;
    default:
      printSetGetErrorMessage("height");
      return -1;
      break;
    }
}

/**sciGetDim
 * Gets the dimension of the Figure or Subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int *pwidth: the return width of the window dimension
 * @param int *pheight: the return height of the window dimension
 */
void sciGetDim( sciPointObj * pobj, int * pWidth, int * pHeight )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if ( pobj == getFigureModel() )
      {
        *pWidth  = pFIGURE_FEATURE(pobj)->pModelData->figureWidth ;
        *pHeight = pFIGURE_FEATURE(pobj)->pModelData->figureHeight;
      }
      else
      {
        int size[2] ;
        sciGetJavaFigureSize(pobj, size);
        *pWidth = size[0] ;
        *pHeight = size[1] ;
      }
      break;
    default:
      printSetGetErrorMessage("size");
      break;
    }
}

/**
 * Get the size of the window enclosing a figure object
 */
int sciGetWindowWidth(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_FIGURE:
    if ( pObj == getFigureModel() )
    {
      return pFIGURE_FEATURE(pObj)->pModelData->windowWidth ;
    }
    else
    {
      int size[2] ;
      sciGetJavaWindowSize(pObj, size) ;
      return size[0] ;
    }
    break;
  default:
    printSetGetErrorMessage("figure_size");
    break;
  }
  return -1;
}


int sciGetWindowHeight(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_FIGURE:
    if ( pObj == getFigureModel() )
    {
      return pFIGURE_FEATURE(pObj)->pModelData->windowHeight ;
    }
    else
    {
      int size[2] ;
      sciGetJavaWindowSize(pObj, size) ;
      return size[1] ;
    }
    break;
  default:
    printSetGetErrorMessage("figure_size");
    break;
  }
  return -1;
}


/**sciIsExistingSubWin
 * Determines if this SubWindow is an existing one in the current SCI_FIGURE 
 * in association with the wrect and frect....
 * @param WRect: Window rectangle dimension in double, 
 * @param FRect: window user's scale dimension in double, 
 * @param logscale: flag for logarithmic window
 * @return the pointer to the existing SubWindow, or Null if no one is existing
 */
sciPointObj *
sciIsExistingSubWin (double WRect[4])
{
  sciPointObj *pparentfigure;
  sciSons *psonstmp;

  double WRectTmp[4];
  int i;


  /* Initialisation de WRectTmp a 0*/
  for(i=0;i<4;i++)
  {
    WRectTmp[i] = 0.0;
  }

  pparentfigure = (sciPointObj *)sciGetCurrentFigure();
  if (pparentfigure == NULL)
  {
    return NULL;
  }

  /**  15/03/2002 **/
  if ((WRect[0] == 0.)&&(WRect[1] == 0.)&&(WRect[2] == 1.)&&(WRect[3] == 1.))
  {
    /* return the last subwindow */
    sciSons * curSon = sciGetLastSons(pparentfigure);
    while (sciGetEntityType(curSon->pointobj) != SCI_SUBWIN)
    {
      curSon = curSon->pprev;
    }
    return curSon->pointobj;
  }

  psonstmp = sciGetSons (pparentfigure);
  while (psonstmp != NULL)
  {
    sciPointObj * curObj = psonstmp->pointobj;
    if (sciGetEntityType (curObj) == SCI_SUBWIN)
    {
      WRectTmp[0] = pSUBWIN_FEATURE (curObj)->WRect[0];
      WRectTmp[1] = pSUBWIN_FEATURE (curObj)->WRect[1];
      WRectTmp[2] = pSUBWIN_FEATURE (curObj)->WRect[2];
      WRectTmp[3] = pSUBWIN_FEATURE (curObj)->WRect[3];

      if (   (Abs(WRectTmp[0] - WRect[0]) < 1e-8)
          && (Abs(WRectTmp[1] - WRect[1]) < 1e-8)
          && (Abs(WRectTmp[2] - WRect[2]) < 1e-8)
          && (Abs(WRectTmp[3] - WRect[3]) < 1e-8))
      {
        /* subwin found */
        return curObj;
      }
    }
    psonstmp = psonstmp->pnext;
  }


  return NULL;
}

/*--------------------------------------------------------------------------*/


/**sciGetPoint
 * returns pointer to the points of the entity, and a pointer to the number of points. This function allocates memory for the tab of point, so after using the tab don't forget to free it
 */

/**MAJ pour le 3D DJ.Abdemouche 2003**/
double *sciGetPoint(sciPointObj * pthis, int *numrow, int *numcol)
{
  double *tab;
  int i,k;
  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:
      {
        int posX ;
        int posY ;
        *numrow = 2;
        *numcol = 2;
        if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
        {
          *numrow = -1;
          *numcol = -1;
	  return NULL;
        }
        sciGetScreenPosition( pthis, &posX, &posY ) ;
        tab[0] = (double) posX ;
        tab[1] = (double) posY ;
        tab[2] = (double)sciGetWidth (pthis);
        tab[3] = (double)sciGetHeight (pthis);
        return tab;
      }
    case SCI_POLYLINE:
      *numrow = pPOLYLINE_FEATURE (pthis)->n1;
      *numcol=(pPOLYLINE_FEATURE (pthis)->pvz != NULL)? 3:2;

      if ( (*numrow)*(*numcol) == 0 )
      {
        /* empty data, no warnings */
        *numrow = 0;
        *numcol = 0;
        return NULL ;
      }

      if(*numcol==2 && pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  *numcol = (*numcol)+1; /* colonne de 0. a prendre en compte / afficher => numcol+1*/
	  if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
          {
            *numrow = -1;
            *numcol = -1;
	    return NULL;
          }
	  for ( i = 0 ; i < *numrow ; i++ )
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
          *numrow = -1;
          *numcol = -1;
	  return NULL ;
        }
	for ( i = 0 ; i < *numrow ; i++ )
	{
	  tab[i] = pPOLYLINE_FEATURE (pthis)->pvx[i];	
	  tab[*numrow+i]= pPOLYLINE_FEATURE (pthis)->pvy[i];
	  if (*numcol== 3)
          {
	    tab[(2*(*numrow))+i] = pPOLYLINE_FEATURE (pthis)->pvz[i] ;
          }
	}
      }
      return tab;
      break;
    case SCI_RECTANGLE:
      *numrow = 1;
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 5: 4;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
      {
        *numrow = -1;
        *numcol = -1;
	return NULL;
      }
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
      {
        *numrow = -1;
        *numcol = -1;
	return NULL;
      }
      tab[0] = pARC_FEATURE (pthis)->x;
      tab[1] =  pARC_FEATURE (pthis)->y;
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  tab[2] = pARC_FEATURE (pthis)->z;
	  tab[3] = pARC_FEATURE (pthis)->width;
	  tab[4] = pARC_FEATURE (pthis)->height;
	  tab[5] = RAD2DEG(pARC_FEATURE (pthis)->alphabegin);
	  tab[6] = RAD2DEG(pARC_FEATURE (pthis)->alphaend);
	}
      else
	{
	  tab[2] = pARC_FEATURE (pthis)->width;
	  tab[3] = pARC_FEATURE (pthis)->height;
	  tab[4] = RAD2DEG(pARC_FEATURE (pthis)->alphabegin);
	  tab[5] = RAD2DEG(pARC_FEATURE (pthis)->alphaend);
 
	}
      return (double*)tab;
      break;
    case SCI_AGREG:
      return (double*)NULL;
      break;
    case SCI_TEXT:
      *numrow = 1;
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 3: 2;
      if ((tab = CALLOC((*numrow)*(*numcol),sizeof(double))) == NULL)
      {
        *numrow = -1;
        *numcol = -1;
	return NULL;
      }
      tab[0] = pTEXT_FEATURE (pthis)->x;
      tab[1] = pTEXT_FEATURE (pthis)->y;
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	tab[2] =  pTEXT_FEATURE (pthis)->z;
      return (double*)tab;
      break;
    
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
          *numrow = -1;
          *numcol = -1;
	  return NULL;
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
        *numrow = -1;
        *numcol = -1;
	return (double *) NULL;
      }
      return (double*)tab;
      break;
    case SCI_SURFACE:
      /* F.Leray 17.03.04*/
      *numrow = -1;
      *numcol = -1;
      return (double*) NULL;
      break;
    case SCI_GRAYPLOT:
      if (pGRAYPLOT_FEATURE (pthis)->type == 0) { /* gray plot */
	int ny=pGRAYPLOT_FEATURE (pthis)->ny,nx=pGRAYPLOT_FEATURE (pthis)->nx;
	*numrow = nx+1;
	*numcol = ny+1;
	if ((tab = CALLOC(*numrow * *numcol,sizeof(double))) == NULL)
        {
          *numrow = -1;
          *numcol = -1;
	  return (double*)NULL;
        }
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
        {
          *numrow = -1;
          *numcol = -1;
	  return (double*)NULL;
        }
        for (i=0;i < nx*ny;i++) 
	  tab[i] = pGRAYPLOT_FEATURE (pthis)->pvecz[i];
      }
      return (double*)tab;
      break;
    case SCI_FEC: 
      *numcol = 3;
      *numrow = pFEC_FEATURE (pthis)->Nnode;
      if ((tab = CALLOC(*numrow * 3,sizeof(double))) == NULL)
      {
        *numrow = -1;
        *numcol = -1;
	return (double*)NULL;
      }

      for (i=0;i < *numrow;i++) {
	tab[i] = pFEC_FEATURE (pthis)->pvecx[i];
	tab[*numrow+i] = pFEC_FEATURE (pthis)->pvecy[i];
	tab[*numrow*2+i] = pFEC_FEATURE (pthis)->pfun[i];
      }
      return (double*)tab;
      break;
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      *numrow = -2;
      *numcol = -2;
      return (double*)NULL;
      break;
    }
  return (double*)NULL;
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
 * Gets SUBWIN of this Scilab graphic figure
 * @author Djalel ABDEMOUCHE
 */
sciPointObj *
sciGetAxes (sciPointObj *pparentfigure,sciPointObj *psubwin)
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
 * Returns the pointer to the user_data and size_of_user_data fields associated with 
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
    case SCI_AXES:
      *user_data_ptr = &(((sciAxes *) pAXES_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciAxes *) pAXES_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_AGREG:
      *user_data_ptr = &(((sciAgreg *) pAGREG_FEATURE (pobj))->user_data);
      *size_ptr =  &(((sciAgreg *) pAGREG_FEATURE (pobj))->size_of_user_data);
      break;
    case SCI_LABEL:
      sciGetPointerToUserData ( pLABEL_FEATURE(pobj)->text, user_data_ptr, size_ptr ) ;
      break;
    case SCI_UIMENU:
      *user_data_ptr = &(pUIMENU_FEATURE (pobj)->user_data);
      *size_ptr=&(pUIMENU_FEATURE (pobj)->size_of_user_data);
      break;
    case SCI_UICONTROL:
      *user_data_ptr = &(pUICONTROL_FEATURE (pobj)->user_data);
      *size_ptr=&(pUICONTROL_FEATURE (pobj)->size_of_user_data);
      break;
    default:
      *user_data_ptr = (int **)NULL;
      *size_ptr =   (int *)NULL;
      break;
    }
}


/**
 * Don't use this ugly function !!!! (Jb Silvy)
 * 130 strcmp to know the type of a parameter
 */
int sciType (char *marker,sciPointObj * pobj)
{ 
  if      (strcmp(marker,"arrow_size_factor") == 0) { return sci_matrix;}
  else if (strcmp(marker,"x_shift") == 0) { return sci_matrix;}
  else if (strcmp(marker,"y_shift") == 0) { return sci_matrix;}
  else if (strcmp(marker,"z_shift") == 0) { return sci_matrix;}
  else if (strcmp(marker,"bar_width") == 0) { return sci_matrix;}
  else if (strcmp(marker,"closed") == 0) { return sci_strings;}
  else if (strcmp(marker,"label") == 0) { return sci_strings;}
  else if (strcmp(marker,"callback") == 0) { return sci_strings;}
  else if (strcmp(marker,"background") == 0) { return sci_matrix;}	
  else if (strcmp(marker,"position") == 0) {return sci_matrix;}
  else if (strcmp(marker,"auto_position") == 0)   {return sci_strings;}		
  else if (strcmp(marker,"auto_rotation") == 0)   {return sci_strings;}		
  else if (strcmp(marker,"interp_color_vector") == 0) {return sci_matrix;}
  else if (strcmp(marker,"interp_color_mode") == 0) {return sci_strings;}
  else if (strcmp(marker,"foreground") == 0) {return sci_matrix;}	
  else if (strcmp(marker,"thickness") == 0)   {return sci_matrix;}
  else if (strcmp(marker,"line_style") == 0) {return sci_matrix;}
  else if (strcmp(marker,"line_mode") == 0) {return sci_strings;}
  else if (strcmp(marker,"fill_mode") == 0) {return sci_strings;}
  else if (strcmp(marker,"surface_mode") == 0) {return sci_strings;}
  else if (strcmp(marker,"mark_style") == 0) {return sci_matrix;}	
  else if (strcmp(marker,"mark_size") == 0) {return sci_matrix;}
  else if (strcmp(marker,"mark_size_unit") == 0) {return sci_strings;}
  else if (strcmp(marker,"mark_mode") == 0)   {return sci_strings;}
  else if (strcmp(marker,"mark_foreground") == 0)   {return sci_matrix;}
  else if (strcmp(marker,"mark_background") == 0)   {return sci_matrix;}
  else if (strcmp(marker,"figure_position") == 0) {return sci_matrix;}	 
  else if (strcmp(marker,"axes_size") == 0)   {return sci_matrix;}
  else if (strcmp(marker,"axes_visible") == 0)   {return sci_strings;}
  else if (strcmp(marker,"hiddencolor") == 0)   {return 1;}/* DJ.A 2003 */
  else if (strcmp(marker,"isoview") == 0)   {return sci_strings;}/**DJ.Abdemouche 2003**/
  else if (strcmp(marker,"view") == 0)   {return sci_strings;}/**DJ.Abdemouche 2003**/	
  else if (strcmp(marker,"figure_size") == 0){return sci_matrix;}	
  else if (strcmp(marker,"figure_id") == 0)   {return sci_matrix;}	
  else if (strcmp(marker,"figure_name") == 0){return sci_strings;}   
  else if (strcmp(marker,"figures_id") == 0)   {return sci_matrix;}
  else if (strcmp(marker,"pixmap") == 0)   {return sci_strings;}/*Ajout A.Djalel*/
  else if (strcmp(marker,"polyline_style") == 0){return sci_matrix;} 
  else if (strcmp(marker,"font_size") == 0)   {return sci_matrix;}	
  else if (strcmp(marker,"font_angle") == 0) {return sci_matrix;}		
  else if (strcmp(marker,"font_foreground") == 0){return sci_matrix;}
  else if (strcmp(marker,"font_color") == 0)   {return sci_matrix;} /* F.Leray 09.04.04 : Adding to support font_color user interface */
  else if (strcmp(marker,"font_style") == 0) {return sci_matrix;}	      
  else if (strcmp(marker,"font_name") == 0)   {return sci_strings;}
  else if (strcmp(marker,"textcolor"          ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"labels_font_size"   ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"labels_font_color"  ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"text"               ) == 0) {return sci_strings;}	 
  else if (strcmp(marker,"text_box"           ) == 0) {return sci_matrix;}	
  else if (strcmp(marker,"text_box_mode"      ) == 0) {return sci_strings;}	
  else if (strcmp(marker,"old_style"          ) == 0) {return sci_strings;}
  else if (strcmp(marker,"figure_style"       ) == 0) {return sci_strings;}        
  else if (strcmp(marker,"visible"            ) == 0) {return sci_strings;} 
  else if (strcmp(marker,"auto_resize"        ) == 0) {return sci_strings;}
  else if (strcmp(marker,"pixel_drawing_mode" ) == 0) {return sci_strings;}    
  else if (strcmp(marker,"default_values"     ) == 0) {return sci_matrix ;} 
  else if (strcmp(marker,"color_map"          ) == 0) {return sci_matrix ;}    
  else if (strcmp(marker,"x_location"         ) == 0) {return sci_strings;} 
  else if (strcmp(marker,"y_location"         ) == 0) {return sci_strings;}   
  else if (strcmp(marker,"tics_direction"     ) == 0) {return sci_strings;}   
  else if (strcmp(marker,"tight_limits"       ) == 0) {return sci_strings;} 
  else if (strcmp(marker,"box"                ) == 0) {return sci_strings;}
  else if (strcmp(marker,"tics_color"         ) == 0) {return sci_matrix ;}	 
  else if (strcmp(marker,"tics_textcolor"     ) == 0) {return sci_matrix ;}	  
  else if (strcmp(marker,"tics_textsize"      ) == 0) {return sci_matrix ;}
  else if (strcmp(marker,"xtics_coord"        ) == 0) {return sci_matrix ;}	
  else if (strcmp(marker,"ytics_coord"        ) == 0) {return sci_matrix ;}	 
  else if (strcmp(marker,"grid"               ) == 0) {return sci_matrix ;}   
  else if (strcmp(marker,"tics_segment"       ) == 0) {return sci_strings;} 
  else if (strcmp(marker,"tics_style"         ) == 0) {return sci_strings;} 
  else if (strcmp(marker,"format_n"           ) == 0) {return sci_strings;}    
  else if (strcmp(marker,"tics_labels"        ) == 0) {return sci_strings;}  
  else if (strcmp(marker,"sub_tics"           ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"sub_ticks"          ) == 0) {return sci_matrix;} /* new writing F.Leray 12.10.04 to be consistent with x,y,z _ticks*/
  else if (strcmp(marker,"zoom_box"           ) == 0) {return sci_matrix;}	
  else if (strcmp(marker,"zoom_state"         ) == 0) {return sci_strings;}  
  else if (strcmp(marker,"clip_box"           ) == 0) {return sci_matrix;}	
  else if (strcmp(marker,"clip_state"         ) == 0) {return sci_strings;} 
  else if (strcmp(marker,"auto_clear"         ) == 0) {return sci_strings;}		
  else if (strcmp(marker,"auto_scale"         ) == 0) {return sci_strings;}		  	 
  else if (strcmp(marker,"arrow_size"         ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"segs_color"         ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"colored"            ) == 0) {return sci_strings;}
  else if (strcmp(marker,"data"               ) == 0)
    if((sciGetEntityType(pobj) == SCI_SURFACE) || 
       (sciGetEntityType(pobj) == SCI_SEGS     && pSEGS_FEATURE(pobj)->ptype == 1) || /* a champ */
       (sciGetEntityType(pobj) == SCI_GRAYPLOT && pGRAYPLOT_FEATURE(pobj)->type == 0))    /* a grayplot (case == 0) */
      return sci_tlist;
    else
      return 1;
  else if (strcmp(marker,"hdl"                ) == 0) {return sci_matrix;}		
  else if (strcmp(marker,"callbackmevent"     ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"callback"           ) == 0) {return sci_strings;} 	
  else if (strcmp(marker,"log_flags"          ) == 0) {return sci_strings;}
  else if (strcmp(marker,"data_mapping"       ) == 0) {return sci_strings;}
  else if (strcmp(marker,"surface_color"      ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"rotation_angles"    ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"color_mode"         ) == 0) {return sci_matrix;}/*DJ.A merge*/ 
  else if (strcmp(marker,"color_flag"         ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"cdata_mapping"      ) == 0) {return sci_strings;}
  else if (strcmp(marker,"axes_bounds"        ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"data_bounds"        ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"margins"            ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"surface_color"      ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"rotation_style"     ) == 0) {return sci_strings;}
  else if (strcmp(marker,"triangles"          ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"z_bounds"           ) == 0) {return sci_matrix;}
  else if (strcmp(marker,"current_axes"       ) == 0) {return sci_handles;}
  else if (strcmp(marker,"current_figure"     ) == 0) {return sci_handles;}
  else if (strcmp(marker,"current_obj"        ) == 0) {return sci_handles;}
  else if (strcmp(marker,"current_entity"     ) == 0) {return sci_handles;}
  else if (strcmp(marker,"default_axes"       ) == 0) {return sci_handles;}/* DJ.A 08/01/04 */
  else if (strcmp(marker,"default_figure"     ) == 0) {return sci_handles;}/* DJ.A 08/01/04 */
  else if (strcmp(marker,"children"           ) == 0) {return sci_handles;}
  else if (strcmp(marker,"cube_scaling"       ) == 0) {return sci_strings;} /* F.Leray 22.04.04 */
  else if (strcmp(marker,"x_label"            ) == 0) {return sci_handles;}  /* F.Leray 27.05.04 */
  else if (strcmp(marker,"y_label"            ) == 0) {return sci_handles;} 
  else if (strcmp(marker,"z_label"            ) == 0) {return sci_handles;}
  else if (strcmp(marker,"title"              ) == 0) {return sci_handles;} 
  else if (strcmp(marker,"x_ticks"            ) == 0) {return sci_tlist;} 
  else if (strcmp(marker,"y_ticks"            ) == 0) {return sci_tlist;} 
  else if (strcmp(marker,"z_ticks"            ) == 0) {return sci_tlist;} 
  else if (strcmp(marker,"auto_ticks"         ) == 0) {return sci_strings;}
  else if (strcmp(marker,"axes_reverse"       ) == 0) {return sci_strings;}
  else if (strcmp(marker,"immediate_drawing"  ) == 0) {return sci_strings;}
  else if (strcmp(marker,"handle_visible"     ) == 0) {return sci_strings;}
  else if (strcmp(marker,"enable"             ) == 0) {return sci_strings;}
  else if (strcmp(marker,"callback_type"      ) == 0) {return sci_strings;}
  else if (strcmp(marker,"bounding_rect"      ) == 0) {return sci_matrix;} /*JBS 16/11/05 */
  else if (strcmp(marker,"hidden_axis_color"  ) == 0) {return sci_matrix;} /*       03/06 */
  else if (strcmp(marker,"alignment"          ) == 0) {return sci_strings;} /*       05/06 */
  else if (strcmp(marker,"viewport"           ) == 0) {return sci_matrix;} /*       01/07 */
  else if (strcmp(marker,"info_message"       ) == 0) {return sci_strings;} /*       01/07 */
  else {return -1;}
}
/*-------------------------------------------------------------------------------*/
/**
 * Return the first surface found within the descendant of the object.
 * @param[in] psubwin Object from which the surface will be searched.
 * @return The first surface object if found, NULL otherwise.
 */
sciPointObj * sciGetSurface( sciPointObj * pObj )
{
  sciSons * psonstmp;
  sciPointObj * sonSurface = NULL ;
  
  psonstmp = sciGetSons( pObj ) ;
  while ( psonstmp != NULL )
  {
    if( sciGetEntityType( psonstmp->pointobj ) == SCI_SURFACE )
    {
      /* We found one, return it.*/
      return psonstmp->pointobj ;
    }
    /* check the sons of this children */
    sonSurface = sciGetSurface( psonstmp->pointobj ) ;
    if ( sonSurface != NULL )
    {
      return sonSurface ;
    }
    psonstmp = psonstmp->pnext;
  }
  /* nothing has been found */
  return NULL;
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
	case SCI_SURFACE:
	case SCI_AXES:
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
	case SCI_FIGURE:
	case SCI_LABEL: /* F.Leray 28.05.04 A REVOIR...*/
	case SCI_UIMENU:
	case SCI_LEGEND:
	default:
	  return -(i+1); /* not a basic entity*/
	}
      prevpparent = sciGetParent(sciGetPointerFromHandle(xtmp));
    }
  return 0;
}

/**sciGetOriginalSubWin
 * PRIVATE.
 * @param sciPointObj * pfigure: the pointer to the parent figure
 * @return the pointer of the original subwindow
 */
sciPointObj *
sciGetOriginalSubWin (sciPointObj * pfigure)
{
  return (sciPointObj *)pFIGURE_FEATURE(pfigure)->originalsubwin0011;
}


/* get the property of the axes box */
EAxesBoxType sciGetBoxType( sciPointObj * pobj )
{
  switch (sciGetEntityType (pobj))
  {
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE(pobj)->axes.rect ;
    default:
      printSetGetErrorMessage("box_type");
      return (EAxesBoxType)0 ;
  }
  return (EAxesBoxType)0;
}


/**sciGetIsBoxed
 * Returns the box existence
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
      return ( pSUBWIN_FEATURE(pobj)->axes.rect == BT_ON || pSUBWIN_FEATURE(pobj)->axes.rect == BT_BACK_HALF ) ;
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
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("box_type");
      return 0;
      break;
    }
  return 0;
}

int * sciGetInterpVector(sciPointObj * pobj)
{

  if(sciGetEntityType(pobj) != SCI_POLYLINE)
    return (int *) NULL;

  return pPOLYLINE_FEATURE(pobj)->scvector;
}

BOOL sciGetIsColorInterpolated(sciPointObj * pobj)
{
  switch ( sciGetEntityType (pobj) )
  {
  case SCI_POLYLINE:
    return pPOLYLINE_FEATURE(pobj)->isinterpshaded;
  default:
    printSetGetErrorMessage("interp_vector");
    break;
  }
  return FALSE;
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
    case SCI_AGREG:
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
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      return FALSE;
      break;
    }
  return FALSE;
}

/*-----------------------------------------------------------------------------------*/
BOOL sciGetLegendDefined( sciPointObj * pObj )
{
  sciSubWindow * ppSubWin ;
 
  if ( pObj == NULL )
  {
    return FALSE ;
  }

  ppSubWin = pSUBWIN_FEATURE( pObj ) ;

  /* get the text size of labels */
  if (sciisTextEmpty(ppSubWin->mon_x_label) && 
      sciisTextEmpty(ppSubWin->mon_y_label) && 
      sciisTextEmpty(ppSubWin->mon_z_label))
    return FALSE ;
  else
    return TRUE ;
 
}
/*-----------------------------------------------------------------------------------*/
BOOL sciGetAutoSize( sciPointObj * pObj )
{
  switch (sciGetEntityType (pObj))
  {
  case SCI_TEXT:
    return pTEXT_FEATURE(pObj)->autoSize ;
    break;
  default:
    return FALSE;
    break;
  }
  return FALSE;
}
/*-----------------------------------------------------------------------------------*/
sciTextAlignment sciGetAlignment( sciPointObj * pObj )
{
  switch (sciGetEntityType (pObj))
  {
  case SCI_TEXT:
    return pTEXT_FEATURE(pObj)->stringsAlign ;
    break;
  default:
    printSetGetErrorMessage("text_alignment");
    return ALIGN_NONE ;
  }
  return ALIGN_NONE ;
}
/*-----------------------------------------------------------------------------------*/
void sciGetUserSize( sciPointObj * pObj, double * width, double * height )
{
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_TEXT:
    *width  = pTEXT_FEATURE(pObj)->userSize[0] ;
    *height = pTEXT_FEATURE(pObj)->userSize[1] ;
    break ;
  default:
    printSetGetErrorMessage("text_box");
    break ;
  }
}
/*-----------------------------------------------------------------------------------*/
/**
 * get how is displayed the text around its position.
 * If TRUE, the position is the middle of the displayed string.
 * If FALSE, it is the lower right corner.
 */
BOOL sciGetCenterPos( sciPointObj * pObj )
{
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_TEXT:
    return pTEXT_FEATURE(pObj)->centeredPos ;
    break ;
  default:
    printSetGetErrorMessage("text_box_mode");
    return FALSE ;
  }
}
/*-----------------------------------------------------------------------------------*/
/**
 * return wether the current object is displayed in 2d or 3d mode.
 */
BOOL sciGetIs3d( sciPointObj * pObj )
{
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->is3d ;
  case SCI_TEXT:
    return pTEXT_FEATURE( pObj )->is3d ;
  case SCI_LABEL:
    return sciGetIs3d( pLABEL_FEATURE( pObj )->text ) ;
  default:
    printSetGetErrorMessage("view");
    return FALSE ;
  }
  return FALSE ;
}
/*-----------------------------------------------------------------------------------*/
/**
 * get the number of children of a graphic object
 * @return number of children of object pObj
 */
int sciGetNbChildren( sciPointObj * pObj )
{
  int nbChildren = 0 ;
  sciSons * curSon = sciGetSons( pObj ) ;
  while ( curSon != NULL && curSon->pointobj != NULL )
  {
    nbChildren++ ;
    curSon = curSon->pnext ;
  }
  return nbChildren ;
}
/*-----------------------------------------------------------------------------------*/
/**
 * Return if an object directly accessible as a son of an other in Scilab.
 * For instance, Label objects are not accessible.
 * @return TRUE if the object is accessible, FALSE otherwise.
 */
BOOL sciGetIsAccessibleChild( sciPointObj * pObj )
{
  return sciGetEntityType( pObj ) != SCI_LABEL
         && GetHandleVisibilityOnUimenu( pObj ) ;
}
/*-----------------------------------------------------------------------------------*/
/**
 * return the number of children of an object. This corresponds to the number of children
 * seen in the Scilab console.
 */
int sciGetNbAccessibleChildren( sciPointObj * pObj )
{
  int nbChildren = 0 ;
  sciSons * curSon = sciGetFirstAccessibleSon( pObj ) ;

  while ( curSon != NULL && curSon->pointobj != NULL )
  {
    nbChildren++ ;
    curSon = sciGetNextAccessibleSon( curSon ) ;
  }
  return nbChildren ;
}
/*-----------------------------------------------------------------------------------*/
BOOL GetHandleVisibilityOnUimenu( sciPointObj * pobj )
{
  if (sciGetEntityType(pobj)!=SCI_UIMENU) { return TRUE ; }
  
  return pUIMENU_FEATURE(pobj)->handle_visible;
}
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
/**
 * return the hidden color of a surface or the inherated attribute of a subwindow.
 */
int sciGetHiddenColor( sciPointObj * pObj )
{

  int colorIndex;
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_SUBWIN:
    colorIndex = pSUBWIN_FEATURE(pObj)->hiddencolor;
    break;
  case SCI_SURFACE:
    colorIndex = pSURFACE_FEATURE(pObj)->hiddencolor;
    break;
  default:
    printSetGetErrorMessage("hidden_color");
    return -10 ;
  }
  return colorIndex ;
}
/*----------------------------------------------------------------------------------*/
/**
 * return the color of the back trihedron of an subwin object.
 */
int sciGetHiddenAxisColor( sciPointObj * pObj )
{

  int colorIndex;
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_SUBWIN:
    colorIndex = pSUBWIN_FEATURE(pObj)->axes.hiddenAxisColor + 1 ;
    break;
  default:
    printSetGetErrorMessage("hidden_axis_color");
    return -10 ;
  }
  colorIndex = sciGetGoodIndex(pObj, colorIndex);
  return colorIndex ;
}
/*----------------------------------------------------------------------------------*/
/**
 * return if the drawlater mode is on for the parent figure.
 */
BOOL sciGetIsAutoDrawable( sciPointObj * pobj )
{
  return (sciGetImmediateDrawingMode(pobj) && !sciGetPixmapMode(pobj));
}
/*----------------------------------------------------------------------------------*/
BOOL sciGetImmediateDrawingMode(sciPointObj * pobj)
{
  return pFIGURE_FEATURE(sciGetParentFigure(pobj))->auto_redraw ;
}
/*----------------------------------------------------------------------------------*/
/**
 * To retrieve the type of grid for each dimension
 */
void sciGetGridStyle( sciPointObj * pObj, int * xStyle, int * yStyle, int * zStyle )
{
  switch ( sciGetEntityType (pObj) )
  {
  case SCI_SUBWIN:
    if (xStyle != NULL) {*xStyle = pSUBWIN_FEATURE(pObj)->grid[0];}
    if (yStyle != NULL) {*yStyle = pSUBWIN_FEATURE(pObj)->grid[1];}
    if (zStyle != NULL) {*zStyle = pSUBWIN_FEATURE(pObj)->grid[2];}
    break ;
  default:
    printSetGetErrorMessage("grid");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * To retrieve the viewport property
 * viewport property is only enable when the auto_resize property of the figure is
 * disable.
 */
void sciGetViewport( sciPointObj * pObj, int viewport[4] )
{
  switch ( sciGetEntityType(pObj) )
  {
  case SCI_FIGURE:
    if (isFigureModel(pObj))
    {
      viewport[0] = pFIGURE_FEATURE(pObj)->pModelData->viewport[0];
      viewport[1] = pFIGURE_FEATURE(pObj)->pModelData->viewport[1];
      viewport[2] = pFIGURE_FEATURE(pObj)->pModelData->viewport[2];
      viewport[3] = pFIGURE_FEATURE(pObj)->pModelData->viewport[3];
    }
    else
    {
      sciGetJavaViewport(pObj, viewport);
    }
    break;
  default:
    printSetGetErrorMessage("viewport");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
void sciGetInfoMessage( sciPointObj * pObj, char * infoMessage )
{
  switch ( sciGetEntityType(pObj) )
  {
  case SCI_FIGURE:
		if (isFigureModel(pObj))
		{
			strcpy(infoMessage, pFIGURE_FEATURE(pObj)->pModelData->infoMessage);
		}
		else
		{
			sciGetJavaInfoMessage(pObj, infoMessage);
		}
		break;
  default:
    printSetGetErrorMessage("info_message");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
int sciGetInfoMessageLength( sciPointObj * pObj )
{
  switch ( sciGetEntityType(pObj) )
  {
  case SCI_FIGURE:
		if (isFigureModel(pObj))
		{
			return (int) strlen( pFIGURE_FEATURE(pObj)->pModelData->infoMessage ) ;
		}
		else
		{
			return sciGetJavaInfoMessageLength(pObj);
		}
  default:
    printSetGetErrorMessage("info_message");
    return -1 ;
  }
  return -1 ;

}
/*----------------------------------------------------------------------------------*/
void sciGetScreenPosition( sciPointObj * pObj, int * posX, int * posY )
{
  switch ( sciGetEntityType(pObj) )
  {
  case SCI_FIGURE:
    if ( pObj == getFigureModel() )
    {
      *posX = pFIGURE_FEATURE(pObj)->pModelData->windowPosition[0] ;
      *posY = pFIGURE_FEATURE(pObj)->pModelData->windowPosition[1] ;
    }
    else
    {
      int pos[2];
      sciGetJavaWindowPosition(pObj, pos) ;
      *posX = pos[0] ;
      *posY = pos[1] ;
    }
    break ;
  default:
    printSetGetErrorMessage("figure_position");
    *posX = -1 ;
    *posY = -1 ;
  }
}
/*----------------------------------------------------------------------------------*/
BOOL sciGetIsEventHandlerEnable( sciPointObj * pObj )
{
  switch ( sciGetEntityType(pObj) )
  {
  case SCI_FIGURE:
    {
      return pFIGURE_FEATURE( pObj )->isEventHandlerEnable ;
    }
    break ;
  default:
    printSetGetErrorMessage("event_handler");
    return FALSE ;
  }
  return FALSE ;
}
/*----------------------------------------------------------------------------------*/
char * sciGetEventHandler( sciPointObj * pObj )
{
  switch ( sciGetEntityType(pObj) )
  {
  case SCI_FIGURE:
    {
      return pFIGURE_FEATURE(pObj)->eventHandler ;
    }
    break ;
  default:
    printSetGetErrorMessage("event_handler");
    return NULL ;
  }
  return NULL ;
}
/*----------------------------------------------------------------------------------*/
double * sciGetAxesBounds( sciPointObj * pObj )
{
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->WRect;
  default:
    printSetGetErrorMessage("axes_bounds");
    return NULL ;
  }
}
/*----------------------------------------------------------------------------------*/
double * sciGetMargins( sciPointObj * pObj )
{
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->ARect;
  default:
    printSetGetErrorMessage("margins");
    return NULL ;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * To get the computed data bounds of a subwin.
 * * @param bounds [Xmin,Xmax,Ymin,Ymax,Zmin,Zmax] vector.
 */
void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] )
{
  int i;
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    for ( i = 0 ; i < 6 ; i++ )
    {
      bounds[i] = pSUBWIN_FEATURE(pObj)->FRect[i] ;
    }
    return ;
  default:
    printSetGetErrorMessage("data_bounds");
    for ( i = 0 ; i < 6 ; i++ )
    {
      bounds[i] = 0.0 ;
    } 
  }
  return ;
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the bounds we need to use for a subwin (between user defined one and zoomed ones).
 */
void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6])
{
  if(sciGetZooming(pObj))
  {
    sciGetZoomBox(pObj, bounds);
  }
  else
  {
    sciGetDataBounds(pObj, bounds);
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get data-bounds defined by the user and not modified for pretty print by scilab.
 * @param bounds [Xmin,Xmax,Ymain,Ymax,Zmin,Zmax] vector.
 */
void sciGetDataBounds( sciPointObj * pObj, double bounds[6] )
{
  int i;
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    for ( i = 0 ; i < 6 ; i++ )
    {
      bounds[i] = pSUBWIN_FEATURE(pObj)->SRect[i] ;
    }
    return ;
  case SCI_SURFACE:
    for ( i = 0 ; i < 6 ; i++ )
    {
      bounds[i] = pSURFACE_FEATURE(pObj)->ebox[i] ;
    }
    return ;
  default:
    printSetGetErrorMessage("data_bounds");
    for ( i = 0 ; i < 6 ; i++ )
    {
      bounds[i] = 0.0 ;
    } 
  }
  return ;
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the viewing angles of a subwindow in degrees.
 */
void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta)
{
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    *alpha = pSUBWIN_FEATURE(pObj)->alpha;
    *theta = pSUBWIN_FEATURE(pObj)->theta;
    break;
  default:
    printSetGetErrorMessage("rotation_angles");
    *alpha = 0.0;
    *theta = 0.0;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Copy the logflags of a subwin to an array of size 3.
 */
void sciGetLogFlags(sciPointObj * pObj, char flags[3])
{
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    flags[0] = pSUBWIN_FEATURE(pObj)->logflags[0];
    flags[1] = pSUBWIN_FEATURE(pObj)->logflags[1];
    flags[2] = pSUBWIN_FEATURE(pObj)->logflags[2];
    break;
  default:
    printSetGetErrorMessage("log_flags");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * To know if the pixmap mode a figure is on or off.
 */
BOOL sciGetPixmapMode(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_FIGURE:
    return pFIGURE_FEATURE(pObj)->pixmapMode;
    break;
  default:
    printSetGetErrorMessage("pixmap");
    break;
  }
  return FALSE;
}
/*----------------------------------------------------------------------------------*/
/**
 * @return number of vertices of a pointObj
 */
int sciGetNbPoints(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_POLYLINE:
    return pPOLYLINE_FEATURE(pObj)->n1;
  default:
    printSetGetErrorMessage("points");
    break;
  }
  return -1;
}
/*----------------------------------------------------------------------------------*/
int sciGetPolylineStyle(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_POLYLINE:
    return pPOLYLINE_FEATURE(pObj)->plot;
  default:
    printSetGetErrorMessage("polyline_style");
    break;
  }
  return -1;
}
/*----------------------------------------------------------------------------------*/
BOOL sciGetIsClosed(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_POLYLINE:
    return pPOLYLINE_FEATURE(pObj)->closed;
  default:
    printSetGetErrorMessage("closed");
    break;
  }
  return FALSE;
}
/*----------------------------------------------------------------------------------*/
double sciGetArrowSize(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_POLYLINE:
    return pPOLYLINE_FEATURE(pObj)->arsize_factor;
  case SCI_SEGS:
    return pSEGS_FEATURE(pObj)->arrowsize;
  default:
    printSetGetErrorMessage("arrow_size");
    break;
  }
  return -1;
}
/*----------------------------------------------------------------------------------*/
void sciGetTextPos(sciPointObj * pObj, double position[3])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_TEXT:
    position[0] = pTEXT_FEATURE(pObj)->x;
    position[1] = pTEXT_FEATURE(pObj)->y;
    position[2] = pTEXT_FEATURE(pObj)->z;
    break;
  case SCI_LABEL:
    sciGetTextPos(pLABEL_FEATURE(pObj)->text, position);
    break;
  default:
    printSetGetErrorMessage("data");
    position[0] = 0.0;
    position[1] = 0.0;
    position[2] = 0.0;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Convert user coordinates to pixel ones (relative to the viewing canvas).
 * @param pObj subwindow handle
 * @param userCoord user coordinates
 * @param pixCoord result in pixels.
 */
void sciGetPixelCoordinate(sciPointObj * pObj, const double userCoord[3], int pixCoord[2])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    sciGetJavaPixelCoordinates(pObj, userCoord, pixCoord);
    break;
  default:
    Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
    pixCoord[0] = -1;
    pixCoord[1] = -1;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Convert user coordinates to user cooordinates (2D).
 * @param pObj subwindow handle
 * @param userCoord3D user coordinates
 * @param userCoords2D result in user coordinates in the default 2D plane.
 */
void sciGet2dViewCoordinate(sciPointObj * pObj, const double userCoords3D[3], double userCoords2D[2])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    sciGetJava2dViewCoordinates(pObj, userCoords3D, userCoords2D);
    break;
  default:
    Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
    userCoords2D[0] = 0.0;
    userCoords2D[1] = 0.0;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Convert pixel coordinates to 2D view coordinate
 * @param pObj subwindow handle
 * @param userCoord pixel coordinates
 * @param userCoords2D user coordinates in default 2D plane
 */
void sciGet2dViewCoordFromPixel(sciPointObj * pObj, const int pixelCoords[2], double userCoords2D[2])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    sciGetJava2dViewCoordFromPixel(pObj, pixelCoords, userCoords2D);
    break;
  default:
    Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
    userCoords2D[0] = 0.0;
    userCoords2D[1] = 0.0;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Convert 2d view coordinates to pixel coordinates
 * @param pObj subwindow handle
 * @param userCoords2D coordinates in th default 2D plane
 * @param pixelsCoords pixel coordinates
 */
void sciGet2dViewPixelCoordinates(sciPointObj * pObj, const double userCoords2D[2], int pixelCoords[2])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    {
      /* create a 3d user coord */
      double userCoord3D[3] = {userCoords2D[0], userCoords2D[1], 0.0};
      sciGetJava2dViewPixelCoordinates(pObj, userCoord3D, pixelCoords);
    }
    break;
  default:
    Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
    pixelCoords[0] = -1;
    pixelCoords[1] = -1;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * @return TRUE if pObj is a subwin with isoview mode on.
 */
BOOL sciGetIsIsoView(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->isoview;
  default:
    return FALSE;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * @return TRUE if pObj is a subwindow with cube scaling mode on.
 */
BOOL sciGetIsCubeScaled(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->cube_scaling;
  default:
    return FALSE;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get axes reverse property for each axis of a subwin object
 */
void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    axesReverse[0] = pSUBWIN_FEATURE(pObj)->axes.reverse[0];
    axesReverse[1] = pSUBWIN_FEATURE(pObj)->axes.reverse[1];
    axesReverse[2] = pSUBWIN_FEATURE(pObj)->axes.reverse[2];
    break;
  default:
    printSetGetErrorMessage("axes_reverse");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * @return TRUE if subwindow pObj tight_limit mode is on.
 */
BOOL sciGetTightLimitsOn(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->tight_limits;
  default:
    return FALSE;
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get auto_ticks property for each axis
 */
void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    autoTicks[0] = pSUBWIN_FEATURE(pObj)->axes.auto_ticks[0];
    autoTicks[1] = pSUBWIN_FEATURE(pObj)->axes.auto_ticks[1];
    autoTicks[2] = pSUBWIN_FEATURE(pObj)->axes.auto_ticks[2];
    break;
  default:
    autoTicks[0] = FALSE;
    autoTicks[1] = FALSE;
    autoTicks[2] = FALSE;
    printSetGetErrorMessage("auto_ticks");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get auto_ticks property for each axis
 */
BOOL sciGetAutoSubticks(sciPointObj * pObj)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    return !(pSUBWIN_FEATURE(pObj)->flagNax);
    break;
  default:
    return FALSE;
    printSetGetErrorMessage("auto_subticks");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the axes visible property for each axis.
 */
void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    axesVisible[0] = pSUBWIN_FEATURE(pObj)->axes.axes_visible[0];
    axesVisible[1] = pSUBWIN_FEATURE(pObj)->axes.axes_visible[1];
    axesVisible[2] = pSUBWIN_FEATURE(pObj)->axes.axes_visible[2];
    break;
  default:
    axesVisible[0] = FALSE;
    axesVisible[1] = FALSE;
    axesVisible[2] = FALSE;
    printSetGetErrorMessage("axes_visible");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the current zoom box of a subwin object
 * @param[out] zoomBox output parameter with thte zoom box
 */
void sciGetZoomBox(sciPointObj * pObj, double zoomBox[6])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    zoomBox[0] = pSUBWIN_FEATURE(pObj)->ZRect[0] ;
    zoomBox[1] = pSUBWIN_FEATURE(pObj)->ZRect[1] ;
    zoomBox[2] = pSUBWIN_FEATURE(pObj)->ZRect[2] ;
    zoomBox[3] = pSUBWIN_FEATURE(pObj)->ZRect[3] ;
    zoomBox[4] = pSUBWIN_FEATURE(pObj)->ZRect[4] ;
    zoomBox[5] = pSUBWIN_FEATURE(pObj)->ZRect[5] ;
    break;
  default:
    printSetGetErrorMessage("zoom_box");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the 4 corners of the bounding box of a text object in 3D
 */
void sciGetTextBoundingBox(sciPointObj * pObj, double corner1[3], double corner2[3],
                           double corner3[3], double corner4[3])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_TEXT:
    {
      int i;
      for (i = 0; i < 3; i++)
      {
        // should be up to date
        corner1[i] = pTEXT_FEATURE(pObj)->corners[0][i];
        corner2[i] = pTEXT_FEATURE(pObj)->corners[1][i];
        corner3[i] = pTEXT_FEATURE(pObj)->corners[2][i];
        corner4[i] = pTEXT_FEATURE(pObj)->corners[3][i];
      }
    }
    break;
  case SCI_LABEL:
    sciGetTextBoundingBox(pLABEL_FEATURE(pObj)->text, corner1, corner2, corner3, corner4);
    break;

  default:
    printSetGetErrorMessage("bounding box");
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the 4 corners of the bounding box of a text object in pixels
 */
void sciGetPixelBoundingBox(sciPointObj * pObj, int corner1[2], int corner2[2],
                            int corner3[2], int corner4[2])
{
  sciGetJavaPixelBoundingBox(pObj, corner1, corner2, corner3, corner4);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the 4 corners of the boundng box of a text object in 2D view user coordinates.
 */
void sciGet2dViewBoundingBox(sciPointObj * pObj, double corner1[2], double corner2[2],
                             double corner3[2], double corner4[2])
{
  sciPointObj * parentSubwin = sciGetParentSubwin(pObj);
  double corners3d[4][3];

  /* get bounding box */
  sciGetTextBoundingBox(pObj, corners3d[0], corners3d[1], corners3d[2], corners3d[3]);
  
  /* convert it to 2d view coordinates */
  sciGetJava2dViewCoordinates(parentSubwin, corners3d[0], corner1);
  sciGetJava2dViewCoordinates(parentSubwin, corners3d[1], corner2);
  sciGetJava2dViewCoordinates(parentSubwin, corners3d[2], corner3);
  sciGetJava2dViewCoordinates(parentSubwin, corners3d[3], corner4);

}
/*----------------------------------------------------------------------------------*/
/**
 * Get the viewing area of a subwindown acoordinf to its axes scale and margins
 * result is in pixels
 */
void sciGetViewingArea(sciPointObj * pObj, int * xPos, int * yPos, int * width, int * height)
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    sciGetJavaViewingArea(pObj, xPos, yPos, width, height);
    break;
  default:
    *xPos = -1;
    *yPos = -1;
    *width = -1;
    *height = -1;
    Scierror(999, _("Only axes handles have a viewing area."));
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the axis aligned bounding box a graphic object
 * @param bounds [xmin, xmax, ymin, ymax, zmin, zmax] bounds.
 */
void sciGetAABoundingBox(sciPointObj * pObj, double bounds[6])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    sciGetDataBounds(pObj, bounds);
    break;
  case SCI_SEGS:
    sciGetJavaSegsBoundingBox(pObj, bounds);
    break;
  case SCI_TEXT:
    {
      double corners[4][3];
      sciGetTextBoundingBox(pObj, corners[0], corners[1], corners[2], corners[3]);
      bounds[0] = Min(corners[0][0],Min(corners[1][0],Min(corners[2][0],corners[3][0])));
      bounds[1] = Max(corners[0][0],Max(corners[1][0],Max(corners[2][0],corners[3][0])));
      bounds[2] = Min(corners[0][1],Min(corners[1][1],Min(corners[2][1],corners[3][1])));
      bounds[3] = Max(corners[0][1],Max(corners[1][1],Max(corners[2][1],corners[3][1])));
      bounds[4] = Min(corners[0][2],Min(corners[1][2],Min(corners[2][2],corners[3][2])));
      bounds[5] = Max(corners[0][2],Max(corners[1][2],Max(corners[2][2],corners[3][2])));
    }
    break;
  case SCI_LABEL:
    sciGetAABoundingBox(pLABEL_FEATURE(pObj)->text, bounds);
    break;
  default:
    Scierror(999, _("Unable to compute data bounds for this kind of object."));
    break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the x_position value
 * @param bounds [xmin, xmax, ymin, ymax, zmin, zmax] bounds.
 */
char sciGetxLocation(sciPointObj * pObj)
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->axes.xdir;
    break;
  default:
    Scierror(999, _("Unable to compute x_location for this kind of object."));
    break;
  }
  return NULL;
}
/*----------------------------------------------------------------------------------*/
char sciGetyLocation(sciPointObj * pObj)
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->axes.ydir;
    break;
  default:
    Scierror(999, _("Unable to compute y_location for this kind of object."));
    break;
  }
  return NULL;
}
/*----------------------------------------------------------------------------------*/
/**
 * Check wether an object is using nurbs for display or not
 */
BOOL sciGetUseNurbs(sciPointObj * pObj)
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    return pSUBWIN_FEATURE(pObj)->useNurbsForArcs;
  case SCI_ARC:
    return pARC_FEATURE(pObj)->useNurbs;
  default:
    printSetGetErrorMessage("drawing_method");
    return FALSE;
  }
}
/*----------------------------------------------------------------------------------*/
/**
* Check wether an object is using fractional metrics for text display
*/
BOOL sciGetIsUsingFractionalMetrics(sciPointObj * pObj)
{
  sciFont * fontContext = sciGetFontContext(pObj);
  if (fontContext != NULL) {
    return fontContext->useFractionalMetrics;
  }
  printSetGetErrorMessage("fractional_font");
  return FALSE;
}
/*----------------------------------------------------------------------------------*/
/**
 * Get number of ticks along X axis of a subwindow.
 */
int sciGetNbXTicks(sciPointObj * pSubwin)
{
  return sciGetJavaNbXTicks(pSubwin);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the labels and positions of ticks along X axis.
 * String composing ticksLabels are allocated with MALLOC.
 */
void sciGetXTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels)
{
  sciGetJavaXTicksPos(pSubwin, ticksPos, ticksLabels);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get number of ticks along Y axis of a subwindow.
 */
int sciGetNbYTicks(sciPointObj * pSubwin)
{
  return sciGetJavaNbYTicks(pSubwin);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the labels and positions of ticks along Y axis.
 * String composing ticksLabels are allocated with C++ new.
 */
void sciGetYTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels)
{
  sciGetJavaYTicksPos(pSubwin, ticksPos, ticksLabels);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get number of ticks along Z axis of a subwindow.
 */
int sciGetNbZTicks(sciPointObj * pSubwin)
{
  return sciGetJavaNbZTicks(pSubwin);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the labels and positions of ticks along Z axis.
 * String composing ticksLabels are allocated with C++ new.
 */
void sciGetZTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels)
{
  sciGetJavaZTicksPos(pSubwin, ticksPos, ticksLabels);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the labels and positions of ticks along Z axis.
 * String composing ticksLabels are allocated with C++ new.
 */
BOOL sciGetIsAbleToCreateWindow(void)
{
  return sciGetJavaIsAbleToCreateWindow();
}
/*----------------------------------------------------------------------------------*/
/**
 * @return index of the subwin between all the subwins lying below its parent figure
 */
int sciGetSubwinIndex(sciPointObj * pSubwin)
{
  sciPointObj * parentFigure = sciGetParentFigure(pSubwin);
  int subwinIndex = 0;
  sciSons * pSons = sciGetSons(parentFigure);

  while (pSons->pointobj != pSubwin)
  {
    if (sciGetEntityType(pSons->pointobj) == SCI_SUBWIN)
    {
      // subwin found
      subwinIndex++;
    }
    pSons = pSons->pnext;
  }
  return subwinIndex;
}
/*----------------------------------------------------------------------------------*/
/**
 * @return TRUE if the object is automatically redraw and does not
 *         need to be explicitely drawn (using sciDrawObj).
 */
BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj)
{
	sciEntityType entityType = sciGetEntityType(pObj);
	return (entityType == SCI_UICONTROL)
		|| (entityType == SCI_UIMENU)
		|| (entityType == SCI_UICONTEXTMENU)
		|| (entityType == SCI_WAITBAR)
		|| (entityType == SCI_PROGRESSIONBAR);
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the subset of colormap bounds used by a particular object (colminmax).
 */
void sciGetColorRange(sciPointObj * pObj, int subset[2])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_FEC:
		subset[0] = pFEC_FEATURE(pObj)->colminmax[0];
		subset[1] = pFEC_FEATURE(pObj)->colminmax[1];
		break;
  default:
    printSetGetErrorMessage("color_range");
		subset[0] = 0;
		subset[1] = 0;
		break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the color to use for an objects when it uses index outside of the colormap (colout).
 */
void sciGetOutsideColor(sciPointObj * pObj, int colors[2])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_FEC:
		colors[0] = pFEC_FEATURE(pObj)->colout[0];
		colors[1] = pFEC_FEATURE(pObj)->colout[1];
		break;
  default:
    printSetGetErrorMessage("outside_color");
		colors[0] = 0;
		colors[1] = 0;
		break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the Z range used by a fec object (zminmax).
 */
void sciGetZBounds(sciPointObj * pObj, double bounds[2])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_FEC:
		bounds[0] = pFEC_FEATURE(pObj)->zminmax[0];
		bounds[1] = pFEC_FEATURE(pObj)->zminmax[1];
		break;
  default:
    printSetGetErrorMessage("z_bounds");
		bounds[0] = 0;
		bounds[1] = 0;
		break;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Get whether the grid is drawn in background or foreground.
 */
BOOL sciGetGridFront(sciPointObj * pObj)
{
  switch (sciGetEntityType(pObj))
  {
	case SCI_SUBWIN:
		return pSUBWIN_FEATURE(pObj)->gridFront;
  default:
    printSetGetErrorMessage("grid_position");
		return FALSE;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * @return the number of pass used for antialiasing or 0 if antialiasing is disable.
 */
int sciGetAntialiasingQuality(sciPointObj * pObj)
{
  switch (sciGetEntityType(pObj))
  {
	case SCI_FIGURE:
		if (isFigureModel(pObj))
		{
			return pFIGURE_FEATURE(pObj)->pModelData->antialiasingQuality;
		}
		else
		{
			return sciGetJavaAntialiasingQuality(pObj);
		}
  default:
    printSetGetErrorMessage("anti_aliasing");
		return FALSE;
	}
}
/*----------------------------------------------------------------------------------*/
/**
 * Get the position of a legend object relative to its parent subwin
 */
sciLegendPlace sciGetLegendLocation(sciPointObj * pObj)
{
  switch (sciGetEntityType(pObj))
  {
	case SCI_LEGEND:
		return pLEGEND_FEATURE(pObj)->place;
  default:
    printSetGetErrorMessage("legend_location");
		return SCI_LEGEND_POSITION_UNSPECIFIED;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Print the message "This object has no xxx property." in Scilab.
 */
void printSetGetErrorMessage(const char * propertyName)
{
  Scierror(999, _("This object has no %s property.\n"), propertyName );
}
/*----------------------------------------------------------------------------------*/

