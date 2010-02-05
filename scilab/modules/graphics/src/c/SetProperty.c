/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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
 *    -This file contains all functions used to SET the properties of graphics
 *    objects.
 *    - there are two kinds of functions : the sciInit*** which only set an object
 *      property and sciSet*** which do the same work but check before if it is
 *      necessary to do so.
 *    - The returning status of each functions is as follow :
 *      0 for correct execution, 1 if no change was made
 *      and -1 if an error occured.
 --------------------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "SetProperty.h"
#include "GetProperty.h"
#include "GetJavaProperty.h"
#include "InitObjects.h"
#include "BuildObjects.h"
#include "math_graphics.h"
#include "Scierror.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "BasicAlgos.h"
#include "WindowList.h"
#include "localization.h"
#include "SetJavaProperty.h"
#include "GraphicSynchronizerInterface.h"
#include "HandleManagement.h"
#include "loadTextRenderingAPI.h"
#include "sciprint.h"

#include "MALLOC.h"
#include "DrawingBridge.h"

#include "CallFigure.h"

#define MAX_MARK_STYLE 14
#define MAX_MARK_STYLE_S "14"

/*---------------------------------------------------------------------------*/
/* setSubWinAngles                                                                    */
/* Modify the viewing angles of a subwindow                                           */
/* for safer modifications prefer use Obj_RedrawNewAngle                              */
/*---------------------------------------------------------------------------*/
int setSubWinAngles( sciPointObj *psubwin, double theta, double alpha )
{
  /**dj20003***/
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE (psubwin) ;

  ppSubWin->alpha = alpha ;
  ppSubWin->theta = theta ;

  if ((alpha == 0.0) && (theta == 270.0))
  {
    ppSubWin->is3d = FALSE ;
    ppSubWin->project[2]= 0; /* no z to display */
    return 0;
  }

  ppSubWin->is3d  = TRUE  ;
  if ((alpha == 0.0 ) || (alpha == 180.0 ) || (alpha == -180.0 ))  /* DJ.A 30/12 */
  {
    ppSubWin->project[2]= 0; /* no z to display */
  }
  else
  {
    ppSubWin->project[2]= 1; /* z must be displayed */
    if (    ((alpha == 90.0 ) || (alpha == 270.0 ) || (alpha == -90.0 ) || (alpha == -270.0 ))
	 && ((theta == 90.0 ) || (theta == -90.0 ) || (theta == 270.0 ) || (theta == -270.0 )))
    {
      ppSubWin->project[1]= 0; /* no y to display */
    }
    else
    {
      ppSubWin->project[1]= 1;
      if (   ( (alpha == 90.0 ) || (alpha == 270.0 ) || (alpha == -90.0  ) || (alpha == -270.0 ))
          && ( (theta == 0.0  ) || (theta == 180.0 ) || (alpha == -180.0 ) ))
      {
        ppSubWin->project[0]= 0; /* BUG evreywhere when theta == 0 */
      }
      else
      {
        ppSubWin->project[0]= 1;
      }
    }
  }
  return 0 ;
}

/*---------------------------------------------------------------------------*/
/* Obj_RedrawNewAngle                                                                 */
/* Modify the viewing angles of a subwindow and the one of its brothers id necessary  */
/*---------------------------------------------------------------------------*/

void Obj_RedrawNewAngle(sciPointObj * pSubwin, double alpha, double theta)
{
  /* check if all the axis must be turned */
  sciPointObj * pParentFigure = sciGetParentFigure( pSubwin ) ;
  if ( pFIGURE_FEATURE(pParentFigure)->rotstyle == 1 )
  {
    /* every axes has the same angles */
    sciSons * subWins = sciGetSons( pParentFigure ) ;

    /* modify each axis */
    while ( subWins != NULL )
    {
      sciPointObj * curSubWin = subWins->pointobj ;
      if ( curSubWin->entitytype == SCI_SUBWIN )
      {
        setSubWinAngles( curSubWin, theta, alpha ) ;
        forceRedraw(curSubWin);
      }
      subWins = subWins->pnext ;
    }
  }
  else
  {
    /* modify angles only for this axes */
    setSubWinAngles( pSubwin, theta, alpha ) ;
    forceRedraw(pSubwin);
  }
}





/**sciSetEntityType
 * Sets the type of this Scilab graphic object (pobj)
 */
int
sciSetEntityType (sciPointObj * pobj, sciEntityType value)
{
  if (pobj != (sciPointObj *) NULL)
  {
    pobj->entitytype = value ;
    return 0 ;
  }
  else
  {
    return -1 ;
  }
}



/**sciSetColormap
 * This function sets a colormap to the figure. It's the same for all sons.
 * Setting the colormap a must be a m x 3 double RGB matrix:
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE
 */
int
sciSetColormap ( sciPointObj * pobj, double *rgbmat, int m, int n )
{
  int oldNbColors = sciGetNumColors(pobj) ;

  if ( n != 3 )
  {
    Scierror(999, _("%s: Number of columns must be %d.\n"),"colormap",3);
    return -1 ;
  }

  if ( SCI_FIGURE != sciGetEntityType(pobj) )
  {
    Scierror(999, _("%s: Object must be a %s.\n"),"sciSetColormap","SCI_FIGURE");
    return -1 ;
  }


  pFIGURE_FEATURE(pobj)->numcolors = m ;

  if ( pobj == getFigureModel() )
  {
    /* colormap is stored in the object */
    FREE(pFIGURE_FEATURE(pobj)->pModelData->colorMap) ;
    pFIGURE_FEATURE(pobj)->pModelData->colorMap = createDoubleArrayCopy(rgbmat, m * n ) ;
    pFIGURE_FEATURE(pobj)->pModelData->numColors = m * n ;
  }
  else
  {
    sciSetJavaColormap( pobj, rgbmat, m * n ) ;
    sciRecursiveUpdateBaW( pobj, oldNbColors, m ) ; /* missing line F.Leray */
  }

  return 0;
}


void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m)
{
  sciSons *psonstmp;

  sciSetNumColors (pobj,old_m);


  if((sciGetEntityType(pobj) != SCI_TEXT)        &&
     (sciGetEntityType(pobj) != SCI_LEGEND)      &&
     (sciGetEntityType(pobj) != SCI_AXES)        &&
     (sciGetEntityType(pobj) != SCI_LABEL)       &&
		 !sciIsAutomaticallyRedrawn(pobj))
    {
      if(old_m +1 == sciGetForeground(pobj)) {   /* 0 => deals with Foreground */
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,0,-1); /* Black */
	sciSetNumColors (pobj,old_m);
      }
      else  if(old_m +2 == sciGetForeground(pobj)) {
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,0,-2); /* White */
	sciSetNumColors (pobj,old_m);
      }
      if(old_m +1 == sciGetBackground(pobj)) { /* 1 => deals with Background */
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,1,-1);
	sciSetNumColors (pobj,old_m);
      }
      else if(old_m +2 == sciGetBackground(pobj)) {
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,1,-2);
	sciSetNumColors (pobj,old_m);
      }
    }

  if((sciGetEntityType(pobj) == SCI_TEXT)        ||
     (sciGetEntityType(pobj) == SCI_SUBWIN)      ||
     (sciGetEntityType(pobj) == SCI_LEGEND)      ||
     (sciGetEntityType(pobj) == SCI_AXES)        ||
     (sciGetEntityType(pobj) == SCI_SUBWIN)      ||
     (sciGetEntityType(pobj) == SCI_FIGURE)      ||
     (sciGetEntityType(pobj) == SCI_LABEL))
    {
      if(old_m +1 == sciGetFontForeground(pobj)) {
	/* 2 => deals with FontForeground */
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,2,-1);
	sciSetNumColors (pobj,old_m);
      }
      else  if(old_m +2 == sciGetFontForeground(pobj)) {
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,2,-2);
	sciSetNumColors (pobj,old_m);
      }

      if(old_m +1 == sciGetFontBackground(pobj)) { /* 3 => deals with FontBackground */
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,3,-1);
	sciSetNumColors (pobj,old_m);
      }
      else if(old_m +2 == sciGetFontBackground(pobj)) {
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,3,-2);
	sciSetNumColors (pobj,old_m);
      }
    }


  /* objects that can have marks */
  if((sciGetEntityType(pobj) == SCI_FIGURE)    ||
     (sciGetEntityType(pobj) == SCI_SUBWIN)    ||
     (sciGetEntityType(pobj) == SCI_LEGEND)    ||
     (sciGetEntityType(pobj) == SCI_ARC)       ||
     (sciGetEntityType(pobj) == SCI_POLYLINE)  ||
     (sciGetEntityType(pobj) == SCI_RECTANGLE) ||
     (sciGetEntityType(pobj) == SCI_SURFACE)   ||
     (sciGetEntityType(pobj) == SCI_AXES)      ||
     (sciGetEntityType(pobj) == SCI_SEGS))
    {
      if(old_m +1 == sciGetMarkForeground(pobj)) {   /* 4 => deals with MarkForeground */
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,4,-1); /* Black */
	sciSetNumColors (pobj,old_m);
      }
      else  if(old_m +2 == sciGetMarkForeground(pobj)) {
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,4,-2); /* White */
	sciSetNumColors (pobj,old_m);
      }
      if(old_m +1 == sciGetMarkBackground(pobj)) { /* 5 => deals with MarkBackground */
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,5,-1);
	sciSetNumColors (pobj,old_m);
      }
      else if(old_m +2 == sciGetMarkBackground(pobj)) {
	sciSetNumColors (pobj,m);
	sciUpdateBaW (pobj,5,-2);
	sciSetNumColors (pobj,old_m);
      }
    }

  sciSetNumColors (pobj,m); /* Add F.Leray 25.06.04 */

  psonstmp = sciGetLastSons (pobj);
  while (psonstmp != (sciSons *) NULL)
    {
      sciRecursiveUpdateBaW(psonstmp->pointobj, old_m, m);
      psonstmp = psonstmp->pprev;
    }
}


int
sciUpdateBaW (sciPointObj * pobj, int flag, int value)
{
  switch (flag)
    {
    case 0: /* Foreground*/
      switch (sciGetEntityType (pobj))
	{
	case SCI_POLYLINE:
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_ARC:
	case SCI_SEGS:
	case SCI_FEC:
	case SCI_GRAYPLOT:
	case SCI_RECTANGLE:
	case SCI_SURFACE:
	case SCI_AXES:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_TEXT:
	  sciSetForeground(pobj,value);
	  break;
	case SCI_UIMENU:
	case SCI_AGREG:
	case SCI_LEGEND:
	default:
	  return -1;
	  break;
	}
      break;
    case 1: /* Background*/
      switch (sciGetEntityType (pobj))
	{
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
	case SCI_LABEL: /* F.Leray 28.05.04 */
	case SCI_TEXT:
	  sciSetBackground(pobj,value);
	  break;
	case SCI_UIMENU:
	case SCI_AGREG:
	case SCI_LEGEND:
	default:
	  return -1;
	  break;
	}
      break;
    case 2: /* FontForeground*/
      switch (sciGetEntityType (pobj))
	{
	case SCI_AXES:
	case SCI_TEXT:
	case SCI_LEGEND:
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetFontForeground(pobj,value);
	  break;
	default:
	  return -1;
	  break;
	}
      break;
    case 3:
      switch (sciGetEntityType (pobj))
	{
	case SCI_AXES:
	case SCI_TEXT:
	case SCI_LEGEND:
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetFontBackground(pobj,value);
	  break;
	case SCI_UIMENU:
	default:
	  return -1;
	  break;
	}
      break;
    case 4: /* MarkForeground*/
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_LEGEND:
	case SCI_ARC:
	case SCI_POLYLINE:
	case SCI_RECTANGLE:
	case SCI_SURFACE:
	case SCI_AXES:
	case SCI_SEGS:
	  sciSetMarkForeground(pobj,value); /* F.Leray 21.01.05 */
	  break;
	case SCI_UIMENU:
	default:
	  return -1;
	  break;
	}
      break;
    case 5: /* MarkBackground*/
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_LEGEND:
	case SCI_ARC:
	case SCI_POLYLINE:
	case SCI_RECTANGLE:
	case SCI_SURFACE:
	case SCI_AXES:
	case SCI_SEGS:
	  sciSetMarkBackground(pobj,value); /* F.Leray 21.01.05 */
	  break;
	case SCI_UIMENU:
	default:
	  return -1;
	  break;
	}
      break;

    default:
      return -1;
      break;
    }

  return 0;
}

