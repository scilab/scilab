/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to SET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "bcg.h"
#include "DrawObjects.h"
#include "BuildObjects.h"


extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;

/* sciClipTab ptabclip[15]; */
BOOL modereplay = FALSE;

extern int cf_type;

void Obj_RedrawNewAngle(sciPointObj *psubwin,double theta,double alpha)
{
  /**dj20003***/ 
  if ((alpha == 0.0) && (theta == 270.0))
    {
      pSUBWIN_FEATURE (psubwin)-> is3d = FALSE; 
      return;
    }
  
  pSUBWIN_FEATURE (psubwin)->alpha = alpha;
  pSUBWIN_FEATURE (psubwin)->theta  = theta;
  pSUBWIN_FEATURE (psubwin)-> is3d = TRUE;
  if ((alpha == 0.0 ) || (alpha == 180.0 ) || (alpha == -180.0 ))  /* DJ.A 30/12 */
    pSUBWIN_FEATURE (psubwin)->project[2]= 0; /* no z to display */
  else 
    {
      pSUBWIN_FEATURE (psubwin)->project[2]= 1; /* z must be displayed */
      if (((alpha == 90.0 ) || (alpha == 270.0 )|| (alpha == -90.0) || (alpha == -270.0 ))
	  && ((theta == 90.0 ) || (theta == -90.0 )
	      || (theta == 270.0 )|| (theta == -270.0 )))
	pSUBWIN_FEATURE (psubwin)->project[1]= 0; /* no y to display */
      else
	{
	  pSUBWIN_FEATURE (psubwin)->project[1]= 1;
	  if (((alpha == 90.0 )|| (alpha == 270.0 )|| (alpha == -90.0) || (alpha == -270.0 ))  
	      && ((theta == 0.0 ) || (theta == 180.0 )|| (alpha == -180.0 )))
	    pSUBWIN_FEATURE (psubwin)->project[0]= 0; /* BUG evreywhere when theta == 0 */
	  else
	    pSUBWIN_FEATURE (psubwin)->project[0]= 1;
	}
    }
}



/**sciSetEntityType
 * @memo Sets the type of this Scilab graphic object (pobj)
 */
void
sciSetEntityType (sciPointObj * pobj, sciEntityType value)
{
  if (pobj != (sciPointObj *) NULL)
    pobj->entitytype = value;
}



/**sciSetColormap
 * This function sets a colormap to the figure. It's the same for all sons. 
 * Setting the colormap a must be a m x 3 double RGB matrix:  
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE 
 */
int
sciSetColormap (sciPointObj * pobj, double *rgbmat, integer m, integer n)
{
  /*  double *pc;*/
  int k,old_m,m1;
  sciPointObj * pcurwin;
  double *cmap;
  int succeed = 0;

 if(n != 3){
    sciprint("colormap : number of colums must be 3\n");
    return 0;
  }
  
  if(SCI_FIGURE != sciGetEntityType(pobj)){
    sciprint("sciSetColormap Error: Object must be a SCI_FIGURE\n");
    return 0;
  }

  old_m = sciGetNumColors(pobj);
  m1=m;
  if (pobj != pfiguremdl) {
    pcurwin=sciGetCurrentFigure ();
    sciSetCurrentFigure ( pobj);
    /*It should be impossible to set the colormap because of restriction on max 
      number of colors. In this case the old one is kept*/
    C2F(dr)("xset","colormap",&m,&n,&succeed,PI0,PI0,PI0,rgbmat,PD0,PD0,PD0,0L,0L);
    sciSetCurrentFigure (pcurwin);
    m1=sciGetNumColors(pobj); /* if m1!=m  old colormap has been  kept*/
  }
  
  if(succeed == 1){ /* failed to allocate or xinit (for Gif driver) was missing */
    sciprint ("Failed to change colormap : Allocation failed or missing xinit detected\n");
    return -1;
  }
  
  if (m1 != old_m){ /* color map size changes, reallocate it */
    if ((cmap = (double *)MALLOC (m*n*sizeof(double))) == (double *) NULL) {
      if (pobj != pfiguremdl) {
	sciSetCurrentFigure ( pobj);
	C2F(dr)("xset","colormap",&old_m,&n,&succeed,PI0,PI0,PI0,
		pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap,PD0,PD0,PD0,0L,0L);
	
	  if(succeed == 1){ /* failed to allocate or xinit (for Gif driver) was missing */
		sciprint ("Failed to change colormap : Allocation failed or missing xinit detected\n");
	    return -1;
	  }
	sciSetCurrentFigure (pcurwin);
      }
      sciprint ("Not enough memory available for colormap, previous one kept\n");
      return -1;
    }  
    FREE(pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap);
    pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap=cmap;
  }
  for (k=0;k<m1*n;k++) pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap[k] = rgbmat[k];
  pFIGURE_FEATURE ((sciPointObj *) pobj)->numcolors = m1;
  
  sciRecursiveUpdateBaW(pobj,old_m, m); /* missing line F.Leray */

  return 0;
}