int sciInitNumColors( sciPointObj * pobj, int numcolors)
{

  /* modified jb Silvy 06/2006 */
	if (sciGetEntityType (pobj)==SCI_FIGURE)
  {
    pFIGURE_FEATURE(pobj)->numcolors = numcolors ;
    return 0 ;
  }else{
    return sciSetNumColors( sciGetParentFigure( pobj ), numcolors ) ;
  }
}

/**sciSetNumColors
 * This function sets the number of the color defined in colormap
 * param sciPointObj * pobj: the pointer to the entity
 * param int numcolor: the number of the indexed color
 * return 0 if ok, -1 if not
 */
int
sciSetNumColors (sciPointObj * pobj, int numcolors)
{
  if ( sciGetNumColors( pobj ) == numcolors )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitNumColors( pobj, numcolors ) ;

}


/*** Adding F.Leray 31.03.04 for supporting -1 and -2 indexes.*/
/* retrieve the realindex inside the colormap from the handle color property */
/* -1 is actually m+1 and -2, m+2 */
int sciSetGoodIndex(sciPointObj * pobj, int colorindex) /* return colorindex or m (Default Black) or m+1 (Default White)*/
{
  int m = sciGetNumColors (pobj);	/* the number of the color*/

  if( colorindex == -1 ) /* Black */
    return m + 1 ; /* modif. ici*/
  else if( colorindex == -2 ) /* White */
    return m + 2 ; /* modif. ici*/
  else
    return colorindex;
}

int sciInitBackground( sciPointObj * pobj, int colorindex )
{
  int m = sciGetNumColors(pobj);
  if(!sciCheckColorIndex(pobj, colorindex)) return 0;

  colorindex = sciSetGoodIndex(pobj,colorindex);

  if (sciGetGraphicContext(pobj) != NULL)
  {
    int newIndex = Max (0, Min (colorindex - 1, m + 1));
    sciGetGraphicContext(pobj)->backgroundcolor = newIndex;

    if (sciGetEntityType(pobj) == SCI_FIGURE && !isFigureModel(pobj))
    {
			/* disable protection since this function will call Java */
		  disableFigureSynchronization(pobj);
      sciSetJavaBackground(pobj, newIndex);
			enableFigureSynchronization(pobj);
    }

    return 0;
  }

  /*printSetGetErrorMessage("background");*/ /* rewrite updatebaw to renable this message */
  return -1;
}

/**sciSetBackground
 * Sets the number of the Background
 */
int
sciSetBackground (sciPointObj * pobj, int colorindex)
{
  if ( sciGetBackground( pobj ) == colorindex )
  {
    /* nothing to do */
    return 1 ;
  }

  return sciInitBackground( pobj, colorindex ) ;

}


int sciInitForeground( sciPointObj * pobj, int colorindex )
{
  int m = sciGetNumColors(pobj);
  if(!sciCheckColorIndex(pobj, colorindex)) return 0;

  colorindex = sciSetGoodIndex(pobj,colorindex);

  if (sciGetGraphicContext(pobj) != NULL)
  {
    sciGetGraphicContext(pobj)->foregroundcolor = Max (0, Min (colorindex - 1, m + 1));
    return 0;
  }

  /*printSetGetErrorMessage("foreground");*/ /* rewrite updatebaw to renable this message */
  return -1;
}

/**sciSetForeground
 * sets the number of the Foreground
 */
int
sciSetForeground (sciPointObj * pobj, int colorindex)
{
  if ( sciGetForeground( pobj ) == colorindex )
  {
    return 1 ;
  }
  return sciInitForeground( pobj, colorindex ) ;

}

int sciSetLineWidth( sciPointObj * pobj, double linewidth )
{
  if ( sciGetLineWidth( pobj ) == linewidth )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitLineWidth( pobj, linewidth ) ;
}


/**sciSetLineWidth
 * Sets the line width
 */
int
sciInitLineWidth (sciPointObj * pobj, double linewidth)
{

  if (linewidth < 0)
    {
      Scierror(999, _("Line width must be greater than %d.\n"),0);
      return -1;
    }
  else
  {

    if (sciGetGraphicContext(pobj) != NULL)
    {
      (sciGetGraphicContext(pobj))->linewidth = linewidth;
      return 0;
    }
  }
  printSetGetErrorMessage("thickness");
  return -1;
}


int sciSetLineStyle( sciPointObj * pobj, int linestyle )
{
  if ( sciGetLineStyle( pobj ) == linestyle )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitLineStyle( pobj, linestyle ) ;
}

/**sciSetLineStyle
 * Sets the line style
 */
int
sciInitLineStyle (sciPointObj * pobj, int linestyle)
{

  if (linestyle < 0)
    {
      Scierror(999, _("The line style must be greater than %d.\n"),0);
      return -1;
    }
  else
  {
    if (sciGetGraphicContext(pobj) != NULL)
    {
      (sciGetGraphicContext(pobj))->linestyle = linestyle;
      return 0;
    }
  }

  printSetGetErrorMessage("line_style");
  return -1;
}


int sciInitIsMark( sciPointObj * pobj, BOOL ismark )
{

  if (sciGetGraphicContext(pobj) != NULL)
  {
    sciGetGraphicContext(pobj)->ismark = ismark;
    return 0;
  }

  printSetGetErrorMessage("mark_mode");
  return -1;

}

/**sciSetIsMark
 * Sets the line style
 */
int
sciSetIsMark (sciPointObj * pobj, BOOL ismark)
{

  if ( sciGetIsMark(pobj) == ismark )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitIsMark( pobj, ismark ) ;

}


int sciInitMarkForeground( sciPointObj * pobj, int colorindex )
{
  if(!sciCheckColorIndex(pobj, colorindex)) return 0;
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/

  if (sciGetGraphicContext(pobj) != NULL)
  {
    sciGetGraphicContext(pobj)->markforeground =
      Max (-1, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
    return 0;
  }

  printSetGetErrorMessage("mark_foreground");
  return -1;

}

/**sciSetMarkForeground
 * sets the number of the Marks'Foreground
 */
int
sciSetMarkForeground (sciPointObj * pobj, int colorindex)
{

  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  if ( sciGetMarkForeground( pobj ) == colorindex )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitMarkForeground( pobj, colorindex ) ;

}

int sciInitMarkBackground( sciPointObj * pobj, int colorindex )
{
  if(!sciCheckColorIndex(pobj, colorindex)) return 0;
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/

  if (sciGetGraphicContext(pobj) != NULL)
  {
    sciGetGraphicContext(pobj)->markbackground =
      Max (-1, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
    return 0;
  }

  printSetGetErrorMessage("mark_background");
  return -1;
}

/**sciSetMarkBackground
 * sets the number of the Marks'Background
 */
int
sciSetMarkBackground (sciPointObj * pobj, int colorindex)
{

  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/

  if ( sciGetMarkBackground( pobj ) == colorindex )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitMarkBackground( pobj, colorindex ) ;

}



int sciInitMarkStyle( sciPointObj * pobj, int markstyle )
{
  if (markstyle < 0 || markstyle > MAX_MARK_STYLE )
  {
    Scierror(999, _("Wrong value for %s property: Must be in the interval [%s, %s].\n"),"mark_style","0",MAX_MARK_STYLE_S);
    return -1;
  }
  else
  {
    if (sciGetGraphicContext(pobj) != NULL)
    {
      sciGetGraphicContext(pobj)->markstyle = markstyle;
      return 0;
    }
  }

  printSetGetErrorMessage("mark_style");
  return -1;
}


/**sciSetMarkStyle
 * Sets the mark style
 */
int
sciSetMarkStyle (sciPointObj * pobj, int markstyle)
{

  if ( sciGetMarkStyle( pobj ) == markstyle )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitMarkStyle( pobj, markstyle ) ;
}

int sciInitMarkSize( sciPointObj * pobj, int marksize )
{
  if (marksize < 0)
  {
    Scierror(999, _("The mark size must be greater or equal than %d.\n"),0);
    return -1;
  }
  else
  {
    if (sciGetGraphicContext(pobj) != NULL)
    {
      sciGetGraphicContext(pobj)->marksize = marksize;
      return 0;
    }

  }

  printSetGetErrorMessage("mark_size");
  return -1;
}

/**sciSetMarkSize
 * Sets the mark size
 */
int
sciSetMarkSize (sciPointObj * pobj, int marksize)
{
  if ( sciGetMarkSize( pobj ) == marksize )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitMarkSize( pobj, marksize ) ;

}

int sciInitMarkSizeUnit( sciPointObj * pobj, int marksizeunit )
{
  if (marksizeunit < 0)
  {
    Scierror(999, _("The mark size unit must be greater than %d.\n"),0);
    return -1;
  }
  else
  {

    if (sciGetGraphicContext(pobj) != NULL)
    {
      (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
      return 0;
    }
  }

  printSetGetErrorMessage("mark_size_unit");
  return -1;
}

/**sciSetMarkSizeUnit
 * Sets the mark size unit
 * 1 : points, 2 : tabulated
 */
int
sciSetMarkSizeUnit (sciPointObj * pobj, int marksizeunit)
{

  if ( sciGetMarkSizeUnit( pobj ) == marksizeunit )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitMarkSizeUnit( pobj, marksizeunit ) ;
}

int sciInitIsLine( sciPointObj * pobj, BOOL isline )
{

  if (sciGetGraphicContext(pobj) != NULL)
  {
    sciGetGraphicContext(pobj)->isline = isline;
    return 0;
  }

  printSetGetErrorMessage("line_mode");
  return -1;

}

/**sciSetIsLine
 * Sets the line style existence
 */
int
sciSetIsLine (sciPointObj * pobj, BOOL isline)
{

  if ( sciGetIsLine( pobj ) == isline )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitIsLine( pobj, isline ) ;
}
/**sciInitPolylineStyle
 * Sets Polyline Style (plot)
 */
int sciInitPolylineStyle( sciPointObj * pobj, int plot )
{
  if (sciGetEntityType(pobj) == SCI_POLYLINE)
  {
    pPOLYLINE_FEATURE(pobj)->plot = 4;
    return 0;
  }

  printSetGetErrorMessage("polyline_style");
  return -1;

}
/**sciInitArrowSize
 * Sets Polyline arrow size (to be used with polyline_style=4)
 */
int sciInitArrowSize( sciPointObj * pobj, double size )
{
  if (sciGetEntityType(pobj) == SCI_POLYLINE)
  {
    pPOLYLINE_FEATURE(pobj)->arsize_factor = size;
    return 0;
  }

  printSetGetErrorMessage("arrow_size");
  return -1;

}


int sciInitFontSize( sciPointObj * pobj, double fontSize )
{
  if (fontSize < 0)
  {
    Scierror(999, _("The font size must be greater than %d.\n"),0);
    return -1;
  }
  else
  {
    if (sciGetFontContext(pobj) != NULL)
    {
      (sciGetFontContext(pobj))->fontSize = fontSize;
      return 0;
    }
    else
    {
      printSetGetErrorMessage("font_size");
      return -1;
    }
  }
}

/**sciSetFontSize
 * Sets the font size
 */
int sciSetFontSize(sciPointObj * pobj, double fontSize)
{

  if (sciGetFontSize( pobj ) == fontSize)
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitFontSize(pobj, fontSize) ;

}

int sciInitFontOrientation( sciPointObj * pobj, double textorientation )
{
  if (sciGetFontContext(pobj) != NULL)
  {
    (sciGetFontContext(pobj))->textorientation = textorientation;
    return 0;
  }
  printSetGetErrorMessage("font_angle");
  return -1;
}



/**sciSetFontOrientation
 * Sets the font Orientation
 */
int
sciSetFontOrientation (sciPointObj * pobj, double textorientation)
{

  if ( sciGetFontOrientation( pobj ) == textorientation )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitFontOrientation( pobj, textorientation ) ;

}

int sciSetStrings( sciPointObj * pObjDest, const StringMatrix * pStrings )
{
  switch (sciGetEntityType (pObjDest))
    {
    case SCI_TEXT:
      deleteMatrix( pTEXT_FEATURE (pObjDest)->pStrings ) ;
      pTEXT_FEATURE (pObjDest)->pStrings = copyStringMatrix( pStrings ) ;
      if ( pTEXT_FEATURE (pObjDest)->pStrings == NULL )
      {
        return -1 ;
      }
      break;
    case SCI_LEGEND:
      deleteMatrix( pLEGEND_FEATURE (pObjDest)->text.pStrings ) ;
      pLEGEND_FEATURE (pObjDest)->text.pStrings = copyStringMatrix( pStrings ) ;
      if ( pLEGEND_FEATURE (pObjDest)->text.pStrings == NULL )
      {
        return -1 ;
      }
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return sciSetStrings( pLABEL_FEATURE(pObjDest)->text, pStrings ) ;
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
      printSetGetErrorMessage("text");
      return -1;
      break;
    }
  return 0;
}

/**sciSetText
 * Sets the Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @param char *text[] : the text which has to be put
 * @param int nbRow : the number of row of the text matrix
 * @param int nbCol : the number of col of the text matrix
 * @return  0 if OK, -1 if not
 */
int sciSetText (sciPointObj * pobj, char ** text, int nbRow, int nbCol)
{

/* Check if we should load LaTex / MathML Java libraries */
	loadTextRenderingAPI(text, nbRow, nbCol);

	switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      deleteMatrix( pTEXT_FEATURE (pobj)->pStrings ) ;
      pTEXT_FEATURE (pobj)->pStrings = newFullStringMatrix( text, nbRow, nbCol ) ;
      if ( pTEXT_FEATURE (pobj)->pStrings == NULL )
      {
        return -1 ;
      }
      break;
    case SCI_LEGEND:
      deleteMatrix( pLEGEND_FEATURE (pobj)->text.pStrings ) ;
      pLEGEND_FEATURE (pobj)->text.pStrings = newFullStringMatrix( text, nbRow, nbCol ) ;
      if ( pLEGEND_FEATURE (pobj)->text.pStrings == NULL )
      {
        return -1 ;
      }
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return sciSetText( pLABEL_FEATURE(pobj)->text, text, nbRow, nbCol ) ;
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
      printSetGetErrorMessage("text");
      return -1;
      break;
    }
  return 0;
}


int sciInitFontBackground( sciPointObj * pobj, int colorindex )
{
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));;
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_UIMENU:
    case SCI_ARC:
    case SCI_SEGS:
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AGREG:
    default:
      printSetGetErrorMessage("font_background");
      return -1;
      break;
    }
  return 0;
}