void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m)
{
  sciSons *psonstmp;
  
  sciSetNumColors (pobj,old_m);
  

  if((sciGetEntityType(pobj) != SCI_TEXT)        &&
     (sciGetEntityType(pobj) != SCI_TITLE)       &&
     (sciGetEntityType(pobj) != SCI_LEGEND)      &&
     (sciGetEntityType(pobj) != SCI_AXES)        &&
     (sciGetEntityType(pobj) != SCI_MENU)        &&
     (sciGetEntityType(pobj) != SCI_MENUCONTEXT) &&
     (sciGetEntityType(pobj) != SCI_STATUSB)    &&
     (sciGetEntityType(pobj) != SCI_LABEL))
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
     (sciGetEntityType(pobj) == SCI_TITLE)       ||
     (sciGetEntityType(pobj) == SCI_LEGEND)      ||
     (sciGetEntityType(pobj) == SCI_AXES)        ||
     (sciGetEntityType(pobj) == SCI_MENU)        ||
     (sciGetEntityType(pobj) == SCI_MENUCONTEXT) ||
     (sciGetEntityType(pobj) == SCI_STATUSB)     ||
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
  if((sciGetEntityType(pobj) == SCI_FIGURE) ||
     (sciGetEntityType(pobj) == SCI_SUBWIN) ||
     (sciGetEntityType(pobj) == SCI_LEGEND) ||
     (sciGetEntityType(pobj) == SCI_ARC) ||
     (sciGetEntityType(pobj) == SCI_POLYLINE) ||
     (sciGetEntityType(pobj) == SCI_RECTANGLE) ||
     (sciGetEntityType(pobj) == SCI_SURFACE) ||
     (sciGetEntityType(pobj) == SCI_AXES) ||
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
	case SCI_LIGHT:
	case SCI_AXES:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetForeground(pobj,value);
	  break;
	case SCI_AGREG:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_PANNER:		/* pas de context graphics */
	case SCI_SBH:		/* pas de context graphics */
	case SCI_SBV:		/* pas de context graphics */
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
	case SCI_LIGHT:
	case SCI_AXES:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetBackground(pobj,value);
	  break;
	case SCI_AGREG:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_PANNER:		/* pas de context graphics */
	case SCI_SBH:		/* pas de context graphics */
	case SCI_SBV:		/* pas de context graphics */
	default:
	  return -1;
	  break;
	}
       break;
    case 2: /* FontForeground*/
      switch (sciGetEntityType (pobj))
	{
	case SCI_AXES:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_TEXT:
	case SCI_TITLE:
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
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetFontBackground(pobj,value);
	  break;
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


/**sciSetNumColors
 * This function sets the number of the color defined in colormap
 * param sciPointObj * pobj: the pointer to the entity
 * param int numcolor: the number of the indexed color
 * return 0 if ok, -1 if not
 */
int
sciSetNumColors (sciPointObj * pobj, int numcolors)
{
  if ( (pobj == pfiguremdl) || (pobj == paxesmdl)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_title)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label) ) /* Addings F.Leray 10.06.04 */
    pFIGURE_FEATURE (pfiguremdl)->numcolors = numcolors;
  else
    sciGetScilabXgc (pobj)->Numcolors = numcolors;
  return 0;
}


/*** Adding F.Leray 31.03.04 for supporting -1 and -2 indexes.*/
int sciSetGoodIndex(sciPointObj * pobj, int colorindex) /* return colorindex or m (Default Black) or m+1 (Default White)*/
{
  int m = sciGetNumColors (pobj);	/* the number of the color*/

  if(colorindex == -1) /* Black */
    return m +1; /* modif. ici*/
  else if(colorindex == -2) /* White */
    return m+1 +1; /* modif. ici*/
  else
    return colorindex;
}




/**sciSetBackground
 * @memo Sets the number of the Background
 */
int
sciSetBackground (sciPointObj * pobj, int colorindex)
{
  /*int zero = 0;*/
  
  int m = sciGetNumColors(pobj); 
  if(colorindex < -2 || colorindex > m+2) return 0;

  colorindex = sciSetGoodIndex(pobj,colorindex);

  if ( (pobj != pfiguremdl) && (pobj != paxesmdl)
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ) /* Addings F.Leray 10.06.04 */
    {
      /* code taken in void C2F(setbackground)(num, v2, v3, v4) from JPC */
      if (sciGetScilabXgc (pobj)->CurColorStatus == 1)
	{
	  /* COLORREF px;                           COLORREF ? "periWin-bgc"*/
	  sciGetScilabXgc (pobj)->NumBackground =
	    Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
	  /* F.Leray 02.04.04: WARNING: What follows is wrong because it forces the background for the all figure, not the object!! IT HAS TO BE REMOVE!!!!*/
	  /*	  C2F(dr) ("xset", "background",&colorindex,&colorindex,&zero,&zero,&zero,PI0,PD0,PD0,PD0,PD0,0L,0L); */ /* DJ.A 07/01/2004 */ 
	  C2F(dr)("xset","alufunction",&(sciGetScilabXgc (pobj)->CurDrawFunction),
		  PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
	}
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TEXT:
      (sciGetFontContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TITLE:
      (sciGetFontContext (pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LIGHT:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENU:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break; 
    case SCI_LABEL: /* F.Leray 28.05.04 */
      (sciGetFontContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
  
    default:
      break;
    }
  return 0;
}


/**sciSetForeground
 * @memo sets the number of the Background
 */
int
sciSetForeground (sciPointObj * pobj, int colorindex)
{

  int m = sciGetNumColors(pobj); 
  if(colorindex < -2 || colorindex > m+2) return 0;
  
  colorindex = sciSetGoodIndex(pobj,colorindex);

  /*pour le moment les couleur pris en compte sont les memes pour tout le monde */
  if ( (pobj != pfiguremdl) && (pobj != paxesmdl) 
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ) /* Addings F.Leray 10.06.04 */
    {
      if (sciGetScilabXgc (pobj)->CurColorStatus == 1)
	{
	  
	  sciGetScilabXgc (pobj)->NumForeground =
	    Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
	  C2F(dr)("xset","alufunction",&(sciGetScilabXgc (pobj)->CurDrawFunction),
		  PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,11L);
	}
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TEXT:
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TITLE:
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LIGHT:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->foregroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENU:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SEGS:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_AGREG:  
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      break;
    }
  return 0;
}




/**sciSetLineWidth
 * @memo Sets the line width
 */
int
sciSetLineWidth (sciPointObj * pobj, int linewidth)
{

  if (linewidth < 0)
    {
      sciprint ("the line width must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  /*sciSetLineWidth (sciGetParentFigure (pobj), linewidth);*/
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_LIGHT:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_MENU:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  break;
        case SCI_SEGS: 
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  break;
	case SCI_AGREG:
	case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	default:
	  /* pas de context graphics */
	  sciprint ("This object has no  line width \n");
	  return -1;
	  break;
	}
    }
  return -1;
}


/**sciSetLineStyle
 * @memo Sets the line style
 */
int
sciSetLineStyle (sciPointObj * pobj, int linestyle)
{

  if (linestyle < 0)
    {
      sciprint ("the line style must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_LIGHT:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_MENU:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  break;
	case SCI_SEGS: 
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  break;
	case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_AGREG:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no  line width \n");
	  return -1;
	  break;
	}
    }
  return 0;
}


/**sciSetIsMark
 * @memo Sets the line style
 */
int
sciSetIsMark (sciPointObj * pobj, BOOL ismark)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->ismark = ismark;
       return 0;
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_LEGEND:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_SEGS:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
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
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      /* pas de context graphics */
      sciprint ("This object have no  ismark \n");
      return -1;
      break;
    }
  return 0;
}


/**sciSetMarkForeground
 * @memo sets the number of the Marks'Foreground
 */
int
sciSetMarkForeground (sciPointObj * pobj, int colorindex)
{
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SEGS: 
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetGraphicContext(pobj))->markforeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LABEL:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciSetMarkBackground
 * @memo sets the number of the Marks'Background
 */
int
sciSetMarkBackground (sciPointObj * pobj, int colorindex)
{
  
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SEGS: 
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetGraphicContext(pobj))->markbackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LABEL:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}






/**sciSetMarkStyle
 * @memo Sets the mark style
 */
int
sciSetMarkStyle (sciPointObj * pobj, int markstyle)
{

  if (markstyle < 0)
    {
      sciprint ("the mark style must be greater or equal than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  /*sciSetMarkStyle (sciGetParentFigure (pobj), markstyle);*/
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_LEGEND:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break; 
	case SCI_SEGS: 
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break; 
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
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no mark \n");
	  return -1;
	  break;
	}
    }
  return 0;
}


/**sciSetMarkSize
 * @memo Sets the mark size
 */
int
sciSetMarkSize (sciPointObj * pobj, int marksize)
{

  if (marksize < 0)
    {
      sciprint ("the mark size must be greater or equal than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  /*sciSetMarksize (sciGetParentFigure (pobj), marksize);*/
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break;
	case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break;
	case SCI_LEGEND:
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break; 
	case SCI_SEGS: 
	  (sciGetGraphicContext(pobj))->marksize = marksize;
	  return 0;
	  break; 
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
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no mark \n");
	  return -1;
	  break;
	}
    }
  return 0;
}

/**sciSetMarkSizeUnit
 * @memo Sets the mark size unit
 * 1 : points, 2 : tabulated
 */
int
sciSetMarkSizeUnit (sciPointObj * pobj, int marksizeunit)
{

  if (marksizeunit < 0)
    {
      sciprint ("the mark size unit must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break;
	case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break;
	case SCI_LEGEND:
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break; 
	case SCI_SEGS: 
	  (sciGetGraphicContext(pobj))->marksizeunit = marksizeunit;
	  return 0;
	  break; 
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
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no mark \n");
	  return -1;
	  break;
	}
    }
  return 0;
}

/**sciSetIsLine
 * @memo Sets the line style existence
 */
int
sciSetIsLine (sciPointObj * pobj, BOOL isline)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_LEGEND:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
    case SCI_SEGS:
      (sciGetGraphicContext(pobj))->isline = isline;
      return 0;
      break;
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
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      /* pas de context graphics */
      sciprint ("This object have no  isline \n");
      return -1;
      break;
    }
  return 0;
}