/**sciSetFontBackground
 * Sets the background color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontBackground (sciPointObj * pobj, int colorindex)
{
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/

  if ( sciGetFontBackground( pobj ) == colorindex )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitFontBackground( pobj, colorindex ) ;

}

int sciInitFontForeground( sciPointObj * pobj, int colorindex )
{
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LABEL: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_UIMENU:
    case SCI_ARC:
    case SCI_SEGS:
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AGREG:
    default:
      printSetGetErrorMessage("font_foreground");
      return -1;
      break;
    }
  return 0;
}

/**sciSetFontForeground
 * Sets the foreground color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, if not
 */
int
sciSetFontForeground (sciPointObj * pobj, int colorindex)
{
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/

  if ( sciGetFontForeground( pobj ) == colorindex )
  {
    /* nothing to do */
    return 1 ;
  }

  return sciInitFontForeground( pobj, colorindex ) ;
}


int sciInitFontStyle( sciPointObj * pobj, int iAttributes )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_SUBWIN:
    case SCI_FIGURE:
    case SCI_LABEL: /* F.Leray 10.06.04 */
      (sciGetFontContext(pobj))->fonttype =iAttributes;
      break;
    case SCI_UIMENU:
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
      printSetGetErrorMessage("font_style");
      return -1;
      break;
    }
  return 0;
}

/**sciSetFontStyle
 * Sets the font style
 * @param sciPointObj * pobj: the pointer to the entity
 * @param  int iAttributes (a logical mask with SCI_ATTR_BOLD|SCI_ATTR_ITALIC|SCI_ATTR_UNDERLINE|SCI_ATTR_STRIKEOUT)
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontStyle (sciPointObj * pobj, int iAttributes )
{
  if ( sciGetFontStyle( pobj ) == iAttributes )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitFontStyle( pobj, iAttributes ) ;

}


int sciInitLegendPlace( sciPointObj * pobj, sciLegendPlace place )
{
  double position[2]={0.0,0.0};
  
  if (sciGetEntityType (pobj) == SCI_LEGEND)
  {
    pLEGEND_FEATURE (pobj)->place = place;
    /* calcul de l emplacement relatif du titre
     * en fonction de la taille de la police
     * de la fenetre...
     */
    return sciInitLegendPos (pobj, position );
  }

  Scierror(999, _("You are not using a legend object.\n"));
  return -1;
}

/**sciSetLegendPlace
 * Sets the Title Place  with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place)
{


  if ( sciGetLegendPlace( pobj ) == place )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitLegendPlace( pobj, place ) ;

}

int sciInitLegendPos( sciPointObj * pobj, double position[] )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_LEGEND:
      pLEGEND_FEATURE (pobj)->pos.x = position[0];
      pLEGEND_FEATURE (pobj)->pos.y = position[1];
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_ARC:
    case SCI_SEGS:
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_AGREG:
    case SCI_LABEL:
    case SCI_UIMENU:
    default:
      Scierror(999, _("You are not using a legend object.\n"));
      return -1;
      break;
    }
  return 0;
}

/**sciSetLegendPos
 * Sets the Legend Position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetLegendPos (sciPointObj * pobj, double position[])
{
  double position_old[2] ;
  sciGetLegendPos( pobj ,position_old) ;
  if ( position[0] ==  position_old[0] && position[1] == position_old[1] )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitLegendPos( pobj, position ) ;
}

int sciInitIsClipping( sciPointObj * pobj, int value )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->isclip = value;
      if(value>0) pSUBWIN_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_ARC:
      pARC_FEATURE (pobj)->isclip = value;
      if(value>0) pARC_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE (pobj)->isclip = value;
      if(value>0) pPOLYLINE_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pobj)->isclip = value;
      if(value>0) pRECTANGLE_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_SEGS:
      pSEGS_FEATURE (pobj)->isclip = value;
      if(value>0) pSEGS_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_TEXT:
      pTEXT_FEATURE (pobj)->isclip = value;
      if(value>0) pTEXT_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_AXES:
      pAXES_FEATURE (pobj)->isclip = value;
      if(value>0) pAXES_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_SURFACE:
      pSURFACE_FEATURE (pobj)->isclip = value;
      if(value>0) pSURFACE_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_FEC:
      pFEC_FEATURE (pobj)->isclip = value;
      if(value>0) pFEC_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_GRAYPLOT:
      pGRAYPLOT_FEATURE (pobj)->isclip = value;
      if(value>0) pGRAYPLOT_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_LEGEND:
			pLEGEND_FEATURE(pobj)->isclip = value;
			if(value>0) { pLEGEND_FEATURE (pobj)->clip_region_set=1;}
      break;
    case SCI_AGREG:
    case SCI_FIGURE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("clip_state");
      return -1 ;
      break;
    }
  return 0 ;
}

/**sciSetIsClipping*/
int
sciSetIsClipping( sciPointObj * pobj, int value )
{

  if ( sciGetIsClipping( pobj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitIsClipping( pobj, value ) ;

}


/**sciSetClipping
 * Sets the clipping area
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
int
sciSetClipping (sciPointObj * pobj, double pclip[4] )
{
  int i;

  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      for(i=0;i<4;i++) {pSUBWIN_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_ARC:
      for(i=0;i<4;i++) {pARC_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_POLYLINE:
      for(i=0;i<4;i++) {pPOLYLINE_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_RECTANGLE:
      for(i=0;i<4;i++) {pRECTANGLE_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_SEGS:
      for(i=0;i<4;i++) {pSEGS_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_TEXT:
      for(i=0;i<4;i++) {pTEXT_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_AXES:
      for(i=0;i<4;i++) {pAXES_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_SURFACE:
      for(i=0;i<4;i++) {pSURFACE_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_LEGEND:
      for(i=0;i<4;i++) {pLEGEND_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_FEC:
      for(i=0;i<4;i++) {pFEC_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_GRAYPLOT:
      for(i=0;i<4;i++) {pGRAYPLOT_FEATURE (pobj)->clip_region[i] = pclip[i];}
      break;
    case SCI_LABEL:
      return sciSetClipping( pLABEL_FEATURE (pobj)->text, pclip ) ;
      break;
    case SCI_AGREG:
    case SCI_FIGURE:
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("clip_box");
      return -1;
      break;
    }
  return 0;

}


int sciInitAddPlot( sciPointObj * pobj, BOOL value )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->addplot = value;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->addplot = value;
      /*(sciGetGraphicMode (sciGetParentFigure(pobj)))->addplot = value;*/
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
      return -1 ;
      break;
    }
  return 0 ;
}

/**sciSetAddPlot
 * Tunrs ON or OFF the possibility to draw multi plots in one graphic.
 * If FALSE, plot is cleared before new drawing
 */
int
sciSetAddPlot (sciPointObj * pobj, BOOL value)
{
  if ( sciGetAddPlot( pobj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitAddPlot( pobj, value ) ;

}


int sciInitAutoScale( sciPointObj * pobj, BOOL value )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (pobj == getFigureModel())
	(sciGetGraphicMode (pobj))->autoscaling = value;
      else
	sciSetAutoScale(sciGetFirstTypedSelectedSon(pobj, SCI_SUBWIN),value);
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->autoscaling = value;
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
      printSetGetErrorMessage("autos_cale");
      return -1 ;
      break;
    }

  return 0 ;
}

/**sciSetAutoScale
 * Sets the mode if the plot is in zoom fit mode, or fixed by the user
 */
int
sciSetAutoScale (sciPointObj * pobj, BOOL value)
{

  if ( sciGetAutoScale( pobj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitAutoScale( pobj, value ) ;

}

int sciInitZooming( sciPointObj * pobj, BOOL value )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->zooming = value;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->zooming = value;
      /* the value is inhirated by the parent */
      sciSetZooming (sciGetParentFigure (pobj), value);
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
      printSetGetErrorMessage("zoom");
      return -1 ;
      break;
    }

  return 0 ;
}

/**sciSetZooming
 * Sets the zooming state of the object. Is it or not zommed right now
 */
int
sciSetZooming (sciPointObj * pobj, BOOL value)
{
  if ( sciGetZooming( pobj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitZooming( pobj, value ) ;

}


/**sciSetDefaultValue
 * Sets the default values of figure properties
 * (graphics context, graphics mode, and graphics font)
 * @author Djalel ABDEMOUCHE
 */
int
sciSetDefaultValues (void)
{
  if ((sciInitGraphicContext (sciGetCurrentFigure()) == -1) ||
      (sciInitGraphicMode (sciGetCurrentFigure()) == -1) ||
      (sciInitFontContext (sciGetCurrentFigure()) == -1)) /* Adding F.Leray 13.04.04 to have the completed init.*/
  {
    Scierror(999, _("Unable to load default values.\n"));
    return -1 ;
  }
  return 0;
}



int sciInitXorMode( sciPointObj * pobj, int value )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->xormode = value;
      break;
    case SCI_SUBWIN:
      /* the value is inhirated by the parent */
      sciSetXorMode (sciGetParentFigure (pobj), value);
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
      return -1 ;
      break;
    }

  return 0 ;
}

/**sciSetXorMode
 * Sets the xor mode
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int value: the value of the xor mode
 * @return
 * @author Djalel ABDEMOUCHE
 * @see sciGetXorMode
 */
int
sciSetXorMode (sciPointObj * pobj, int value)
{

  if ( sciGetXorMode( pobj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitXorMode( pobj, value ) ;

}

int sciInitVisibility( sciPointObj * pobj, BOOL value )
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE (pobj)->visible = value;
      break;
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->visible = value;
      break;
    case SCI_LEGEND:
      pLEGEND_FEATURE (pobj)->visible = value;
      break;
    case SCI_ARC:
      pARC_FEATURE (pobj)->visible = value;
      break;
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE (pobj)->visible = value;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pobj)->visible = value;
      break;
    case SCI_SURFACE:
      pSURFACE_FEATURE (pobj)->visible = value;
      break;
    case SCI_SEGS:
      pSEGS_FEATURE (pobj)->visible = value;
      break;
    case SCI_FEC:
      pFEC_FEATURE (pobj)->visible = value;
      break;
    case SCI_GRAYPLOT:
      pGRAYPLOT_FEATURE (pobj)->visible = value;
      break;
    case SCI_TEXT:
      pTEXT_FEATURE (pobj)->visible = value;
      break;
    case SCI_AXES:
      pAXES_FEATURE (pobj)->visible = value;
      break;
    case SCI_AGREG:
      pAGREG_FEATURE (pobj)->visible = value;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return sciInitVisibility( pLABEL_FEATURE(pobj)->text, value ) ;
      break;
    case SCI_UIMENU:
    case SCI_UICONTROL:
    default:
      return -1 ;
      break;
    }
  return 0 ;
}

/**sciSetVisibility*/
int
sciSetVisibility (sciPointObj * pobj, BOOL value)
{
  if ( sciGetVisibility( pobj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitVisibility( pobj, value ) ;

}


int sciInitResize( sciPointObj * pobj, BOOL value )
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (isFigureModel(pobj))
      {
        pFIGURE_FEATURE(pobj)->pModelData->autoResizeMode = value;
      }
      else
      {
        sciSetJavaAutoResizeMode(pobj, value);
      }
      break;
    case SCI_SUBWIN:
      sciInitResize(sciGetParentFigure(pobj), value);
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
      return -1 ;
      break;
    }
  return 0 ;
}