/**sciSetFillStyle
 * @memo Sets the fillstyle
 */
int
sciSetFillStyle (sciPointObj * pobj, int fillstyle)
{

  if (fillstyle < 0)
    {
      sciprint ("the fill width must be equal or greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  /*sciSetFillStyle (sciGetParentFigure (pobj), fillstyle);*/
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
        case SCI_AXES:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_SEGS: 
        case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_LIGHT:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_AGREG:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	default:
	  sciprint ("This object have no  line style \n");
	  return -1;
	  break;
	}
    }
  return 0;
}



/**sciSetFillColor
 * @memo Sets the fillcolor
 */
int
sciSetFillColor (sciPointObj * pobj, int fillcolor)
{

  if (fillcolor < 0)
    {
      sciprint ("the fill width must be equal or greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  /*sciSetFillStyle (sciGetParentFigure (pobj), fillcolor);*/
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
        case SCI_AXES:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_SEGS: 
        case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_LIGHT:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_AGREG:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	default:
	  sciprint ("This object have no  line width \n");
	  return -1;
	  break;
	}
    }
  return 0;
}


int
sciSetFillFlag (sciPointObj * pobj, int fillflag)
{
	
  switch (sciGetEntityType (pobj))
    { 
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pobj)->fillflag = fillflag;
      break;
    case SCI_ARC:
      pARC_FEATURE (pobj)->fill = fillflag;
      break;
    case SCI_FIGURE:           /* pas de remplissage */
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:  
    case SCI_PANNER:		
    case SCI_SBH:		
    case SCI_SBV:
    case SCI_LABEL: /* F.Leray 28.05.04 */	
    default:
      break;
    }
  return 0;
}



/**sciSetFontDeciWidth
 * @memo Sets the font size
 */
int
sciSetFontDeciWidth (sciPointObj * pobj, int fontdeciwidth)
{

  if (fontdeciwidth < 0)
    {
      sciprint ("The font size must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	  case SCI_FIGURE: /* Adding F.Leray 08.04.04 */
	  /* pas de context graphics */
	  /*  sciprint ("This object have no  Font width \n");
	      return -1;*/
	  (sciGetFontContext(pobj))->fontdeciwidth = fontdeciwidth;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  /* pas de context graphics */
	  /*sciprint ("This object have no  Font width \n");
	    return -1;*/
	  /* Adding F.Leray 08.04.04 */
	  (sciGetFontContext(pobj))->fontdeciwidth = fontdeciwidth;
	  return 0;
	  break;
	case SCI_TEXT:
	  (sciGetFontContext(pobj))->fontdeciwidth = fontdeciwidth;
	  return 0;
	  break;
	case SCI_TITLE:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_LEGEND:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_ARC:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_POLYLINE:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_RECTANGLE:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_SURFACE:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;
	case SCI_LIGHT:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;
        case SCI_AXES:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;
	case SCI_MENU:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  break;
	case SCI_LABEL:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  break;
        case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT:
        case SCI_AGREG:
	default:
	  break;
	}
    }
  return 0;
}




/**sciSetFontOrientation
 * @memo Sets the font Orientation
 */
int
sciSetFontOrientation (sciPointObj * pobj, int textorientation)
{
  if (textorientation < 0)
    {
      sciprint ("The font size must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_TEXT:
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
	  break;
	case SCI_TITLE:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_LEGEND:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_MENU:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  break;
	case SCI_SUBWIN: /* F.Leray 08.04.04*/
	  /* Is text orientation usefull with Axes ?? */
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
	  break;
	case SCI_FIGURE: /* F.Leray 08.04.04*/
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
	  break;
	case SCI_LABEL: /* F.Leray 28.05.04*/
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
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
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;

	}
    }
  return 0;
}





/**sciSetText
 * @memo Sets the Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @param  char *text : the text which has to be put
 * @param  int n
 * @return  0 if OK, -1 if not
 */