/**sciSetResize
 * Tunrs ON or OFF the autoresizing mode (when the window is resized by user)
 * @param sciPointObj * pobj: the pointer to the entity
 * @param BOOL value: the value of the switch
 */
int
sciSetResize (sciPointObj * pobj, BOOL value)
{

  if ( sciGetResize( pobj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitResize( pobj, value ) ;

}



int sciInitName(sciPointObj * pobj, char * newName)
{
  switch (sciGetEntityType (pobj))
  {
    case SCI_FIGURE:
    {
      int newNameLength;

			/* first case newName is NULL */
			if (newName == NULL)
			{
				/* Just set an empty title for the physical window if needed */
				if (!isFigureModel(pobj))
				{
					sciSetJavaTitle(pobj, "");
				}
				
				return 0;
			}
			
			/* newName is a valid string */
			newNameLength = (int) strlen(newName);
      
      /* Reallocate name */
      pFIGURE_FEATURE(pobj)->name = MALLOC( (newNameLength + 1) * sizeof(char) );

			/* copy the string */
      strcpy(pFIGURE_FEATURE(pobj)->name, newName) ;
			
			/* Update the name of the physical window if one exists */
      if (!isFigureModel(pobj))
      {
      	/* In this case, we need to send the name to the physical window */
        if ( checkPercent(newName) == 0 )
        {
         	/* no %d, set the java title directly */
        	sciSetJavaTitle(pobj, pFIGURE_FEATURE(pobj)->name);
        }
        else
        {
          /* a %d inside */
          /* get the number digits of the window number */
          int figureNumber = sciGetNum( pobj ) ;
          int nbDigits = GET_NB_DIGITS( figureNumber ) ;
          int realTitleLength = newNameLength + nbDigits - 2 ; /* -2 for the %d which is replaced */
          char * realTitle = MALLOC( (realTitleLength + 1) * sizeof(char) ) ;
          if ( realTitle == NULL )
          {
            Scierror(999, _("%s: No more memory.\n"),"sciSetName");
            return -1 ;
          }
          sprintf( realTitle, newName, figureNumber ) ;
          sciSetJavaTitle(pobj, realTitle);
          FREE(realTitle) ;
        }

        
      }

      return 0 ;
    }
    default:
      printSetGetErrorMessage("figure_name");
      return -1 ;
      break;
    }
}

/**sciSetName
 * Sets the name of the Figure
 * @param sciPointObj * pobj: the pointer to the entity
 * @param char newName: a pointer to the string contening name
 * @return
 */
int
sciSetName(sciPointObj * pobj, char * newName)
{
  if (   newName != NULL
      && sciGetName(pobj) != NULL
      && strcmp(sciGetName(pobj), newName) == 0)
	{
		// nothing to do
		return 1;
	}
	
	/* Check that the string contains at most one %d character */
	if (checkPercent(newName) < 0)
  {
  	Scierror(999, _("Figure name may not contains any %% character, except a single %%d.\n")) ;
  	return -1 ;
  }
	
	/* Free the current name if needed */
	if (sciGetName(pobj) != NULL)
	{
		FREE(pFIGURE_FEATURE(pobj)->name);
		pFIGURE_FEATURE(pobj)->name = NULL;
	}
	
	return sciInitName(pobj, newName);
}

int sciInitNum( sciPointObj * pobj, int value )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE(pobj)->number = value;
      break;
    default:
      printSetGetErrorMessage("figure_id");
      return -1 ;
      break;
    }
  return 0 ;
}

/**sciSetNum
 * Sets the number of the Figure.
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int value: the value of the number of the windows
 */
int
sciSetNum (sciPointObj * pobj, int value )
{

  if ( sciGetNum(pobj) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitNum( pobj, value ) ;

}

int sciInitDimension( sciPointObj * pobj, int newWidth, int newHeight )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if ( isFigureModel(pobj) )
      {
        pFIGURE_FEATURE(pobj)->pModelData->figureWidth  = newWidth ;
        pFIGURE_FEATURE(pobj)->pModelData->figureHeight = newHeight;
      }
      else
      {
        int size[2] = {newWidth, newHeight} ;
        return sciSetJavaFigureSize(pobj, size) ;
      }
      break;
    default:
      printSetGetErrorMessage("size");
      return -1 ;
      break;
    }
  return 0 ;
}


/**sciSetDim
 * Sets the dimension of the Figure or Subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int *pwidth: the width of the window dimension
 * @param int *pheight: the height of the window dimension
 * @return one of the EResizeStatus values.
 */
int sciSetDimension( sciPointObj * pobj, int newWidth, int newHeight )
{

  int width ;
  int height ;
  sciGetDim( pobj, &width, &height ) ;
  if ( width == newWidth && height == newHeight )
  {
    return 1 ;
  }
  return sciInitDimension( pobj, newWidth, newHeight ) ;

}

int sciInitWindowDim( sciPointObj * pobj, int newWidth, int newHeight )
{
  switch (sciGetEntityType (pobj))
  {
  case SCI_FIGURE:
    if ( pobj == getFigureModel() )
    {
      pFIGURE_FEATURE(pobj)->pModelData->windowWidth  = newWidth ;
      pFIGURE_FEATURE(pobj)->pModelData->windowHeight = newHeight;
    }
    else
    {
      int size[2] = {newWidth, newHeight} ;
      sciSetJavaWindowSize(pobj, size) ;
      //Check the new size
      sciGetJavaWindowSize(pobj, size);
      if(size[0]!=newWidth || size[1]!=newHeight)
      {
        sciprint(_("WARNING : The size of the figure may not be as wide as you want.\n"));
      }
    }
    break;
  default:
    printSetGetErrorMessage("figure_size");
    return -1 ;
    break;
  }
  return 0 ;
}

int sciSetWindowDim( sciPointObj * pobj, int newWidth, int newHeight )
{
  if ( sciGetWindowWidth(pobj) == newWidth && sciGetWindowHeight(pobj) == newHeight )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitWindowDim(pobj, newWidth, newHeight ) ;
}

int sciInitScreenPosition( sciPointObj * pobj, int pposx, int pposy )
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if ( pobj == getFigureModel() )
      {
        pFIGURE_FEATURE(pobj)->pModelData->windowPosition[0] = pposx ;
        pFIGURE_FEATURE(pobj)->pModelData->windowPosition[1] = pposy ;
      }
      else
      {
        int pos[2] = {pposx, pposy} ;
        sciSetJavaWindowPosition(pobj, pos) ;
      }
      return 0;
    default:
      printSetGetErrorMessage("figure_position");
      return -1;
      break;
    }
  return 0;
}

/**sciSetScreenPosition
 * Sets the position of the FIGURE (the window) in root.
 */
int
sciSetScreenPosition(sciPointObj * pobj, int pposx, int pposy)
{
  int posX ;
  int posY ;
  sciGetScreenPosition( pobj, &posX, &posY ) ;
  if ( posX == pposx && posY == pposy )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitScreenPosition( pobj, pposx, pposy ) ;

}


/*--------------------------------------------------------------------------*/
/* sciSelectFirstSubwin                                                              */
/* select under the first window found under the current figure                      */
/*--------------------------------------------------------------------------*/
void sciSelectFirstSubwin( sciPointObj * parentFigure )
{
  sciSons * figureSons = sciGetSons ( parentFigure ) ;
  if ( figureSons != (sciSons *) NULL )
  {
    /* look for the first subwindow */
    while (   (figureSons->pnext != (sciSons *) NULL)
           && (sciGetEntityType (figureSons->pointobj) != SCI_SUBWIN))
    {
      figureSons = figureSons->pnext;
    }

    if( sciGetEntityType (figureSons->pointobj) == SCI_SUBWIN )
    {
      /* we found another valid subwindow */
      sciSetSelectedSubWin (figureSons->pointobj);
    }
    else
    {
      sciSetSelectedSubWin(NULL);
    }
  }
}


int sciInitSelectedSubWin( sciPointObj * psubwinobj )
{
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE ( psubwinobj ) ;

  sciInitSelectedObject( psubwinobj ) ;

  return 0 ;
}

/**sciSetSelectedSubWin
 * Determines wich SubWin is selected or not. WARNING TO BE DEFINED.
 * @param sciPointObj * psubwinobj: the pointer to the entity sub window
 * @return 1 if OK or -1 if NOT OK
 */
int
sciSetSelectedSubWin (sciPointObj * psubwinobj)
{


  /* on verifie que l'entite passee en argument est bien une sous fenetre */
  if (sciGetEntityType (psubwinobj) != SCI_SUBWIN)
  {
    Scierror(999, _("Handle is not a SubWindow.\n"));
    return -1;
  }

  /* on verifie que la sous fenetre donnee n'est pas deja selectionnee */
  if (sciGetIsSelected(psubwinobj))
  {
    /* nothing to do then */
    return 1 ;
  }

  return sciInitSelectedSubWin( psubwinobj ) ;
}

/*-------------------------------------------------------------------------------*/
int sciInitSelectedObject( sciPointObj * pObj )
{
  sciAddUniqueSelectedSon(sciGetParent(pObj), pObj ) ;
  return 0 ;
}
/*-------------------------------------------------------------------------------*/

/**sciSetOriginalSubWin
 * DO NOT USE !!!
 * @param sciPointObj * pfigure, the parent figure,
 * @param sciPointObj * psubwin: the pointer to the entity subwindow
 * @return 0 if OK -1 if not
 */
int
sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin)
{
  pFIGURE_FEATURE(pfigure)->originalsubwin0011 = psubwin;
  return 0;
}

/**sciSetPoint
 * sets points of the entity, and a pointer to the number of points
 */