int
sciSetText (sciPointObj * pobj, char text[], int n)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      FREE(pTEXT_FEATURE (pobj)->ptextstring);
      if ((pTEXT_FEATURE (pobj)->ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pTEXT_FEATURE (pobj)->ptextstring, text, n);
      pTEXT_FEATURE (pobj)->textlen = n;
      break;
    case SCI_TITLE:
      FREE(pTITLE_FEATURE (pobj)->text.ptextstring);
      if ((pTITLE_FEATURE (pobj)->text.ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pTITLE_FEATURE (pobj)->text.ptextstring, text, n);
      pTITLE_FEATURE (pobj)->text.textlen = n;
      break;
    case SCI_LEGEND:
      FREE(pLEGEND_FEATURE (pobj)->text.ptextstring);
      if ((pLEGEND_FEATURE (pobj)->text.ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pLEGEND_FEATURE (pobj)->text.ptextstring, text, n);
      pLEGEND_FEATURE (pobj)->text.textlen = n;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      FREE(pLABEL_FEATURE (pobj)->text.ptextstring);
      if ((pLABEL_FEATURE (pobj)->text.ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pLABEL_FEATURE (pobj)->text.ptextstring, text, n);
      pLABEL_FEATURE (pobj)->text.textlen = n;
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
      return -1;
      break;
    }
  return 0;
}




/**sciSetFontBackground
 * @memo Sets the background color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontBackground (sciPointObj * pobj, int colorindex)
{

 colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));;
      break;
    case SCI_TITLE:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENU:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
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
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciSetFontForeground
 * @memo Sets the foreground color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, if not
 */
int
sciSetFontForeground (sciPointObj * pobj, int colorindex)
{
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TITLE:
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
    case SCI_MENU:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
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
    default:
      return -1;
      break;
    }
  return 0;
}





/**sciSetFontStyle
 * @memo Sets the font style
 * @param sciPointObj * pobj: the pointer to the entity
 * @param  int iAttributes (a logical mask with SCI_ATTR_BOLD|SCI_ATTR_ITALIC|SCI_ATTR_UNDERLINE|SCI_ATTR_STRIKEOUT)
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontStyle (sciPointObj * pobj, int iAttributes)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SUBWIN: 
    case SCI_FIGURE: 
    case SCI_LABEL: /* F.Leray 10.06.04 */
      (sciGetFontContext(pobj))->fonttype =iAttributes;
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
    default:
      return -1;
      break;
    }
  return 0;
}


/**sciSetFontName
 * @memo Sets the font name 
 * @param sciPointObj * pobj: the pointer to the entity
 * @param  char *fontname
 * @param  int n
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontName (sciPointObj * pobj, char pfontname[], int n)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      /*      if (realloc
	      (	(sciGetFontContext(pobj))->pfontname,
	      n * sizeof (char)) == NULL)
	      return -1;
	      strncpy (	(sciGetFontContext(pobj))->pfontname, pfontname, n);
	      (sciGetFontContext(pobj))->fontnamelen = n;*/
      break;
    case SCI_TITLE:
      /*if (realloc
	(	(sciGetFontContext(pobj))->pfontname,
	n * sizeof (char)) == NULL)
	return -1;
	strncpy (	(sciGetFontContext(pobj))->pfontname, pfontname,
	n);
	(sciGetFontContext(pobj))->fontnamelen = n;*/
      break;
    case SCI_LEGEND:
      /*if (realloc
	(	(sciGetFontContext(pobj))->pfontname,
	n * sizeof (char)) == NULL)
	return -1;
	strncpy (	(sciGetFontContext(pobj))->pfontname, pfontname,
	n);
	(sciGetFontContext(pobj))->fontnamelen = n;*/
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
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
    default:
      return -1;
      break;
    }
  return 0;
}




/**sciSetTextPosX
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
int
sciSetTextPosX (sciPointObj * pobj, double x)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      pTEXT_FEATURE (pobj)->x = x;
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
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}


/**sciSetTextPosY
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
int
sciSetTextPosY (sciPointObj * pobj, double y)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      pTEXT_FEATURE (pobj)->y = y;
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
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}


/**sciSetTitlePos
 * @memo Sets the Title Position in the graphique window. This function is actualy private
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetTitlePos (sciPointObj * pobj, int x, int y)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TITLE:
      pTITLE_FEATURE (pobj)->pos.x = x;
      pTITLE_FEATURE (pobj)->pos.y = y;
      break;
    case SCI_FIGURE:
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
    default:
      sciprint ("Your are not using a title object !\n");
      return -1;
      break;
    }
  return 0;
}



/**sciSetTitlePlace
 * @memo Sets the Title Place  with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetTitlePlace (sciPointObj * pobj, sciTitlePlace place)
{
  int x = 0;
  int y = 0;

  if (sciGetEntityType (pobj) == SCI_TITLE)
    {
      pTITLE_FEATURE (pobj)->titleplace = place;
      /* calcul de l emplacement relatif du titre 
       * en fonction de la taille de la police 
       * de la fenetre...
       */
      sciSetTitlePos (pobj, x, y);
      return 0;
    }
  else
    sciprint ("Your are not using a title object !\n");
  return -1;
}





/**sciSetLegendPlace
 * @memo Sets the Title Place  with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place)
{
  int x = 0;
  int y = 0;

  if (sciGetEntityType (pobj) == SCI_TITLE)
    {
      pLEGEND_FEATURE (pobj)->place = place;
      /* calcul de l emplacement relatif du titre 
       * en fonction de la taille de la police 
       * de la fenetre...
       */
      sciSetLegendPos (pobj, x, y);
      return 0;
    }
  else
    sciprint ("Your are not using a legend object !\n");
  return -1;
}


/**sciSetLegendPos
 * @memo Sets the Legend Position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetLegendPos (sciPointObj * pobj, int x, int y)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_LEGEND:
      pLEGEND_FEATURE (pobj)->pos.x = x;
      pLEGEND_FEATURE (pobj)->pos.y = y;
      break;
    case SCI_TITLE:
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
    default:
      sciprint ("Your are not using a legend object !\n");
      return -1;
      break;
    }
  return 0;
}


/**sciSetIsClipping*/
void
sciSetIsClipping (sciPointObj * pobj, int value)
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
    case SCI_LIGHT: 
      pLIGHT_FEATURE (pobj)->isclip = value;
      if(value>0) pLIGHT_FEATURE (pobj)->clip_region_set=1;
      break;   
    case SCI_AXES: 
      pAXES_FEATURE (pobj)->isclip = value;
      if(value>0) pAXES_FEATURE (pobj)->clip_region_set=1;
      break;
    case SCI_SURFACE:  
    case SCI_LEGEND: 
    case SCI_TITLE:
    case SCI_FEC: 
    case SCI_GRAYPLOT:    
    case SCI_AGREG: 
    case SCI_FIGURE:
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      break;
    }
}