/** MAJ pour le 3D DJ.Abdemouche 2003**/
int
sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol)
{
  int i,n1,k,k1,k2;
  double * pvx  = NULL ;
  double * pvy  = NULL ;
  double * pvz  = NULL ;
  double * pvfx = NULL ;
  double * pvfy = NULL ;
  double * pvfz = NULL ;
  int * pstyle = NULL;

  switch (sciGetEntityType (pthis))
    {
    case SCI_POLYLINE:
      n1=pPOLYLINE_FEATURE (pthis)->n1;
      if ( (*numcol != 3) && (*numcol != 2) && (*numcol != 0) )
	{
	  Scierror(999, _("Number of columns must be %d (%d if %s coordinate).\n"),2,3,"z");
	  return -1;
	}
      if (*numrow != n1) /* SS 30/1/02 */
      {

        FREE(pPOLYLINE_FEATURE (pthis)->pvx); pPOLYLINE_FEATURE (pthis)->pvx = NULL;
        FREE(pPOLYLINE_FEATURE (pthis)->pvy); pPOLYLINE_FEATURE (pthis)->pvy = NULL;
        FREE(pPOLYLINE_FEATURE (pthis)->pvz); pPOLYLINE_FEATURE (pthis)->pvz = NULL;

        n1=*numrow;

        if ( *numcol > 0 )
        {
          if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	  if ((pvy = MALLOC (n1 * sizeof (double))) == NULL)
          {
            FREE(pvx); pvx = (double *) NULL;
	    return -1;
	  }
	  if (*numcol == 3)
          {
            if ((pvz = MALLOC (n1 * sizeof (double))) == NULL)
            {
              FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
              return -1;
            }
          }

	  for ( i = 0 ; i < *numrow ; i++ )
	  {
	    pvx[i] = tab[i];
	    pvy[i] = tab[i+ (*numrow)];
            if (*numcol == 3)
            {
	      pvz[i] = tab[i+ 2*(*numrow)];
            }
          }
	  pPOLYLINE_FEATURE (pthis)->pvx=pvx;
	  pPOLYLINE_FEATURE (pthis)->pvy=pvy;
          pPOLYLINE_FEATURE (pthis)->pvz=pvz;
        }

        pPOLYLINE_FEATURE (pthis)->n1 = n1 ;

      }
      else
	{
	  for (i=0;i < *numrow;i++)
	    {
	      pPOLYLINE_FEATURE (pthis)->pvx[i] = tab[i];
	      pPOLYLINE_FEATURE (pthis)->pvy[i] = tab[i+ (*numrow)];
	    }
	  if (*numcol == 3)
	    {
	      if(pPOLYLINE_FEATURE (pthis)->pvz==NULL)
		if ((pPOLYLINE_FEATURE (pthis)->pvz = MALLOC ((*numrow) * sizeof (double))) == NULL)
		  return -1;

	      for (i=0;i < *numrow;i++)
		pPOLYLINE_FEATURE (pthis)->pvz[i] = tab[i+ 2*(*numrow)];
	    }
	  else
	    {
	      FREE(pPOLYLINE_FEATURE (pthis)->pvz);
	      pPOLYLINE_FEATURE (pthis)->pvz=NULL;
	    }
	}



      return 0;
      break;
    case SCI_RECTANGLE:
    {
      int widthIndex = 2 ;
      int size = *numrow * *numcol ;
      if ( size != 5 && size != 4 )
      {
        Scierror(999, _("Number of elements must be %d (%d if %s coordinate).\n"),4,5,"z");
        return -1;
      }

      pRECTANGLE_FEATURE (pthis)->x = tab[0] ;
      pRECTANGLE_FEATURE (pthis)->y = tab[1] ;

      if ( size == 5 )
      {
        pRECTANGLE_FEATURE (pthis)->z = tab[2] ;
        widthIndex = 3 ;
      }

      /* check that the height and width are positive */
      if ( tab[widthIndex] < 0.0 || tab[widthIndex + 1] < 0.0 )
      {
        Scierror(999,"Width and height must be positive.\n") ;
        return -1 ;
      }
      pRECTANGLE_FEATURE (pthis)->width  = tab[widthIndex    ] ;
      pRECTANGLE_FEATURE (pthis)->height = tab[widthIndex + 1] ;

      return 0;
    }
    break;
    case SCI_ARC:
      if ((*numrow * *numcol != 7)&&(*numrow * *numcol != 6))
	{
	  Scierror(999, _("Number of elements must be %d (%d if z coordinate )\n"),6,7);
	  return -1;
	}

      pARC_FEATURE (pthis)->x          = tab[0];
      pARC_FEATURE (pthis)->y          = tab[1];
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  pARC_FEATURE (pthis)->z          = tab[2];
	  pARC_FEATURE (pthis)->width      = tab[3];
	  pARC_FEATURE (pthis)->height     = tab[4];
	  pARC_FEATURE (pthis)->alphabegin = DEG2RAD(tab[5]);
	  pARC_FEATURE (pthis)->alphaend   = DEG2RAD(tab[6]);
	}
      else
	{
	  pARC_FEATURE (pthis)->width      = tab[2];
	  pARC_FEATURE (pthis)->height     = tab[3];
	  pARC_FEATURE (pthis)->alphabegin = DEG2RAD(tab[4]);
	  pARC_FEATURE (pthis)->alphaend   = DEG2RAD(tab[5]);
	}
      return 0;
      break;
    case SCI_TEXT:
      if ((*numrow * *numcol != 2)&&(*numrow * *numcol != 3))
	{
	  Scierror(999, _("Number of elements must be %d (%d if %s coordinate).\n"),2,3,"z");
	  return -1;
	}
      pTEXT_FEATURE (pthis)->x = tab[0];
      pTEXT_FEATURE (pthis)->y = tab[1];
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	pTEXT_FEATURE (pthis)->z = tab[2];
      return 0;
      break;
    case SCI_SEGS:
      if (pSEGS_FEATURE (pthis)->ptype <= 0) {
	if ((*numcol != 3)&&(*numcol != 2)) {
	  Scierror(999, _("Number of columns must be %d (%d if %s coordinate).\n"),2,3,"z");
	  return -1;
	}
	n1=pSEGS_FEATURE (pthis)->Nbr1;
	if (*numrow != n1) {
	  n1=*numrow;
	  if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	  if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    return -1;
	  }
	  if (*numcol == 3)
	    if ((pvz = MALLOC (n1 * sizeof (double))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
	      return -1;
	    }
	  if ((pstyle = MALLOC (n1 * sizeof (int))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    FREE(pvy); pvy = (double *) NULL;
	    FREE(pvz); pvz = (double *) NULL;
	    return -1;
	  }
	  FREE(pSEGS_FEATURE (pthis)->vx); pSEGS_FEATURE (pthis)->vx = NULL;
	  FREE(pSEGS_FEATURE (pthis)->vy); pSEGS_FEATURE (pthis)->vx = NULL;
	  if (*numcol == 3)
	    FREE(pSEGS_FEATURE (pthis)->vz); pSEGS_FEATURE (pthis)->vz = NULL;
	  /* Attention ici on detruit pstyle !! F.Leray 20.02.04*/
	  FREE(pSEGS_FEATURE (pthis)->pstyle); pSEGS_FEATURE (pthis)->pstyle = NULL;
	  for (i=0;i < *numrow;i++)
	    {
	      pvx[i] = tab[i];
	      pvy[i] = tab[i+ (*numrow)];
	      if (*numcol == 3)
		pvz[i] = tab[i+ 2*(*numrow)];
	      pstyle[i] = 0;
	    }
	  pSEGS_FEATURE (pthis)->vx=pvx;
	  pSEGS_FEATURE (pthis)->vy=pvy;
	  if (*numcol == 3)
	    pSEGS_FEATURE (pthis)->vz=pvz;
	  pSEGS_FEATURE (pthis)->Nbr1=n1;
	  pSEGS_FEATURE (pthis)->pstyle=pstyle;
	}
	else {
	  if ((*numcol == 3) && (pSEGS_FEATURE (pthis)->vz == NULL))
	    if ((pSEGS_FEATURE (pthis)->vz = MALLOC (n1 * sizeof (double))) == NULL) return -1;

	  for (i=0;i < *numrow;i++) {
	    pSEGS_FEATURE (pthis)->vx[i] = tab[i];
	    pSEGS_FEATURE (pthis)->vy[i] = tab[i+ (*numrow)];
	    if (*numcol == 3)
	      pSEGS_FEATURE (pthis)->vz[i] = tab[i+ 2*(*numrow)];
	  }
	}
      }
      else { /* Strange test object == Champ: e=gce(); e.data = e.data
	        make this error happened! Remove it to perform such legal operation */

	n1=pSEGS_FEATURE (pthis)->Nbr1;
	if (*numrow != n1) /* SS 30/1/02 */
	  {
	    n1=*numrow;
	    if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	    if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      return -1;
	    }
	    if ((pstyle = MALLOC (n1 * sizeof (int))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
	      FREE(pvz); pvz = (double *) NULL;
	      return -1;
	    }
	    if ((pvfx = MALLOC ((n1*n1) * sizeof (double))) == NULL) return -1;
	    if ((pvfy = MALLOC ((n1*n1) * sizeof (double))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
	      FREE(pvz); pvz = (double *) NULL;
	      FREE(pvfx); pvfx = (double *) NULL;
	      return -1;
	    }
	    if (*numcol == 3 +3*(*numrow * *numrow))
	      {
		if ((pvz = MALLOC (n1 * sizeof (double))) == NULL) {
		  FREE(pvx); pvx = (double *) NULL;
		  FREE(pvy); pvy = (double *) NULL;
		  return -1;
		}
		if ((pvfz = MALLOC ((n1*n1) * sizeof (double))) == NULL) {
		  FREE(pvx); pvx = (double *) NULL;
		  FREE(pvy); pvy = (double *) NULL;
		  FREE(pvz); pvz = (double *) NULL;
		  FREE(pvfx); pvfx = (double *) NULL;
		  FREE(pvfy); pvfy = (double *) NULL;
		  return -1;
		}
		FREE(pSEGS_FEATURE (pthis)->vz); pSEGS_FEATURE (pthis)->vz = NULL;
		FREE(pSEGS_FEATURE (pthis)->vfz); pSEGS_FEATURE (pthis)->vfz = NULL;
	      }
	    FREE(pSEGS_FEATURE (pthis)->vx); pSEGS_FEATURE (pthis)->vx = NULL;
	    FREE(pSEGS_FEATURE (pthis)->vy); pSEGS_FEATURE (pthis)->vy = NULL;
	    FREE(pSEGS_FEATURE (pthis)->vfx); pSEGS_FEATURE (pthis)->vfx = NULL;
	    FREE(pSEGS_FEATURE (pthis)->vfy); pSEGS_FEATURE (pthis)->vfy = NULL;
	    for (i=0;i < n1;i++)
	      {
		pvx[i] = tab[i];
		pvy[i] = tab[i+ (*numrow)];
		if (*numcol == 3 +3*(*numrow * *numrow))
		  pvz[i] = tab[i+ 2*(*numrow)];

	      }
	    k=3*n1;
	    for (i=0;i < n1*n1;i++)
	      {
		pvfx[i] = tab[k+i];
		pvfy[i] = tab[k+n1*n1+i];
		if (*numcol == 3 +3*(*numrow * *numrow))
		  pvfz[i] = tab[2*k+n1*n1+i];

	      }
	    pSEGS_FEATURE (pthis)->vx=pvx;
	    pSEGS_FEATURE (pthis)->vy=pvy;
	    pSEGS_FEATURE (pthis)->vx=pvfx;
	    pSEGS_FEATURE (pthis)->vy=pvfy;
	    pSEGS_FEATURE (pthis)->Nbr1=n1;
	    if (*numcol == 3 +3*(*numrow * *numrow))
	      {
		pSEGS_FEATURE (pthis)->vz=pvz;
		pSEGS_FEATURE (pthis)->vy=pvfz;
	      }

	  }
	else {
	  for (i=0;i < *numrow;i++)   {
	    pSEGS_FEATURE (pthis)->vx[i] = tab[i];
	    pSEGS_FEATURE (pthis)->vy[i] = tab[i+ (*numrow)];
	    if (pSEGS_FEATURE (pthis)->vz != (double *)NULL)
	      pSEGS_FEATURE (pthis)->vz[i] = tab[i+ 2*(*numrow)];
	  }
	  k=2* (*numrow);
	  k1=k+ (*numrow * *numrow);
	  k2=2*k+ (*numrow * *numrow);
	  for (i=0;i < *numrow * *numrow ;i++)   {
	    pSEGS_FEATURE (pthis)->vfx[i] = tab[k+i];
	    pSEGS_FEATURE (pthis)->vfy[i] = tab[k1+i];
	    if (pSEGS_FEATURE (pthis)->vfz != (double *)NULL)
	      pSEGS_FEATURE (pthis)->vfz[i] = tab[k2+i];
	  }
	}
      }
      return 0;
      break;


    case SCI_SURFACE:/* DJ.A 2003 */
      Scierror(999, _("Unhandled data field\n"));
      return -1;
      break;
    case SCI_GRAYPLOT:
      if (pGRAYPLOT_FEATURE (pthis)->type == 0) { /* gray plot */
	double *pvecx,*pvecy,*pvecz;
	int nx,ny;
	nx=*numrow-1;
	ny=*numcol-1;
	if (pGRAYPLOT_FEATURE (pthis)->ny!=ny || pGRAYPLOT_FEATURE (pthis)->nx!=nx) {
	  if ((pvecx = CALLOC(nx,sizeof(double))) == NULL) {
	    Scierror(999, _("%s: No more memory.\n"), "sciSetPoint") ;
	    return -1;}
	  if ((pvecy = CALLOC(ny,sizeof(double))) == NULL) {
	    FREE(pvecx);
	    Scierror(999, _("%s: No more memory.\n"), "sciSetPoint") ;
	    return -1;}
	  if ((pvecz = CALLOC(nx*ny,sizeof(double))) == NULL) {
	    FREE(pvecx);FREE(pvecy);
	    Scierror(999, _("%s: No more memory.\n"), "sciSetPoint") ;
	    return -1;}
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecx);pGRAYPLOT_FEATURE (pthis)->pvecx=pvecx;
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecy);pGRAYPLOT_FEATURE (pthis)->pvecy=pvecy;
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecz);pGRAYPLOT_FEATURE (pthis)->pvecz=pvecz;
	}
	for (i=0;i < nx;i++)
	  pGRAYPLOT_FEATURE (pthis)->pvecx[i] = tab[i+1];

	for (i=0;i < ny;i++)
	  pGRAYPLOT_FEATURE (pthis)->pvecy[i] = tab[*numrow*(i+1)];
	for (i=0;i < ny;i++)
	  for (k=0;k < nx;k++)
	    pGRAYPLOT_FEATURE (pthis)->pvecz[nx*i+k]=tab[*numrow*(i+1)+k+1];
	pGRAYPLOT_FEATURE (pthis)->ny=ny;
	pGRAYPLOT_FEATURE (pthis)->nx=nx;
      }
      else  {/* Matplot */
	double *pvecz;
	int nx,ny;
	nx=*numrow;
	ny=*numcol;
	if (pGRAYPLOT_FEATURE (pthis)->ny!=ny+1 || pGRAYPLOT_FEATURE (pthis)->nx!=nx+1) {
	  if ((pvecz = CALLOC(nx*ny,sizeof(double))) == NULL) {
	    Scierror(999, _("%s: No more memory.\n"), "sciSetPoint") ;
	    return -1;}
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecz);pGRAYPLOT_FEATURE (pthis)->pvecz=pvecz;
	}
	for (i=0;i < nx*ny;i++)
	  pGRAYPLOT_FEATURE (pthis)->pvecz[i]=tab[i];
	pGRAYPLOT_FEATURE (pthis)->ny=ny+1;
	pGRAYPLOT_FEATURE (pthis)->nx=nx+1;
      }
      break;
    case SCI_FEC:
      {
	double *pvecx,*pvecy,*pfun;
	int Nnode;
	if (*numcol != 3) {
	  Scierror(999, _("Number of columns must be %d.\n"),2);
	  return -1;}

	Nnode = *numrow;
	if (pFEC_FEATURE (pthis)->Nnode!=Nnode) {
	  if ((pvecx = CALLOC(Nnode,sizeof(double))) == NULL) {
	    Scierror(999, _("%s: No more memory.\n"), "sciSetPoint") ;
	    return -1;}
	  if ((pvecy = CALLOC(Nnode,sizeof(double))) == NULL) {
	    Scierror(999, _("%s: No more memory.\n"), "sciSetPoint") ;
	    FREE(pvecx);
	    return -1;}
	  if ((pfun = CALLOC(Nnode,sizeof(double))) == NULL) {
	    Scierror(999, _("%s: No more memory.\n"), "sciSetPoint") ;
	    FREE(pvecx);FREE(pvecy);
	    return -1;}
	  FREE( pFEC_FEATURE (pthis)->pvecx); pFEC_FEATURE (pthis)->pvecx=pvecx;
	  FREE( pFEC_FEATURE (pthis)->pvecy); pFEC_FEATURE (pthis)->pvecy=pvecy;
	  FREE( pFEC_FEATURE (pthis)->pfun); pFEC_FEATURE (pthis)->pfun=pfun;
	}
	for (i=0;i < Nnode;i++) {
	  pFEC_FEATURE (pthis)->pvecx[i]=tab[i];
	  pFEC_FEATURE (pthis)->pvecy[i]=tab[Nnode+i];
	  pFEC_FEATURE (pthis)->pfun[i]=tab[2*Nnode+i];
	}
      }
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      printSetGetErrorMessage("data");
      return -1;
      break;
    }
  return 0;
}