/**sciSetClipping
 * @memo Sets the clipping area
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
int
sciSetClipping (sciPointObj * pobj, double *pclip)
{
  int i;

  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      for(i=0;i<4;i++) pSUBWIN_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;
    case SCI_ARC:
      for(i=0;i<4;i++) pARC_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;
    case SCI_POLYLINE:
      for(i=0;i<4;i++) pPOLYLINE_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;
    case SCI_RECTANGLE:
      for(i=0;i<4;i++) pRECTANGLE_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;
    case SCI_SEGS: 
      for(i=0;i<4;i++) pSEGS_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;      
    case SCI_TEXT: 
      for(i=0;i<4;i++) pTEXT_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;   
    case SCI_LIGHT: 
      for(i=0;i<4;i++) pLIGHT_FEATURE (pobj)->clip_region[i] = pclip[i]; /* not used for now 04.04.2005 */
      break;   
    case SCI_AXES: 
      for(i=0;i<4;i++) pAXES_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;
      /* not used for now 04.04.2005 */
/*     case SCI_SURFACE: */
/*       for(i=0;i<4;i++) pSURFACE_FEATURE (pobj)->clip_region[i] = pclip[i]; */
/*       break; */
/*     case SCI_LEGEND:  */
/*       for(i=0;i<4;i++) pLEGEND_FEATURE (pobj)->clip_region[i] = pclip[i]; */
/*       break; */
/*     case SCI_TITLE:   */
/*       for(i=0;i<4;i++) pSURFACE_FEATURE (pobj)->clip_region[i] = pclip[i]; */
/*       break; */
/*     case SCI_AGREG:  */
/*       for(i=0;i<4;i++) pAGREG_FEATURE (pobj)->clip_region[i] = pclip[i]; /\* not used for now 04.04.2005 *\/ */
/*       break; */
/*     case SCI_FEC:  */
/*       for(i=0;i<4;i++) pFEC_FEATURE (pobj)->clip_region[i] = pclip[i]; /\* not used for now 04.04.2005 *\/ */
/*       break; */
/*     case SCI_GRAYPLOT: */
/*       for(i=0;i<4;i++) pGRAYPLOT_FEATURE (pobj)->clip_region[i] = pclip[i]; /\* not used for now 04.04.2005 *\/ */
/*       break; */
    case SCI_LABEL:
      for(i=0;i<4;i++) pLABEL_FEATURE (pobj)->clip_region[i] = pclip[i];
      break;
    case SCI_SURFACE:
    case SCI_LEGEND: 
    case SCI_TITLE:    
    case SCI_AGREG:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_FIGURE: 
    default:
      return -1;
      break;
    } 
  return 0;

}


/**sciSetHighLight
 * @memo Tunrs ON or OFF the highlighting of the objects when there are selected
 */

void
sciSetHighLight (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* (sciGetGraphicMode (pobj))->highlight = value; unused */
      break;
    case SCI_SUBWIN:
      /* la valeur est herite du parent...pour le moment */

      /* (sciGetGraphicMode (pobj))->highlight = value;
	 sciSetHighLight (sciGetParentFigure (pobj), value);unused */
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
      sciprint ("We cannot highlight this object\n");
      break;
    default:
      sciprint ("We cannot highlight this object\n");
      break;
    }
}


/**sciSetAddPlot
 * @memo Tunrs ON or OFF the possibility to draw multi plots in one graphic. 
 * If FALSE, plot is cleared before new drawing
 */
void
sciSetAddPlot (sciPointObj * pobj, BOOL value)
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
    default:
      sciprint ("This object cannot be addploted\n");
      break;
    }
}



/**sciSetAutoScale
 * @memo Sets the mode if the plot is in zoom fit mode, or fixed by the user
 */
void
sciSetAutoScale (sciPointObj * pobj, BOOL value)
{
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (pobj == pfiguremdl)
	(sciGetGraphicMode (pobj))->autoscaling = value;
      else
	sciSetAutoScale(sciGetSelectedSubWin (pobj),value);
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->autoscaling = value;
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
      sciprint ("This object cannot be autoscaled 1 sciSetAutoScale\n");
      break;
      break;
    default:
      sciprint ("This object cannot be autoscaled 2 sciSetAutoScale\n");
      break;
    }
}



/**sciSetZooming
 * @memo Sets the zooming state of the object. Is it or not zommed right now
 */
void
sciSetZooming (sciPointObj * pobj, BOOL value)
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
      sciprint ("\r\nThis object cannot be zoomed\r\n");
      break;
    default:
      sciprint ("\r\nThis object cannot be zoomed\r\n");
      break;
    }
}


/**sciSetDefaultValue
 * @memo Sets the default values of figure properties
 * (graphics context, graphics mode, and graphics font)
 * @author Djalel ABDEMOUCHE
 */
void
sciSetDefaultValues ()
{
  if ((sciInitGraphicContext (sciGetCurrentFigure()) == -1) ||
      (sciInitGraphicMode (sciGetCurrentFigure()) == -1) ||
      (sciInitFontContext (sciGetCurrentFigure()) == -1)) /* Adding F.Leray 13.04.04 to have the completed init.*/
    sciprint("\r\n default values cant not be loaded !");
  else
    {
      C2F(dr)("xset","default",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,7L); 
      sciGetScilabXgc (sciGetCurrentFigure())->graphicsversion = 0; /* To Re enable the NG -> graphicsversion = 0*/
    }
}

/**sciSetGraphicsStyle
 * @memo Sets the graphics style
 */
void
sciSetGraphicsStyle (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->oldstyle = value;
      break;
    case SCI_SUBWIN: 
      /* the value is inhirated by the parent */
      sciSetGraphicsStyle (sciGetParentFigure (pobj), value);
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
      sciprint ("\r\nNothing to do\n");
      break;
    default:
      sciprint ("\r\nNothing to do\n");
      break;
    }
}

/**sciSetXorMode
 * @memo Sets the xor mode
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int value: the value of the xor mode
 * @return 
 * @author Djalel ABDEMOUCHE
 * @see sciGetXorMode
 */
void
sciSetXorMode (sciPointObj * pobj, int value)
{ 
  if ( (pobj != pfiguremdl) && (pobj != paxesmdl))
    C2F(dr)("xset","alufunction",&(value),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,11L);
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
      sciprint ("\r\nNothing to do\n");
      break;
    default:
      sciprint ("\r\nNothing to do\n");
      break;
    }
}