int sciInitdrawmode( BOOL mode )
{
  static sciPointObj * pobj ;
  pobj = sciGetFirstTypedSelectedSon(sciGetCurrentFigure(), SCI_SUBWIN);
  pSUBWIN_FEATURE(pobj)->visible = mode ;
  sciDrawObj(sciGetCurrentFigure ());
  return 0;
}

int
sciSetdrawmode (BOOL mode)
{
  static sciPointObj * pobj ;
  pobj = sciGetFirstTypedSelectedSon(sciGetCurrentFigure(), SCI_SUBWIN);
  if ( sciGetdrawmode( pobj ) == mode )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitdrawmode( mode ) ;

}

int sciSwitchWindow(int winnum)
{
  /* find if exist figure winnum */
  /* une autre methode c est de tester CurXGC->mafigure = NULL */
  if ( !sciIsExistingFigure(winnum) )
  {
    /* Figure winnum don't exist, create it! */
    if( createFullFigure(&winnum) == NULL)
    {
      return -1; /* failed to switch */
    }
  }
  else
  {
    sciSetCurrentFigure(getFigureFromIndex(winnum));
  }
  return 0;
}

/*----------------------------------------------------------------------------------*/
/**
 * In new graphic style, select a window and create one if not already done.
 */
int sciInitUsedWindow( int winNum )
{
  return sciSwitchWindow( winNum ) ; /* create the handle */
}

/*----------------------------------------------------------------------------------*/
/**
 * In new graphic style, select a window and create one if not already done.
 */
int sciSetUsedWindow( int winNum )
{
  /* select or create the window in the driver */
  if ( sciHasFigures() && sciGetNum( sciGetCurrentFigure() ) == winNum )
  {
    /* nothing to do */
    return 1 ;
  }

  return sciInitUsedWindow( winNum ) ;
}

/*----------------------------------------------------------------------------------*/



int sciInitIsFilled( sciPointObj * pobj, BOOL isfilled )
{
  switch (sciGetEntityType (pobj))
  {
  case SCI_LABEL:
    /* isfilled correspond to several properties in text object */
    sciInitIsBoxed(  pLABEL_FEATURE(pobj)->text, isfilled ) ;
    sciInitIsLine(   pLABEL_FEATURE(pobj)->text, isfilled ) ;
    return sciInitIsFilled( pLABEL_FEATURE(pobj)->text, isfilled ) ;
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pobj)->axes.filled = isfilled;
    return 0;
  default:
    if(sciGetGraphicContext(pobj) != NULL)
    {
      sciGetGraphicContext(pobj)->isfilled = isfilled;
      return 0;
    }
  }

  printSetGetErrorMessage("fill_mode");
  return -1;
}

/**sciSetIsFilled
 * Sets the filled line existence
 */
int
sciSetIsFilled (sciPointObj * pobj, BOOL isfilled)
{

  if ( sciGetIsFilled( pobj ) == isfilled )
  {
    return 1 ;
  }
  return sciInitIsFilled( pobj, isfilled ) ;

}


int sciInitBoxType( sciPointObj * pobj, EAxesBoxType type )
{
  switch (sciGetEntityType (pobj))
  {
    case SCI_SUBWIN:
      pSUBWIN_FEATURE(pobj)->axes.rect = type ;
      /* redondant information in scilab :'( */
      switch ( type )
      {
        case BT_OFF:
          pSUBWIN_FEATURE(pobj)->axes.flag[2] = 0 ;
          break ;
        case BT_ON:
          pSUBWIN_FEATURE(pobj)->axes.flag[2] = 4 ;
          break ;
        case BT_HIDDEN_AXES:
          pSUBWIN_FEATURE(pobj)->axes.flag[2] = 2 ;
          break ;
        case BT_BACK_HALF:
          pSUBWIN_FEATURE(pobj)->axes.flag[2] = 3 ;
          break ;
      }
      return 0;
      break;
    default:
      printSetGetErrorMessage("box_type");
      return -1 ;
      break;
  }
}

/* set the property of the axes box  */
int sciSetBoxType( sciPointObj * pobj, EAxesBoxType type )
{

  if ( sciGetBoxType( pobj ) == type )
  {
    return 1 ;
  }
  return sciInitBoxType( pobj, type ) ;

}


int sciInitIsBoxed( sciPointObj * pobj, BOOL isboxed )
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      pTEXT_FEATURE(pobj)->isboxed = isboxed;
      return 0;
      break;
    case SCI_SUBWIN:
      Scierror(999, _("%s: please use %s instead.\n"),"sciSetIsBoxed","sciSetBoxType");
      if ( isboxed )
      {
        pSUBWIN_FEATURE(pobj)->axes.rect = BT_ON ;
      }
      else
      {
        pSUBWIN_FEATURE(pobj)->axes.rect = BT_OFF ;
      }
      return 0;
      break;
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
      return -1;
      break;
    }

}

/**sciSetIsBoxed
 * Sets the box existence
 */
int
sciSetIsBoxed (sciPointObj * pobj, BOOL isboxed)
{

  if ( sciGetIsBoxed( pobj ) == isboxed )
  {
    return 1 ;
  }
  return sciInitIsBoxed( pobj, isboxed ) ;

}


int
sciSetInterpVector(sciPointObj * pobj, int size, int * value)
{
  int i;

  FREE(pPOLYLINE_FEATURE(pobj)->scvector);

  if((pPOLYLINE_FEATURE(pobj)->scvector = MALLOC(size*sizeof(int)))==NULL){
    Scierror(999, _("%s: No more memory.\n"), "sciSetInterpVector") ;
    return -1;
  }

  for(i=0;i<size;i++)
    pPOLYLINE_FEATURE(pobj)->scvector[i] = value[i];

  return 0;
}

int sciInitAutoRotation( sciPointObj * pObj, BOOL value )
{
  switch (sciGetEntityType (pObj))
  {
  case SCI_LABEL:
    pLABEL_FEATURE(pObj)->auto_rotation = value ;
    return 0;
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
    printSetGetErrorMessage("auto_rotation");
    return -1 ;
    break;
  }

}

/* set the auto_rotation property of an object */
int sciSetAutoRotation ( sciPointObj * pObj, BOOL value )
{

  if ( sciGetAutoRotation( pObj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitAutoRotation( pObj, value ) ;

}


int sciInitAutoPosition( sciPointObj * pObj, BOOL value )
{
  switch (sciGetEntityType (pObj))
  {
  case SCI_LABEL:
    pLABEL_FEATURE(pObj)->auto_position = value ;
    return 0;
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
    printSetGetErrorMessage("auto_position");
    return -1 ;
    break;
  }

}

/* set the auto_position property of an object */
int sciSetAutoPosition ( sciPointObj * pObj, BOOL value )
{

  if ( sciGetAutoPosition( pObj ) == value )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitAutoPosition( pObj, value ) ;

}

/*---------------------------------------------------------------------------*/
int checkPercent( char * string )
{
  /* check for the percent in the string */
  char * firstPercent  = strchr( string, '%' ) ;

  if ( firstPercent == NULL )
  {
    /* no % character found */
    return 0 ;
  }
  else if ( firstPercent[1] != 'd' )
  {
    /* a %something with something != d has been found */
    return -1 ;
  }
  else
  {
    /* here we have found a first %d, check if there is not any more % */
    firstPercent++ ;
    firstPercent = strchr( firstPercent, '%' ) ;
    if ( firstPercent == NULL )
    {
      /* no other % character found */
      return 1 ;
    }
    else
    {
      return -1 ;
    }
  }
}
/*---------------------------------------------------------------------------*/
int sciInitAutoSize( sciPointObj * pObj, BOOL autoSize )
{
  switch ( sciGetEntityType (pObj) )
  {
  case SCI_TEXT:
    pTEXT_FEATURE(pObj)->autoSize = autoSize ;
    return 0 ;
  default:
   printSetGetErrorMessage("text_box_mode");
    return -1 ;
    break;
  }

}
/*---------------------------------------------------------------------------*/
int sciSetAutoSize( sciPointObj * pObj, BOOL autoSize )
{
  if ( sciGetAutoSize( pObj ) == autoSize )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitAutoSize( pObj, autoSize ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitAlignment( sciPointObj * pObj, sciTextAlignment align )
{
  switch (sciGetEntityType (pObj))
  {
  case SCI_TEXT:
    pTEXT_FEATURE(pObj)->stringsAlign = align ;
    return 0 ;
  default:
    printSetGetErrorMessage("alignment");
    return -1 ;
  }

}
/*-----------------------------------------------------------------------------------*/
int sciSetAlignment( sciPointObj * pObj, sciTextAlignment align )
{
  if ( sciGetAlignment( pObj ) == align )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitAlignment( pObj, align ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitUserSize( sciPointObj * pObj, double width, double height )
{
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_TEXT:
    pTEXT_FEATURE(pObj)->userSize[0] = width ;
    pTEXT_FEATURE(pObj)->userSize[1] = height ;
    return 0 ;
  default:
    printSetGetErrorMessage("text_box");
    return -1 ;
  }
}
/*-----------------------------------------------------------------------------------*/
int sciSetUserSize( sciPointObj * pObj, double width, double height )
{
  double curWidth  ;
  double curHeight ;
  sciGetUserSize( pObj, &curWidth, &curHeight ) ;
  if ( curWidth == width && curHeight == height )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitUserSize( pObj, width, height ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitCenterPos( sciPointObj * pObj, BOOL newCP )
{
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_TEXT:
    pTEXT_FEATURE(pObj)->centeredPos = newCP ;
    return 0 ;
  default:
    printSetGetErrorMessage("data");
    return -1 ;
  }

}
/*-----------------------------------------------------------------------------------*/
int sciSetCenterPos( sciPointObj * pObj, BOOL newCP )
{
  if ( sciGetCenterPos( pObj ) == newCP )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitCenterPos( pObj, newCP ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitIs3d(  sciPointObj * pObj, BOOL is3d )
{
   switch( sciGetEntityType( pObj ) )
   {
   case SCI_SUBWIN:
     if ( is3d )
     {
       pSUBWIN_FEATURE (pObj)->is3d = TRUE ;
       Obj_RedrawNewAngle( pObj,
                           pSUBWIN_FEATURE (pObj)->alpha_kp,
                           pSUBWIN_FEATURE (pObj)->theta_kp ) ;
     }
     else
     {
       /* switch to 2d */
       if ( sciGetSurface(pObj) == NULL)
       {
         pSUBWIN_FEATURE (pObj)->is3d = FALSE;
         pSUBWIN_FEATURE (pObj)->project[2]= 0;
       }
       pSUBWIN_FEATURE (pObj)->theta_kp = pSUBWIN_FEATURE (pObj)->theta;
       pSUBWIN_FEATURE (pObj)->alpha_kp = pSUBWIN_FEATURE (pObj)->alpha;
       pSUBWIN_FEATURE (pObj)->alpha = 0.0;
       pSUBWIN_FEATURE (pObj)->theta = 270.0;
       pSUBWIN_FEATURE(pObj)->is3d = FALSE;
       return 0 ;
     }
     return 0 ;
   case SCI_TEXT:
     pTEXT_FEATURE( pObj )->is3d = is3d ;
     return 0 ;
   case SCI_LABEL:
     return sciInitIs3d( pLABEL_FEATURE( pObj )->text, is3d ) ;
   default:
     printSetGetErrorMessage("view");
     return -1 ;
   }
}
/*-----------------------------------------------------------------------------------*/
/**
 * Force an object to be displayed in 2d or 3d mode.
 */
int sciSetIs3d( sciPointObj * pObj, BOOL is3d )
{
  if ( sciGetIs3d( pObj ) == is3d )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitIs3d( pObj, is3d ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitHiddenColor( sciPointObj * pObj, int newColor )
{
  if(!sciCheckColorIndex(pObj, newColor)) { return -1;}

  switch( sciGetEntityType( pObj ) )
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->hiddencolor = newColor;
    return 0 ;
  case SCI_SURFACE:
    pSURFACE_FEATURE(pObj)->hiddencolor = newColor;
    return 0;
  default:
    printSetGetErrorMessage("hidden_color");
    return -1 ;
  }
}
/*-----------------------------------------------------------------------------------*/
int sciSetHiddenColor( sciPointObj * pObj, int newColor )
{
  if ( sciGetHiddenColor( pObj ) == newColor )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitHiddenColor( pObj, newColor ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitHiddenAxisColor( sciPointObj * pObj, int newColor )
{
  int m = sciGetNumColors(pObj);
  if(newColor < -2 || newColor > m + 2) return 0;

  newColor = sciSetGoodIndex(pObj,newColor);

  switch( sciGetEntityType( pObj ) )
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->axes.hiddenAxisColor = Max (0, Min (newColor - 1, m + 1));
    return 0 ;
  default:
    printSetGetErrorMessage("hidden_axis_color");
    return -1 ;
  }
}
/*-----------------------------------------------------------------------------------*/
int sciSetHiddenAxisColor( sciPointObj * pObj, int newColor )
{
  if ( sciGetHiddenAxisColor( pObj ) == newColor )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitHiddenAxisColor( pObj, newColor ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle )
{
  if (    !sciCheckColorIndex(pObj, xStyle)
       || !sciCheckColorIndex(pObj, yStyle)
       || !sciCheckColorIndex(pObj, zStyle))
  {
    return -1;
  }

  switch( sciGetEntityType( pObj ) )
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->grid[0] = xStyle ;
    pSUBWIN_FEATURE(pObj)->grid[1] = yStyle ;
    pSUBWIN_FEATURE(pObj)->grid[2] = zStyle ;
    return 0 ;
  default:
   printSetGetErrorMessage("grid");
    return -1 ;
  }
}
/*-----------------------------------------------------------------------------------*/
/**
 * Set the grid of an axes object
 */
int sciSetGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle )
{
  int curX ;
  int curY ;
  int curZ ;
  sciGetGridStyle( pObj, &curX, &curY, &curZ ) ;
  if ( curX == xStyle && curY == yStyle && curZ == zStyle )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitGridStyle( pObj, xStyle, yStyle, zStyle ) ;
}
/*-----------------------------------------------------------------------------------*/
/**
 * Set the viewport property of a figure.
 * Effective only if the auto_resize property is enable
 */
int sciSetViewport( sciPointObj * pObj, const int viewport[4] )
{
  switch( sciGetEntityType( pObj ) )
  {
  case SCI_FIGURE:
    if (isFigureModel(pObj))
    {
      pFIGURE_FEATURE(pObj)->pModelData->viewport[0] = viewport[0];
      pFIGURE_FEATURE(pObj)->pModelData->viewport[1] = viewport[1];
      pFIGURE_FEATURE(pObj)->pModelData->viewport[2] = viewport[2];
      pFIGURE_FEATURE(pObj)->pModelData->viewport[3] = viewport[3];
    }
    else
    {
      sciSetJavaViewport(pObj, viewport);
    }
    return 0;
  default:
    printSetGetErrorMessage("viewport");
    return -1 ;
  }

}
/*-----------------------------------------------------------------------------------*/
int sciInitInfoMessage(sciPointObj * pObj, const char * newMessage)
{
	if ( sciGetEntityType( pObj ) == SCI_FIGURE)
	{

		/* first case newName is NULL */
		if (newMessage == NULL)
		{
			/* Just set an empty title for the physical window if needed */
			if(isFigureModel(pObj))
			{
				pFIGURE_FEATURE(pObj)->pModelData->infoMessage = NULL;
			}
			else
			{
				sciSetJavaInfoMessage(pObj, "");
			}
				
			return 0;
		}

		if (isFigureModel(pObj))
		{
			/* Copy the message into the special data */
			int newMessageLength = (int) strlen(newMessage);
			pFIGURE_FEATURE(pObj)->pModelData->infoMessage = MALLOC((newMessageLength + 1) * sizeof(char));
			if (pFIGURE_FEATURE(pObj)->pModelData->infoMessage != NULL)
			{
				strcpy(pFIGURE_FEATURE(pObj)->pModelData->infoMessage, newMessage);
			}
		}
		else
		{
			/* Copy in the Java data */
			sciSetJavaInfoMessage(pObj, newMessage);
		}

		return 0 ;
	}
	else
	{
		printSetGetErrorMessage("info_message");
		return -1;
	}
}
/*-----------------------------------------------------------------------------------*/
/**
 * Modify the string in the info bar of the graphic window
 */
int sciSetInfoMessage( sciPointObj * pObj, const char * newMessage )
{
  if (isFigureModel(pObj) && pFIGURE_FEATURE(pObj)->pModelData->infoMessage != NULL)
	{
		FREE(pFIGURE_FEATURE(pObj)->pModelData->infoMessage);
		pFIGURE_FEATURE(pObj)->pModelData->infoMessage = NULL;
	}
	return sciInitInfoMessage(pObj, newMessage);
}
/*-----------------------------------------------------------------------------------*/
int sciInitEventHandler( sciPointObj * pObj, char * name )
{
  if( sciGetEntityType( pObj ) == SCI_FIGURE)
    {
      sciFigure * ppFigure = pFIGURE_FEATURE(pObj) ;

      if ( ppFigure == NULL )
	{
	  Scierror(999, _("%s: No more memory.\n"), "sciInitEventHandler") ;
	  return -1 ;
	}

      if ( ppFigure->eventHandler != NULL )
	{
	  FREE(ppFigure->eventHandler);
	}

      ppFigure->eventHandler = strdup(name);

      /* Java is called to set the listener */
      if (!isFigureModel(pObj))
        {
          setFigureEventHandler(sciGetNum(pObj), name);
        }

      if ( strcmp( name, "" ) == 0 )
	{
	  sciInitIsEventHandlerEnable( pObj, FALSE ) ;
	}

      return 0 ;
    }
  else
    {
      printSetGetErrorMessage("event_handler");
      return -1 ;
    }

}
/*-----------------------------------------------------------------------------------*/
int sciSetEventHandler( sciPointObj * pObj, char * name )
{
  if ( strcmp( sciGetEventHandler(pObj), name ) == 0 )
  {
    /* nothing to do */
    return 1 ;
  }

  return sciInitEventHandler( pObj, name ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitIsEventHandlerEnable( sciPointObj * pObj, BOOL enable )
{
  if(sciGetEntityType( pObj ) == SCI_FIGURE)
    {
      sciFigure * ppFigure = pFIGURE_FEATURE(pObj) ;
      /*
      ** If we try to enable an empty Event Handler
      ** raise a Warning.
      */
      if (ppFigure != NULL
	  && ppFigure->eventHandler != NULL
	  && (strcmp(ppFigure->eventHandler, "") != 0 || enable == FALSE))
	{
	  pFIGURE_FEATURE(pObj)->isEventHandlerEnable = enable ;

	  /* Java is called to enable or disable the listener */
	  if (!isFigureModel(pObj))
	    {
	      setFigureEventHandlerEnabled(sciGetNum(pObj), enable);
	    }
	}
      else
	{
	  Scierror(999, _("%s: Can't enable a void event handler.\n"), "sciInitEventHandler") ;
	}
      return 0 ;
    }
  else
    {
      printSetGetErrorMessage("event_handler");
      return -1 ;
    }
}
/*-----------------------------------------------------------------------------------*/
int sciSetIsEventHandlerEnable( sciPointObj * pObj, BOOL enable )
{
  if ( sciGetIsEventHandlerEnable(pObj) == enable )
  {
    /* nothing to do */
    return 1 ;
  }

  return sciInitIsEventHandlerEnable( pObj, enable ) ;
}
/*-----------------------------------------------------------------------------------*/
/**
 * Set data-bounds defined by the user.
 * @param bounds [Xmin,Xmax,Ymain,Ymax,Zmin,Zmax] vector.
 */
int sciSetDataBounds( sciPointObj * pObj, double bounds[6] )
{
  int i;
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    for ( i = 0 ; i < 6 ; i++ )
    {
      pSUBWIN_FEATURE(pObj)->SRect[i] = bounds[i] ;
    }
    return 0;
  case SCI_SURFACE:
    for ( i = 0 ; i < 6 ; i++ )
    {
      pSURFACE_FEATURE(pObj)->ebox[i] = bounds[i] ;
    }
    return 0;
  default:
    printSetGetErrorMessage("data_bounds");
    return -1 ;

  }
}
/*-----------------------------------------------------------------------------------*/
/**
 * Set the displayed data bounds of a subwin object.
 */
int sciSetRealDataBounds(sciPointObj * pObj, const double bounds[6])
{
  int i;
  switch( sciGetEntityType(pObj) )
  {
  case SCI_SUBWIN:
    for ( i = 0 ; i < 6 ; i++ )
    {
      pSUBWIN_FEATURE(pObj)->FRect[i] = bounds[i] ;
    }
    return 0;
  default:
    printSetGetErrorMessage("data_bounds");
    return -1 ;

  }
}
/*--------------------------------------------------------------------------------------------*/
int sciInitViewingAngles( sciPointObj * pObj, double alpha, double theta)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->alpha = alpha;
    pSUBWIN_FEATURE(pObj)->theta = theta;
    return 0;
  default:
    printSetGetErrorMessage("rotation_angles");
	return -1;
  }

}
/*-----------------------------------------------------------------------------------*/
/**
 * Set the viewing angles (in degrees) of a subwindow
 */
int sciSetViewingAngles( sciPointObj * pObj, double alpha, double theta)
{
  double curAlpha;
  double curTheta;
  sciGetViewingAngles(pObj, &curAlpha, &curTheta);
  if ( curAlpha == alpha && curTheta == theta )
  {
    /* nothing to do */
    return 1;
  }

  return sciInitViewingAngles(pObj, alpha, theta) ;

}
/*-----------------------------------------------------------------------------------*/
/**
 * Set the info message of a figure with the information about rotation angles
 */
int setInfoMessageWithRotationAngles(sciPointObj * pFigure, double alpha, double theta)
{
  /* keep this lower than 10*/
  #define ANGLE_DECIMAL_NUMBERS 1
  char angleDisplayFormat[5];
  int returnStatus = -1;
  double intPart;
  char * infoMessage = NULL;
  char formatedInfoMessage[29];

  /* Size without numbers is 8 for "alpha = ", 10 for ", theta = " and 1 for the null terminating character */
  int infoMessageSize = 19;

  /* set number of digits so format is %.xf where x is ANGLE_DECIMAL_NUMBERS */
  sprintf(angleDisplayFormat, "%%.%df", ANGLE_DECIMAL_NUMBERS);

  /* compute size of alpha wich is the length of its int part plus 1 for the dot */
  /* and the number of decimals */
  modf(alpha, &intPart); /* get int part of alpha */
  infoMessageSize += GET_NB_DIGITS(intPart) + 1 + ANGLE_DECIMAL_NUMBERS;

  /* same for theta */
  modf(theta, &intPart);
  infoMessageSize += GET_NB_DIGITS(intPart) + 1 + ANGLE_DECIMAL_NUMBERS;

  /* We use also infomessage string to store the formated massage */
  /* The needed size might be 19 plus twice the format length so 8 => 27 */

  /* Add alpha size, which is the size of its int part plus 1 for the dot and only  */

  infoMessage = MALLOC(infoMessageSize * sizeof(char));
  if (infoMessage == NULL)
  {
    /* no more memory */
    return -1;
  }

  /* Put the formats in the string */
  sprintf(formatedInfoMessage, "alpha = %s, theta = %s", angleDisplayFormat, angleDisplayFormat);

  /* convert the formats into the angle values */
  sprintf(infoMessage, formatedInfoMessage, alpha, theta);

  returnStatus = sciSetInfoMessage(pFigure, infoMessage) ;

  FREE(infoMessage);

  return returnStatus;

  #undef ANGLE_DECIMAL_NUMBERS
}
/*-----------------------------------------------------------------------------------*/
/**
 * To set the pixmap mode of a figure to on or off
 */
int sciInitPixmapMode(sciPointObj * pObj, BOOL onOrOff)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_FIGURE:
    pFIGURE_FEATURE(pObj)->pixmapMode = onOrOff;
    return 0;
  default:
    printSetGetErrorMessage("pixmap");
	return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * To set the pixmap mode of a figure to on or off
 */
int sciSetPixmapMode(sciPointObj * pObj, BOOL onOrOff)
{
  if (sciGetPixmapMode(pObj) == onOrOff)
  {
    /* Nothing to do */
    return 1;
  }
  return sciInitPixmapMode(pObj, onOrOff);
}
/*----------------------------------------------------------------------------------*/
int sciInitTextPos( sciPointObj * pObj, double posX, double posY, double posZ)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_TEXT:
    pTEXT_FEATURE(pObj)->x = posX;
    pTEXT_FEATURE(pObj)->y = posY;
    pTEXT_FEATURE(pObj)->z = posZ;
    return 0;
  case SCI_LABEL:
    return sciInitTextPos(pLABEL_FEATURE(pObj)->text, posX, posY, posZ);
  default:
    printSetGetErrorMessage("data");
    return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Set the position of a label or text object.
 */
int sciSetTextPos( sciPointObj * pObj, double posX, double posY, double posZ)
{
  double curPos[3];
  sciGetTextPos(pObj, curPos);
  if ( curPos[0] == posX && curPos[1] == posY && curPos[2] == posZ )
  {
    /* nothing to do */
    return 1;
  }
  return sciInitTextPos(pObj, posX, posY, posZ);
}
/*----------------------------------------------------------------------------------*/
int sciInitLogFlags(sciPointObj * pObj, char logFlags[3])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->logflags[0] = logFlags[0];
    pSUBWIN_FEATURE(pObj)->logflags[1] = logFlags[1];
    pSUBWIN_FEATURE(pObj)->logflags[2] = logFlags[2];



    return 0;
  default:
    printSetGetErrorMessage("log_flags");
    return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Set the log flags of a subwindow
 */
int sciSetLogFlags(sciPointObj * pObj, char logFlags[3])
{
  char curLogFlags[3];
  int status;
  sciGetLogFlags(pObj, curLogFlags);
  if (   logFlags[0] == curLogFlags[0] && logFlags[1] == curLogFlags[1]
      && logFlags[2] == curLogFlags[2])
  {
    // nothing to do
    return 1;
  }
  status = sciInitLogFlags(pObj, logFlags);

  // force redraw of all children of the object.
  if (status == 0 && pObj != getAxesModel())
  {
    forceHierarchyRedraw(pObj);
  }

  return status;
}
/*----------------------------------------------------------------------------------*/
int sciInitAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->axes.auto_ticks[0] = autoTicksX;
    pSUBWIN_FEATURE(pObj)->axes.auto_ticks[1] = autoTicksY;
    pSUBWIN_FEATURE(pObj)->axes.auto_ticks[2] = autoTicksZ;
    return 0;
  default:
    printSetGetErrorMessage("auto_ticks");
    return -1;
  }
}
/*----------------------------------------------------------------------------------*/
int sciSetAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ)
{
  BOOL curAutoTicks[3];
  sciGetAutoTicks(pObj, curAutoTicks);

  if (curAutoTicks[0] == autoTicksX && curAutoTicks[1] == autoTicksY && curAutoTicks[2] == autoTicksZ)
  {
    // nothing to do
    return 1;
  }

  return sciInitAutoTicks(pObj, autoTicksX, autoTicksY, autoTicksZ);

}
/*----------------------------------------------------------------------------------*/
/**
 * Specify a new zoom box for a subwin object.
 * @param zoomBox [xMin, xMax, yMin, yMax, zMin, zMax] vector.
 */
int sciSetZoomBox(sciPointObj * pObj, const double zoomBox[6])
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->ZRect[0] = zoomBox[0];
    pSUBWIN_FEATURE(pObj)->ZRect[1] = zoomBox[1];
    pSUBWIN_FEATURE(pObj)->ZRect[2] = zoomBox[2];
    pSUBWIN_FEATURE(pObj)->ZRect[3] = zoomBox[3];
    pSUBWIN_FEATURE(pObj)->ZRect[4] = zoomBox[4];
    pSUBWIN_FEATURE(pObj)->ZRect[5] = zoomBox[5];
    return 0;
  default:
    printSetGetErrorMessage("zoom_box");
    return -1;
  }
}
/*----------------------------------------------------------------------------------*/
int sciInitImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw)
{
  switch(sciGetEntityType(pObj))
  {
  case SCI_FIGURE:
    pFIGURE_FEATURE(pObj)->auto_redraw = autoRedraw;
    return 0;
  default:
    printSetGetErrorMessage("immediate_drawing");
    return -1;
  }
}
/*----------------------------------------------------------------------------------*/
int sciSetImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw)
{
  if (sciGetImmediateDrawingMode(pObj) == autoRedraw)
  {
    // nothing to do
    return 1;
  }
  return sciInitImmediateDrawingMode(pObj, autoRedraw);
}
/*----------------------------------------------------------------------------------*/
int sciInitUseNurbs(sciPointObj * pObj, BOOL useNurbs)
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_SUBWIN:
    pSUBWIN_FEATURE(pObj)->useNurbsForArcs = useNurbs;
    return 0;
  case SCI_ARC:
    pARC_FEATURE(pObj)->useNurbs = useNurbs;
    return 0;
  default:
    printSetGetErrorMessage("drawing_method");
    return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Return whether the drawing of an arc object uses nurbs or lines
 */
int sciSetUseNurbs(sciPointObj * pObj, BOOL useNurbs)
{
  if (sciGetUseNurbs(pObj) == useNurbs)
  {
    // nothing to do
    return 1;
  }
  return sciInitUseNurbs(pObj, useNurbs);
}
/*----------------------------------------------------------------------------------*/
int sciInitIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics)
{
  sciFont * fontContext = sciGetFontContext(pObj);
  if (fontContext != NULL)
  {
    fontContext->useFractionalMetrics = useFractionalMetrics;
    return 0;
  }
  printSetGetErrorMessage("fractional_font");
  return -1;
}
/*----------------------------------------------------------------------------------*/
/**
* Return whether an arc object uses fractional metrics for text display
*/
int sciSetIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics)
{
  if (sciGetIsUsingFractionalMetrics(pObj) == useFractionalMetrics)
  {
    // nothing to do
    return 1;
  }
  return sciInitIsUsingFractionalMetrics(pObj, useFractionalMetrics);
}
/*----------------------------------------------------------------------------------*/
int sciInitColorRange(sciPointObj * pObj, int subset[2])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_FEC:
		pFEC_FEATURE(pObj)->colminmax[0] = subset[0];
		pFEC_FEATURE(pObj)->colminmax[1] = subset[1];
		return 0;
  default:
    printSetGetErrorMessage("color_range");
		return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Modify the subset of colormap bounds used by a particular object (colminmax).
 */