/**sciSetVisibility*/
void
sciSetVisibility (sciPointObj * pobj, BOOL value)
{  
  sciSons *psonstmp;

  psonstmp = sciGetSons ((sciPointObj *) pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE (pobj)->visible = value;
/*       while ((psonstmp != (sciSons *) NULL) && (psonstmp->pointobj != (sciPointObj *)NULL)) */
/* 	{ */
/* 	  sciSetVisibility ((sciPointObj *)psonstmp->pointobj,value);  */
/* 	  psonstmp = psonstmp->pnext; */
/* 	} */
      break;
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->visible = value;
/*       while ((psonstmp != (sciSons *) NULL) && (psonstmp->pointobj != (sciPointObj *)NULL)) */
/* 	{ */
/* 	  sciSetVisibility ((sciPointObj *)psonstmp->pointobj,value);  */
/* 	  psonstmp = psonstmp->pnext; */
/* 	} */
      break;
    case SCI_TITLE:
      pTITLE_FEATURE (pobj)->visible = value;
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
    case SCI_LIGHT: 
      pLIGHT_FEATURE (pobj)->visible = value;
      break;
    case SCI_AXES: 
      pAXES_FEATURE (pobj)->visible = value;
      break;    
    case SCI_AGREG: 
      pAGREG_FEATURE (pobj)->visible = value; 
/*       while ((psonstmp != (sciSons *) NULL) && (psonstmp->pointobj != (sciPointObj *)NULL)) */
/* 	{ */
/* 	  sciSetVisibility ((sciPointObj *)psonstmp->pointobj,value);  */
/* 	  psonstmp = psonstmp->pnext; */
/* 	}   */
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      pLABEL_FEATURE (pobj)->visible = value;
      break;
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      break;
    }
}





/**sciSetResize
 * @memo Tunrs ON or OFF the autoresizing mode (when the window is resized by user)
 * @param sciPointObj * pobj: the pointer to the entity
 * @param BOOL value: the value of the switch
 */
void
sciSetResize (sciPointObj * pobj, BOOL value)
{
  integer num1 = (value ? 1 : 0);
  if ( (pobj != pfiguremdl) && (pobj != paxesmdl))
    { 
      /* this code will coms from
       *  C2F(setwresize)((i = value, &i), PI0,PI0,PI0);
       * je changerais ce morceau de code quand tout csera OK
       */
      if (sciGetScilabXgc (pobj)->CurResizeStatus != num1)
	{
	  C2F(dr)("xset","wresize",&(num1),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L);  
	  
	  /*  sciGetScilabXgc (pobj)->CurResizeStatus = num1;  
	      C2F(dr)("xget","wpdim",&xtmp,x,&xtmp,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L); 
	      C2F(dr)("xset","wpdim",&(x[0]),&(x[1]),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L);*/
#ifdef WIN32
	  /* Win function sciGetScilabXgc (pobj)->horzsi.nPos !!? BCG.horzsi.nPos*/
	  /* SetViewportOrgEx (GetDC (sciGetScilabXgc (pobj)->CWindow),  
	     -sciGetScilabXgc (pobj)->horzsi.nPos,
	     -sciGetScilabXgc (pobj)->vertsi.nPos, NULL);*/
	  /*if (sciGetPixmapStatus () == 0)                                          
	    InvalidateRect (sciGetScilabXgc (pobj)->CWindow, NULL, TRUE); 
	    return ScilabXgc->CurPixmapStatus;
	    UpdateWindow (sciGetScilabXgc (pobj)->CWindow); */ /* dependent function MacWinOther.c"*/
	  
#endif
	}
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->wresize = value;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->wresize = value;
      /* the value is inhirated by the parent */
      /*sciSetResize (sciGetParentFigure (pobj), value);*/
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
    default:
      sciprint ("This object cannot be resized\n");
      return;
      break;
    }
}



/**sciSetName 
 * @memo Sets the name of the Figure
 * @param sciPointObj * pobj: the pointer to the entity
 * @param char pvalue: a pointer to the string contening name
 * @param int length: the length of the string
 * @return 
 */
void
sciSetName (sciPointObj * pobj, char *pvalue, int length)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if(strlen(pvalue) > 79){
	sciprint("Warning: Figure name is limlited to 80 characters\n");
	break;}
      else
	strcpy (pFIGURE_FEATURE (pobj)->name, pvalue);
      pFIGURE_FEATURE (pobj)->namelen = Min (79, length); 
      if (pobj != pfiguremdl) {
	char str[80];
	sprintf(str,pvalue,sciGetNum(pobj));
	C2F(dr)("xname",str,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,(long) length);
      }
      break;
    case SCI_SUBWIN:
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can be named\n");
      break;
    }
}



/**sciSetNum
 * @memo Sets the number of the Figure.
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int value: the value of the number of the windows
 */
void
sciSetNum (sciPointObj * pobj, int *value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE (pobj)->number = *value;
      break;
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->number = *value;
      sciSetNum (sciGetParentFigure (pobj), value);
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure be numerated\n");
      break;
    }
}



/**sciSetDim
 * @memo Sets the dimension of the Figure or Subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int *pwidth: the width of the window dimension
 * @param int *pheight: the height of the window dimension
 * @return 
 */
void
sciSetDim (sciPointObj * pobj, int *pwidth, int *pheight)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* selectionner le xgc correspondant puis */
      pFIGURE_FEATURE (pobj)->figuredimwidth = *pwidth;
      pFIGURE_FEATURE (pobj)->figuredimheight = *pheight;

      /* GPopupResize (sciGetScilabXgc (pobj), pwidth, pheight); commented out SS 03/01/03 */

      break;
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->windimwidth = *pwidth;
      pSUBWIN_FEATURE (pobj)->windimheight = *pheight;
      if (pobj != paxesmdl)
	C2F(dr)("xset","wdim",pwidth, pheight,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,4L);
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure or Subwin can be sized\n");
      break;
    }
}



/**sciSetFigurePos
 * @memo Sets the position of the FIGURE (the window) in root.
 */
int
sciSetFigurePos (sciPointObj * pobj, int pposx, int pposy)
{
  integer y=0,cur,num,na;

  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (pobj != pfiguremdl) {
	num=pFIGURE_FEATURE(pobj)->number;
	C2F(dr)("xget","window",&y,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
	C2F(dr)("xset","window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F(dr)("xset","wpos",&pposx,&pposy,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,4L);
	C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      }
      pFIGURE_FEATURE (pobj)->inrootposx = pposx;
      pFIGURE_FEATURE (pobj)->inrootposy = pposy;
      return 0;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can breturn position\n");
      return -1;
      break;
    }
  return -1;
}


/**sciSetFigureIconified
 * @memo Minimizes or Restores the window if TRUE or FALSE (useful to get the Window on front)
 * @param sciPointObj * pobj: the pointer to the entity
 * @param BOOL value: TRUE the window will be iconify, FALSE the window will be raise
 * @return 
 */
void
sciSetFigureIconify (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE (pobj)->isiconified = value;
      /* hWndParent de Type HWND (BCG) "WIN" */
      /*     if (value)
	     ShowWindow ((sciGetScilabXgc (pobj))->hWndParent, SW_MINIMIZE);
	     else
	     {
	     ShowWindow ((sciGetScilabXgc (pobj))->hWndParent, SW_SHOWNORMAL); 
	     BringWindowToTop ((sciGetScilabXgc (pobj))->hWndParent);
	     } */
      break;
    default:
      sciSetFigureIconify(sciGetCurrentFigure(), value);
      break;
    }
}