int sciSetColorRange(sciPointObj * pObj, int subset[2])
{

	int curColorRange[2];
	sciGetColorRange(pObj, curColorRange);
	if (curColorRange[0] == subset[0] && curColorRange[1] == subset[1])
	{
		/* nothing to do */
		return 1;
	}

	return sciInitColorRange(pObj, subset);

}
/*----------------------------------------------------------------------------------*/
int sciInitOutsideColors(sciPointObj * pObj, int colors[2])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_FEC:
		pFEC_FEATURE(pObj)->colout[0] = colors[0];
		pFEC_FEATURE(pObj)->colout[1] = colors[1];
		return 0;
  default:
    printSetGetErrorMessage("outside_color");
		return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Modify the color to use for an objects when it uses index outside of the colormap (colout).
 */
int sciSetOutsideColors(sciPointObj * pObj, int colors[2])
{

	int curColors[2];
	sciGetOutsideColor(pObj, curColors);
	if (curColors[0] == colors[0] && curColors[1] == colors[1])
	{
		/* nothing to do */
		return 1;
	}

  return sciInitOutsideColors(pObj, colors);
}
/*----------------------------------------------------------------------------------*/
int sciInitZBounds(sciPointObj * pObj, double bounds[2])
{
  switch (sciGetEntityType(pObj))
  {
  case SCI_FEC:
		pFEC_FEATURE(pObj)->zminmax[0] = bounds[0];
		pFEC_FEATURE(pObj)->zminmax[1] = bounds[1];
		return 0;
  default:
    printSetGetErrorMessage("z_bounds");
		return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Modify the Z range used by a fec object (zminmax).
 */
int sciSetZBounds(sciPointObj * pObj, double bounds[2])
{
  double curBounds[2];
	sciGetZBounds(pObj, curBounds);
	if (curBounds[0] == bounds[0] && curBounds[1] == bounds[1])
	{
		/* nothing to do */
		return 1;
	}

	return sciInitZBounds(pObj, bounds);
}
/*----------------------------------------------------------------------------------*/
int sciInitGridFront(sciPointObj * pObj, BOOL gridFront)
{
  switch (sciGetEntityType(pObj))
  {
	case SCI_SUBWIN:
		pSUBWIN_FEATURE(pObj)->gridFront = gridFront;
		return 0;
	default:
    printSetGetErrorMessage("grid_position");
		return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Modify whether the grid is drawn in background or foreground.
 */
int sciSetGridFront(sciPointObj * pObj, BOOL gridFront)
{
	if (sciGetGridFront(pObj) == gridFront)
	{
		/* nothing to do */
		return 1;
	}
	return sciInitGridFront(pObj, gridFront);
}
/*----------------------------------------------------------------------------------*/
int sciInitAntialiasingQuality(sciPointObj * pObj, int quality)
{
  switch (sciGetEntityType(pObj))
  {
	case SCI_FIGURE:
		if (isFigureModel(pObj))
		{
			pFIGURE_FEATURE(pObj)->pModelData->antialiasingQuality = quality;
		}
		else
		{
			sciSetJavaAntialiasingQuality(pObj, quality);
		}
		return 0;
  default:
    printSetGetErrorMessage("anti_aliasing");
		return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Modify the quality of antialiasing or disable it.
 * If quality if 0, the antialiasing is disabled,
 * otherwise it might be either 1, 2, 4, 8 or 16 and then
 * specifies the number of pass for antialiasing.
 * @param quality positive integer.
 */
int sciSetAntialiasingQuality(sciPointObj * pObj, int quality)
{
  if (sciGetAntialiasingQuality(pObj) == quality)
	{
		/* nothing to do */
		return 1;
	}
	return sciInitAntialiasingQuality(pObj, quality);
}
/*----------------------------------------------------------------------------------*/
int sciInitLegendLocation(sciPointObj * pObj, sciLegendPlace location)
{
	switch (sciGetEntityType(pObj))
  {
	case SCI_LEGEND:
		pLEGEND_FEATURE(pObj)->place = location;
		return 0;
	default:
    printSetGetErrorMessage("legend_location");
		return -1;
  }
}
/*----------------------------------------------------------------------------------*/
/**
 * Modify the legend position relatively to the subwindow
 */
int sciSetLegendLocation(sciPointObj * pObj, sciLegendPlace location)
{
	if (sciGetLegendLocation(pObj) == location)
	{
		/* nothing to do */
		return 1;
	}
	return sciInitLegendLocation(pObj, location);
}
/*----------------------------------------------------------------------------------*/
/**
 * Check that a color index is within the colormap range or not
 * @param pObj object conatining the color
 */
BOOL sciCheckColorIndex(sciPointObj * pObj, int colorIndex)
{
  return (colorIndex >= -2) && (colorIndex <= sciGetNumColors(pObj) + 2);
}
/*----------------------------------------------------------------------------------*/