/**sciSetSubwindowPos
 * Sets subwindow position 
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int pposx: the pointer to the x position
 * @param int pposy: the pointer to the y position
 * @return  0 if OK , else -1
 * @version 0.1.
 **/
int
sciSetSubWindowPos (sciPointObj * pobj, int *pposx, int *pposy)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      /* selectionner le xgc correspondant puis */
      pSUBWIN_FEATURE (pobj)->infigureposx = *pposx;
      pSUBWIN_FEATURE (pobj)->infigureposy = *pposy;
      return 0;
      /* C2F(setwindowpos)( width, height, PI0, PI0); */
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only subwin can be return position\n");
      return -1;
      break;
    }
  return -1;
}



/**sciSetSelectedSubWin
 * @memo Determines wich SubWin is selected or not. WARNING TO BE DEFINED.
 * @param sciPointObj * psubwinobj: the pointer to the entity sub window
 * @return 1 if OK or -1 if NOT OK
 */
int
sciSetSelectedSubWin (sciPointObj * psubwinobj)
{
  sciPointObj *pselectedsubwin;

  /* on verifie que l'entite passee en argument est bien une sous fenetre */
  if (sciGetEntityType (psubwinobj) != SCI_SUBWIN)
    {
      sciprint("This Handle is not a SubWindow\n");
      return -1;
    }
  /* on verifie que la sous fenetre donnee n'est pas deja selectionnee */
  if (sciGetIsSelected (psubwinobj))
    return 1;

  /* sinon on recherche la sous fenetre deja selectionnee */
  /* dans l'ensemble des sous fenetres du parent SCI_FIGURE */
  if ((pselectedsubwin = (sciPointObj *)sciGetSelectedSubWin(sciGetParent(psubwinobj))) != (sciPointObj *)NULL)
    /* puis on la deselectionne */
    pSUBWIN_FEATURE (pselectedsubwin)->isselected = FALSE;

  /* puis on selectionne la sous fenetre passee en argument */
  pSUBWIN_FEATURE (psubwinobj)->isselected = TRUE;


  set_scale ("tttftt", pSUBWIN_FEATURE (psubwinobj)->WRect,
	     pSUBWIN_FEATURE (psubwinobj)->FRect, NULL,
	     pSUBWIN_FEATURE(psubwinobj)->logflags, pSUBWIN_FEATURE (psubwinobj)->ARect);
  

  return 1;
}



/**sciSetOriginalSubWin
 * @memo DO NOT USE !!!
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




/**sciSetCurrentFigure
 * @memo Sets the pointer to the current selected figure. 
 */

void 
sciSetCurrentFigure (sciPointObj * mafigure)
{
  sciGetCurrentScilabXgc ()->mafigure = mafigure ;
  cf_type=1;/* current figure is a graphic one */
}                                                               


/**sciSetReplay
 * @memo sets the variable "we are in replay mode"
 */
int
sciSetReplay (BOOL value)
{
  modereplay = value;
  return 0;

}



/**sciSetPoint
 * @memo sets points of the entity, and a pointer to the number of points
 */
/** MAJ pour le 3D DJ.Abdemouche 2003**/
int
sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol)
{
  int i,n1,k,k1,k2;
  double *pvx = NULL,
    *pvy = NULL,
    *pvz = NULL,
    *pvfx = NULL,
    *pvfy = NULL,
    *pvfz = NULL;
  int *pstyle = NULL;
  POINT2D *pvector;

  switch (sciGetEntityType (pthis))
    {
    case SCI_POLYLINE:
      n1=pPOLYLINE_FEATURE (pthis)->n1;
      if ((*numcol != 3)&&(*numcol != 2))
	{
	  sciprint("The number of columns must be 2 (3 if three-dimensional axes) \n");
	  return -1;
	}
      if (*numrow != n1) /* SS 30/1/02 */
	{
	  n1=*numrow;
	  if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	  if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    return -1;
	  } 
	  if ((pvector = MALLOC (n1 * sizeof (POINT2D))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    FREE(pvy); pvy = (double *) NULL;
	    return -1;
	  }
	  if (*numcol == 3)
	    if ((pvz = MALLOC (n1 * sizeof (double))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
	      FREE(pvector); pvector = (POINT2D *) NULL;
	      return -1;
	    }
	  
/* 	  for (i = 0; i < *numrow; i++) /\* Init. to 0. if no z is specified *\/ */
/* 	    pvz[i] = 0.; */
	  

	  FREE(pPOLYLINE_FEATURE (pthis)->pvx); pPOLYLINE_FEATURE (pthis)->pvx = NULL;
	  FREE(pPOLYLINE_FEATURE (pthis)->pvy); pPOLYLINE_FEATURE (pthis)->pvy = NULL;
	  FREE(pPOLYLINE_FEATURE (pthis)->pvector); pPOLYLINE_FEATURE (pthis)->pvector = NULL;
	  FREE(pPOLYLINE_FEATURE (pthis)->pvz); pPOLYLINE_FEATURE (pthis)->pvz = NULL;
	  
	  for (i=0;i < *numrow;i++)
	    {
	      pvx[i] = tab[i];
	      pvy[i] = tab[i+ (*numrow)];
	      pvector[i].x =  tab[i];
	      pvector[i].y =  tab[i+ (*numrow)];
	      if (*numcol == 3)
		pvz[i] = tab[i+ 2*(*numrow)];
	    }

	  pPOLYLINE_FEATURE (pthis)->pvx=pvx;
	  pPOLYLINE_FEATURE (pthis)->pvy=pvy; 
	  pPOLYLINE_FEATURE (pthis)->pvector=pvector;
	  pPOLYLINE_FEATURE (pthis)->n1=n1;


	  /* if (*numcol == 3) */
	  pPOLYLINE_FEATURE (pthis)->pvz=pvz;
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
	  
	  
	  
	  /*	  if (*numcol == 3)
	    	  pPOLYLINE_FEATURE (pthis)->pvz = pvz; */ /* ..and here we put pvz...*/
	}
      

   
      return 0;
      break;
    case SCI_RECTANGLE:
      if ((*numrow * *numcol != 5)&&(*numrow * *numcol != 4))
	{
	  sciprint("The number of element must be 4 (5 if z coordinate )\n");
	  return -1;
	}

      pRECTANGLE_FEATURE (pthis)->x          = tab[0];
      pRECTANGLE_FEATURE (pthis)->y          = tab[1];
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  pRECTANGLE_FEATURE (pthis)->z          = tab[2];
	  pRECTANGLE_FEATURE (pthis)->width      = tab[3];
	  pRECTANGLE_FEATURE (pthis)->height     = tab[4];
	}
      else
	{
	  pRECTANGLE_FEATURE (pthis)->width      = tab[2];
	  pRECTANGLE_FEATURE (pthis)->height     = tab[3];
	}
      return 0;
      break;
    case SCI_ARC:
      if ((*numrow * *numcol != 7)&&(*numrow * *numcol != 6))
	{
	  sciprint("The number of elements must be 6 (7 if z coordinate )\n");
	  return -1;
	}
      
      pARC_FEATURE (pthis)->x          = tab[0];
      pARC_FEATURE (pthis)->y          = tab[1];  
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  pARC_FEATURE (pthis)->z          = tab[2];
	  pARC_FEATURE (pthis)->width      = tab[3];
	  pARC_FEATURE (pthis)->height     = tab[4];
	  pARC_FEATURE (pthis)->alphabegin = tab[5];
	  pARC_FEATURE (pthis)->alphaend   = tab[6];
	}
      else
	{
	  pARC_FEATURE (pthis)->width      = tab[2];
	  pARC_FEATURE (pthis)->height     = tab[3];
	  pARC_FEATURE (pthis)->alphabegin = tab[4];
	  pARC_FEATURE (pthis)->alphaend   = tab[5]; 
	}
      return 0;
      break;
    case SCI_TEXT:
      if ((*numrow * *numcol != 2)&&(*numrow * *numcol != 3))
	{
	  sciprint("The number of elements must be 2 (3 if z coordinate)\n");
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
	  sciprint("The number of columns must be 2 (3 if three-dimensional axes) \n");
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
	/* F.Leray 27.07.04 */
/* 	if ((*numcol != 3 +3*(*numrow * *numrow))&&(*numcol != 2 +2*(*numrow * *numrow))) */
/* 	  { */
/* 	    sciprint("The number of columns must be %d (%d if three-dimensional axes)\n", */
/* 		     2+2*(*numrow * *numrow),3+3*(*numrow * *numrow)); */
/* 	    return -1; */
/* 	  } */
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
      sciprint ("Un handled data field\n");
      return -1;
      break;
    case SCI_GRAYPLOT:
      if (pGRAYPLOT_FEATURE (pthis)->type == 0) { /* gray plot */
	double *pvecx,*pvecy,*pvecz;
	int nx,ny;
	nx=*numrow-1;
	ny=*numcol-1;
	if (pGRAYPLOT_FEATURE (pthis)->ny!=ny || pGRAYPLOT_FEATURE (pthis)->nx!=nx) {
	  if ((pvecx = calloc(nx,sizeof(double))) == NULL) {
	    sciprint ("Not enough memory\n");
	    return -1;}
	  if ((pvecy = calloc(ny,sizeof(double))) == NULL) {
	    FREE(pvecx);
	    sciprint ("Not enough memory\n");
	    return -1;}
	  if ((pvecz = calloc(nx*ny,sizeof(double))) == NULL) {
	    FREE(pvecx);FREE(pvecy);
	    sciprint ("Not enough memory\n");
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
	  if ((pvecz = calloc(nx*ny,sizeof(double))) == NULL) {
	    sciprint ("Not enough memory\n");
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
	  sciprint ("The data must have 3 columns\n");
	  return -1;}
      
	Nnode = *numrow;
	if (pFEC_FEATURE (pthis)->Nnode!=Nnode) {
	  if ((pvecx = calloc(Nnode,sizeof(double))) == NULL) {
	    sciprint ("Not enough memory\n");
	    return -1;}
	  if ((pvecy = calloc(Nnode,sizeof(double))) == NULL) {
	    sciprint ("Not enough memory\n");
	    FREE(pvecx);
	    return -1;}
	  if ((pfun = calloc(Nnode,sizeof(double))) == NULL) {
	    sciprint ("Not enough memory\n");
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
    case SCI_SBV:
    case SCI_SBH:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_LIGHT:    
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object has no possibility to set points !\n");
      return -1;
      break;
    }
  return -1;
}


int
sciSetdrawmode (BOOL mode)
{ 
  static sciPointObj *pobj;

  pobj= (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ());
  pSUBWIN_FEATURE(pobj)->visible = mode ;	
  sciDrawObj(sciGetCurrentFigure ());
  return 0;
}



int sciSwitchWindow(winnum)
     int *winnum; 
{ 
  struct BCG *CurXGC; 
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;  
  integer v=0;
  double dv=0.0; 
  /* find if exist figure winnum */
  /* une autre methode c est de tester CurXGC->mafigure = NULL */
  if ( (sciPointObj *) sciIsExistingFigure(winnum) == (sciPointObj *) NULL) 
    {
      /** Figure winnum don't exist **/
      /** Create Figure **/ 
      C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&CurXGC,&dv,&dv,&dv,5L,10L);/* ????? SS*/
      /*    if ((mafigure = ConstructFigure (CurXGC)) != NULL)*/ /*F.Leray 24.03.04 */
       if ((mafigure = ConstructFigure (CurXGC)) != NULL)
	{
	  sciSetCurrentObj (mafigure); /* F.Leray 25.03.04*/
	  CurXGC->mafigure = mafigure;
          CurXGC->graphicsversion = 0;
	  if ((masousfen = ConstructSubWin (mafigure, CurXGC->CurWindow)) != NULL) {
	    sciSetCurrentObj (masousfen);
	    sciSetOriginalSubWin (mafigure, masousfen);
	    cf_type=1;/* current figure is a graphic one */
	  }
	}
       else
	 return 1; /* failed to switch */
       
    }
  else
    cf_type=1;/* current figure is a graphic one */
  return 0;
}


void set_version_flag(int flag) 
{ 
  double *XGC,dv=0;
  struct BCG *CurrentScilabXgc; 
  int v=0;
  
  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;
  if (CurrentScilabXgc !=(struct BCG *)NULL) 
    CurrentScilabXgc->graphicsversion = flag; 
}

