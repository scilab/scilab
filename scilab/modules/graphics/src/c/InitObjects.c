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

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "InitObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "bcg.h"
#include "PloEch.h"
#include "Axes.h"
#include "DestroyObjects.h"
#include "CloneObjects.h"
#include "BuildObjects.h"
#include "Xcall1.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"

#include "MALLOC.h" /* MALLOC */

static char error_message[70]; /* DJ.A 08/01/04 */
extern unsigned short defcolors[];

static sciPointObj * pfiguremdl = (sciPointObj *) NULL;
static sciPointObj * paxesmdl = (sciPointObj *) NULL;

sciPointObj * getFigureModel( void )
{
  return pfiguremdl ;
}

sciPointObj * getAxesModel( void )
{
  return paxesmdl ;
}


/* DJ.A 08/01/04 */
int C2F(graphicsmodels) (void)
{
  sciHandleTab * newhd1 ;
  sciHandleTab * newhd2 ;
  sciSubWindow * ppaxesmdl = NULL ;

  if ((pfiguremdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      strcpy(error_message,"Default figure cannot be create");
      return 0;  	  
    }
  sciSetEntityType (pfiguremdl, SCI_FIGURE);
  if ((pfiguremdl->pfeatures = MALLOC ((sizeof (sciFigure)))) == NULL)
    {
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  
  if ((newhd1 = MALLOC ((sizeof (sciHandleTab)))) == NULL)
    {
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  newhd1->pnext = (sciHandleTab *) NULL;
  newhd1->pprev = (sciHandleTab *) NULL;
  newhd1->index = (long)pfiguremdl;
  (sciGetRelationship (pfiguremdl))->phandle = newhd1;
    
  if (!(sciAddThisToItsParent(pfiguremdl, (sciPointObj *)NULL))) 
    {
      sciDelHandle (pfiguremdl);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  
  sciInitSelectedSons( pfiguremdl ) ;

  pFIGURE_FEATURE (pfiguremdl)->relationship.psons = (sciSons *) NULL;
  pFIGURE_FEATURE (pfiguremdl)->relationship.plastsons = (sciSons *) NULL;

  /* set default properties */
  if ( InitFigureModel() < 0 )
  {
    sciDelHandle (pfiguremdl);
    FREE(pfiguremdl->pfeatures);
    FREE(pfiguremdl);
    strcpy(error_message,"Default figure cannot be create");
    return 0;
  }

  /* F.Leray Adding some FontContext Info for InitFontContext function */
  /*  pFIGURE_FEATURE (pfiguremdl)->fontcontext.backgroundcolor = */

  
  if ((paxesmdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  sciSetEntityType (paxesmdl, SCI_SUBWIN);
  if ((paxesmdl->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
    {
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  if ((newhd2 = MALLOC ((sizeof (sciHandleTab)))) == NULL)
    {
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  newhd2->pnext = (sciHandleTab *) NULL;
  newhd2->pprev = (sciHandleTab *) NULL;
  newhd2->index = (long)paxesmdl;
  (sciGetRelationship (paxesmdl))->phandle = newhd2;
  
  if (!(sciAddThisToItsParent (paxesmdl, pfiguremdl)))
    {
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  
  ppaxesmdl =  pSUBWIN_FEATURE (paxesmdl);

  sciInitSelectedSons( paxesmdl ) ;
  ppaxesmdl->relationship.psons = (sciSons *) NULL;
  ppaxesmdl->relationship.plastsons = (sciSons *) NULL;
  
  if ( InitAxesModel() < 0 )
  {
    sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
    sciDelHandle (paxesmdl);
    FREE(paxesmdl->pfeatures);
    FREE(paxesmdl);          
    strcpy(error_message,"Default axes cannot be create");
    return 0;
  }

  /* there are properties not initialized bu InitAxesModel */
  /* Is it a missing ? */
     
  ppaxesmdl->FirstPlot = TRUE;
  ppaxesmdl->with_leg = 0;
     
  ppaxesmdl->ARect[0]   = 0.125;
  ppaxesmdl->ARect[1]   = 0.125;
  ppaxesmdl->ARect[2]   = 0.125;
  ppaxesmdl->ARect[3]   = 0.125;
  
  ppaxesmdl->clip_region[0] = 0.;
  ppaxesmdl->clip_region[1] = 0.;
  ppaxesmdl->clip_region[2] = 0.;
  ppaxesmdl->clip_region[3] = 0.;

  /* the model has not been changed !!! */
  ppaxesmdl->clip_region_set = 0 ;
  
  
 
  
  return 1;
}


/**sciInitGraphicContext
 * Inits the graphic context of this object with the default value. the new graphic object inherits parent's features by sciGetParent()
 */
int
sciInitGraphicContext (sciPointObj * pobj)
{
  /* 
   * initialisation du contexte graphique par defaut 
   * que l'on peut recuperer sur les structure de base de scilab 
   * la colormap des fils est heritee du parent
   */

  switch (sciGetEntityType (pobj))
  {
  case SCI_FIGURE:
    if ( pobj == pfiguremdl )
    {
      (sciGetGraphicContext(pobj))->backgroundcolor = -3; /*33;*/  /* F.Leray 29.03.04: Wrong index here: 32+1 (old method) must be changed to -1 new method*/
      (sciGetGraphicContext(pobj))->foregroundcolor = -2; /*32;*/  /* F.Leray 29.03.04: Wrong index here: 32+2 (old method) must be changed to -2 new method*/
      (sciGetGraphicContext(pobj))->fillstyle = HS_HORIZONTAL;
      (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
      (sciGetGraphicContext(pobj))->linewidth = 1;
      (sciGetGraphicContext(pobj))->linestyle = PS_SOLID;
      (sciGetGraphicContext(pobj))->ismark    = FALSE;
      (sciGetGraphicContext(pobj))->isline    = TRUE;
      (sciGetGraphicContext(pobj))->markstyle = 0;
      (sciGetGraphicContext(pobj))->marksize = 0 ; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->markbackground = -3; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->markforeground = -2; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->marksizeunit = 2; /* New F.Leray 22.02.05 */ /* 1 : points, 2 : tabulated */
    }
    else
    {
      cloneGraphicContext( pfiguremdl, pobj ) ;
    }
    return 0;
    break;
  case SCI_SUBWIN:
    if ( pobj == paxesmdl )
    {
      cloneGraphicContext( sciGetParent (pobj), pobj ) ;
      sciGetGraphicContext(pobj)->backgroundcolor = -3 ;
      sciGetGraphicContext(pobj)->foregroundcolor = -2 ;
      sciGetGraphicContext(pobj)->markbackground  = -3 ;
      sciGetGraphicContext(pobj)->markforeground  = -2 ;
    }
    else
    {
      cloneGraphicContext( paxesmdl, pobj ) ;
    }
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
  case SCI_AXES:
  case SCI_MENU:
  case SCI_MENUCONTEXT:
  case SCI_STATUSB: 
  case SCI_LEGEND: /* Adding a graphic context to legend object F.Leray 21.01.05 */
  case SCI_TEXT:
    cloneGraphicContext( sciGetParent (pobj), pobj ) ;
    return 0;
    break;
  case SCI_LABEL: /* F.Leray 28.05.04, modif JB.Silvy 03/2006 */
    if ( sciGetParent(pobj) == paxesmdl )
    {
      /* this is a label model */
      cloneGraphicContext( sciGetParent( pobj ), pobj ) ;
      return 0;
    }
    else
    {
      sciPointObj * plabelmdl = NULL ;
      switch ( pLABEL_FEATURE(pobj)->ptype )
      {
      case 1:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_title;
        break ;
      case 2:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_x_label;
        break ;
      case 3:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_y_label;
        break ;
      case 4:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_z_label;
        break ;
      default:
        /* arrgh*/
        return -1 ;
        break ;
      }
      cloneGraphicContext( plabelmdl, pobj ) ;
      return 0 ;
    }
  case SCI_AGREG:
  case SCI_TITLE:
  case SCI_PANNER:		/* pas de context graphics */
  case SCI_SBH:		/* pas de context graphics */
  case SCI_SBV:		/* pas de context graphics */
  case SCI_UIMENU:
  default:
    return -1;
    break;
  }
  return 0;
}

/**
 * Inits the font context of an object byt copying the one of an other.
 * @param pObjSource the object from which the FC is taken
 * @param pObjDest the object in which the FC is paste
 */
int initFCfromCopy(  sciPointObj * pObjSource, sciPointObj * pObjDest )
{
  sciGetFontContext( pObjDest )->fontnamelen = 0 ;
  sciGetFontContext( pObjDest )->pfontname = NULL ;
  return cloneFontContext( pObjSource, pObjDest ) ;
}

/**sciInitFontContext
 * Inits the graphic context of this object with the default value
 * @param sciPointObj * pobj: the pointer to the entity
 */
int
sciInitFontContext (sciPointObj * pobj)
{

  /* 
   * initialisation du contexte font par defaut 
   * que l'on peut recuperer sur les structures de base de scilab 
   * la colormap des fils est heritee du parent
   * nous prenons le couleur de background et foreground
   * plutot que fontforeground pour l'initialisation
   */

  /* unknown function initfontname "Win-stand"!! */
  /* static TCHAR inifontname[] = TEXT ("Times New Roman");*/  
  

  switch (sciGetEntityType (pobj))
  {
  case SCI_TEXT:
  case SCI_TITLE:
  case SCI_LEGEND:
  case SCI_AXES:
  case SCI_MENU:
  case SCI_MENUCONTEXT:
  case SCI_STATUSB:
  {
    initFCfromCopy( sciGetParent(pobj), pobj ) ;
  }
  break;
  case SCI_LABEL: /* Re-init here must be better F.Leray 28.05.04 */
    if ( sciGetParent(pobj) == paxesmdl )
    {
      initFCfromCopy( sciGetParent(pobj), pobj ) ;
    }
    else
    {
      sciPointObj * plabelmdl = NULL;
	  
      if(pLABEL_FEATURE(pobj)->ptype == 1) /* title */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_title;
      }
      else if(pLABEL_FEATURE(pobj)->ptype == 2) /* x_label */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_x_label;
      }
      else if(pLABEL_FEATURE(pobj)->ptype == 3) /* y_label */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_y_label;
      }
      else if(pLABEL_FEATURE(pobj)->ptype == 4) /* z_label */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_z_label;
      }
        
      initFCfromCopy( plabelmdl, pobj ) ;

    }
    break;
  case SCI_SUBWIN: 
    if (pobj == paxesmdl)
    {
      initFCfromCopy( sciGetParent( pobj ), pobj ) ;
    }
    else
    {
      initFCfromCopy( paxesmdl, pobj ) ;
    } 
    break;
  case SCI_FIGURE:
    if (pobj == pfiguremdl)
    {
      sciInitFontStyle (pobj, 6); /* set helvetica font */
      (sciGetFontContext(pobj))->backgroundcolor = -3;
      (sciGetFontContext(pobj))->foregroundcolor = -2;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
	  
      if (
        ((sciGetFontContext(pobj))->pfontname = CALLOC ((sciGetFontContext(pobj))->fontnamelen + 1,
                                                        sizeof (char))) == NULL)
      {
        sciprint ("No more Memory for fontname\n");
        return 0;
      }
      /* END ADDING F.Leray 08.04.04*/ 
    }
    else
    {
      initFCfromCopy( pfiguremdl, pobj ) ;
    }
    break;
  case SCI_PANNER:		/* pas de context graphics */
  case SCI_SBH:		/* pas de context graphics */
  case SCI_SBV:		/* pas de context graphics */
  case SCI_ARC:
  case SCI_SEGS: 
  case SCI_FEC: 
  case SCI_GRAYPLOT: 
  case SCI_POLYLINE:
  case SCI_RECTANGLE:
  case SCI_SURFACE:
  case SCI_LIGHT:
  case SCI_AGREG:
  case SCI_UIMENU:
  default:
    return -1;
    break;
  }
  return 0;
}

void initsubwin()  /* Interesting / F.Leray 05.04.04 */
{
  sciPointObj  * psubwin  ;
  sciSubWindow * ppSubWin ;
  

  Cscale2default(); 
  psubwin = sciGetCurrentSubWin();
  ppSubWin = pSUBWIN_FEATURE (psubwin) ;


  initSubWinSize( psubwin  ) ;

  clearUserData( psubwin ) ;

  reinitSubWin( psubwin ) ;
  
  (ppSubWin->axes).axes_visible[0] = FALSE;
  (ppSubWin->axes).axes_visible[1] = FALSE;
  (ppSubWin->axes).axes_visible[2] = FALSE;

  (ppSubWin->axes).reverse[0] = FALSE;
  (ppSubWin->axes).reverse[1] = FALSE;
  (ppSubWin->axes).reverse[2] = FALSE;
  
  ppSubWin->axes.rect = BT_OFF;  
  ppSubWin->axes.ticscolor = -1;
  ppSubWin->axes.subint[0] =  1;
  ppSubWin->axes.subint[1] =  1;
  ppSubWin->axes.subint[2] =  1;
  ppSubWin->axes.limits[0]  = 0; 

}

int InitFigureModel( void )
{ 
  int i ;
  int m = NUMCOLORS_SCI ;

  pFIGURE_FEATURE (pfiguremdl)->allredraw = FALSE;

  if ( sciInitGraphicContext(pfiguremdl) < 0 )
  {
    return -1 ;
  }
  if ( sciInitGraphicMode(pfiguremdl) < 0 )
  {
    return -1 ;
  }
  if ( sciInitFontContext(pfiguremdl) < 0)
  {
    /* F.Leray 10.06.04 */
    return -1 ;
  }
  strncpy (pFIGURE_FEATURE (pfiguremdl)->name, "Scilab Graphic", sizeof ("Scilab Graphic") + 4);
  pFIGURE_FEATURE (pfiguremdl)->namelen = Min (sizeof ("Scilab Graphic") + 4, 14); 
  pFIGURE_FEATURE (pfiguremdl)->number          = 0   ;
  pFIGURE_FEATURE (pfiguremdl)->figuredimwidth  = 610 ;
  pFIGURE_FEATURE (pfiguremdl)->figuredimheight = 461 ;
  pFIGURE_FEATURE (pfiguremdl)->windowdimwidth  = 600 ;
  pFIGURE_FEATURE (pfiguremdl)->windowdimheight = 400 ;
  pFIGURE_FEATURE (pfiguremdl)->inrootposx      = 200 ;
  pFIGURE_FEATURE (pfiguremdl)->inrootposy      = 200 ;
  
  if((pFIGURE_FEATURE(pfiguremdl)->pcolormap = (double *) MALLOC (m * 3 * sizeof (double))) == (double *) NULL)
    {
      strcpy(error_message,"Cannot init color map");
      return -1 ;
    }  
  for (i= 0 ; i < m ; i++)
  {
    pFIGURE_FEATURE(pfiguremdl)->pcolormap[i] = (double) (defcolors[3*i]/255.0);
    pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+m] = (double) (defcolors[3*i+1]/255.0); 
    pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+2*m] = (double) (defcolors[3*i+2]/255.0);
  }
  
  pFIGURE_FEATURE (pfiguremdl)->numcolors = m;
  pFIGURE_FEATURE (pfiguremdl)->isiconified = FALSE;
  pFIGURE_FEATURE (pfiguremdl)->isselected = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->rotstyle = 0;
  pFIGURE_FEATURE (pfiguremdl)->visible = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->auto_redraw = TRUE; /* by default, we draw live */ /* F.Leray 29.12.04 modified on 04.01.05 */

  pFIGURE_FEATURE (pfiguremdl)->user_data = (int *) NULL; /* pour completude */
  pFIGURE_FEATURE (pfiguremdl)->size_of_user_data = 0; /* pour completude */

  pFIGURE_FEATURE (pfiguremdl)->numsubwinselected = 0; 
  pFIGURE_FEATURE (pfiguremdl)->pixmap = 0 ; 
  pFIGURE_FEATURE (pfiguremdl)->wshow = 0 ;

  pFIGURE_FEATURE(pfiguremdl)->infoMessage = NULL ;
  sciSetInfoMessage( pfiguremdl, "" ) ;
  sciInitIsEventHandlerEnable( pfiguremdl, FALSE ) ;
  pFIGURE_FEATURE(pfiguremdl)->eventHandler = NULL ;
  sciInitEventHandler( pfiguremdl, "" ) ;

  pfiguremdl->pObservers = NULL ;
  pfiguremdl->pDrawer = NULL ;

  return 0;
}



int InitAxesModel()
{ 
  int i;
  double tab[] = {0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.}; /* graduations init. tmptab */
  sciPointObj * pobj = NULL;
  sciSubWindow * ppobj = NULL;
  sciSubWindow * ppaxesmdl = pSUBWIN_FEATURE (paxesmdl);

  sciInitGraphicContext (paxesmdl);
  sciInitGraphicMode (paxesmdl);
  sciInitFontContext (paxesmdl);  /* F.Leray 10.06.04 */
  ppaxesmdl->cube_scaling = FALSE;
  ppaxesmdl->callback = (char *)NULL;
  ppaxesmdl->callbacklen = 0;
  ppaxesmdl->callbackevent = 100;  
  ppaxesmdl->logflags[0] = 'n';
  ppaxesmdl->logflags[1] = 'n';
  ppaxesmdl->logflags[2] = 'n';
  ppaxesmdl->axes.ticscolor  = -1;
  ppaxesmdl->axes.subint[0]  = 1;   
  ppaxesmdl->axes.subint[1]  = 1; 
  ppaxesmdl->axes.subint[2]  = 1;
  ppaxesmdl->axes.xdir='d'; 
  ppaxesmdl->axes.ydir='l';  
  ppaxesmdl->axes.rect  = BT_OFF;
  
  ppaxesmdl->user_data = (int *) NULL; /* to be complete */
  ppaxesmdl->size_of_user_data = 0;

  for (i=0 ; i<7 ; i++)
  {
    ppaxesmdl->axes.limits[i] = 0;
  }

  for (i=0 ; i<3 ; i++)
  {
    ppaxesmdl->grid[i]  = -1;
  }

  ppaxesmdl->alpha  = 0.0;
  ppaxesmdl->theta  = 270.0;
  ppaxesmdl->alpha_kp  = 45.0;
  ppaxesmdl->theta_kp  = 215.0;
  ppaxesmdl->is3d  = FALSE;

  for (i=0 ; i<4 ; i++)
    {  
      ppaxesmdl->axes.xlim[i]= Cscale.xtics[i]; 
      ppaxesmdl->axes.ylim[i]= Cscale.ytics[i]; 
    }

  /* F.Leray 22.09.04 */
  (ppaxesmdl->axes).axes_visible[0] = FALSE;
  (ppaxesmdl->axes).axes_visible[1] = FALSE;
  (ppaxesmdl->axes).axes_visible[2] = FALSE;
  (ppaxesmdl->axes).reverse[0] = FALSE;
  (ppaxesmdl->axes).reverse[1] = FALSE;
  (ppaxesmdl->axes).reverse[2] = FALSE;
  ppaxesmdl->flagNax = FALSE;
  
  /*F.Leray : just for completion : */
  ppaxesmdl->axes.nbsubtics[0] = 1; /* not used at all because needs ppaxesmdl->flagNax = TRUE */
  ppaxesmdl->axes.nbsubtics[1] = 1; /* and when it is TRUE, it means WE have given the corresponding */
  ppaxesmdl->axes.nbsubtics[2] = 1; /* ppaxesmdl->axes.nbsubtics[0,1,2] !! */

  (ppaxesmdl->axes).nxgrads = 11; /* computed ticks */
  (ppaxesmdl->axes).nygrads = 11;
  (ppaxesmdl->axes).nzgrads = 3;

  for(i=0;i<11;i++)
  {
    ppaxesmdl->axes.xgrads[i] = tab[i];
  }
  
  for(i=0;i<11;i++)
  {
    ppaxesmdl->axes.ygrads[i] = tab[i];
  }

  (ppaxesmdl->axes).zgrads[0] = -1.;
  (ppaxesmdl->axes).zgrads[1]  = 0.;
  (ppaxesmdl->axes).zgrads[2]  = 1.;

  (ppaxesmdl->axes).u_xgrads = (double *)NULL;
  (ppaxesmdl->axes).u_ygrads = (double *)NULL;
  (ppaxesmdl->axes).u_zgrads = (double *)NULL;
  (ppaxesmdl->axes).u_nxgrads = 0; /* user ticks */
  (ppaxesmdl->axes).u_nygrads = 0;
  (ppaxesmdl->axes).u_nzgrads = 0;
  (ppaxesmdl->axes).u_xlabels= (char **) NULL;
  (ppaxesmdl->axes).u_ylabels= (char **) NULL;
  (ppaxesmdl->axes).u_zlabels= (char **) NULL;

  (ppaxesmdl->axes).auto_ticks[0] = TRUE;
  (ppaxesmdl->axes).auto_ticks[1] = TRUE;
  (ppaxesmdl->axes).auto_ticks[2] = TRUE;
  /* end 22.09.04 */

  ppaxesmdl->axes.zlim[0]= -1.0;
  ppaxesmdl->axes.zlim[1]= 1.0;  
  ppaxesmdl->axes.flag[0]= 2;
  ppaxesmdl->axes.flag[1]= 2;
  ppaxesmdl->axes.flag[2]= 4;
  ppaxesmdl->axes.hiddenAxisColor = 4 ;
  ppaxesmdl->project[0]= 1;
  ppaxesmdl->project[1]= 1;
  ppaxesmdl->project[2]= 0;
  ppaxesmdl->hiddencolor=4;
  ppaxesmdl->hiddenstate=0; 
  ppaxesmdl->isoview= FALSE;/*TRUE;*/
 
  ppaxesmdl->WRect[0]   = 0;
  ppaxesmdl->WRect[1]   = 0;
  ppaxesmdl->WRect[2]   = 1;
  ppaxesmdl->WRect[3]   = 1;  
  ppaxesmdl->FRect[0]   = 0.0; /* xmin */
  ppaxesmdl->FRect[1]   = 0.0; /* ymin */
  ppaxesmdl->FRect[2]   = 1.0; /* xmax */
  ppaxesmdl->FRect[3]   = 1.0; /* ymax */
  ppaxesmdl->FRect[4]   = -1.0;/* zmin */
  ppaxesmdl->FRect[5]   = 1.0;   /* zmax */
  ppaxesmdl->visible = sciGetVisibility(pfiguremdl); 
  /* /\*   ppaxesmdl->drawlater = sciGetDrawLater(pfiguremdl); *\/ */
  /*   ppaxesmdl->drawlater = FALSE; */

  ppaxesmdl->isclip = -1;  
  ppaxesmdl->pPopMenu = (sciPointObj *)NULL;

  /* Les SRect sont rentres dans l'ordre: 
     [xmin ymin zmin xmax ymax zmax] */
  ppaxesmdl->SRect[0]   = 0.0; /* xmin */
  ppaxesmdl->SRect[1]   = 1.0; /* xmax */
  ppaxesmdl->SRect[2]   = 0.0; /* ymin */
  ppaxesmdl->SRect[3]   = 1.0; /* ymax */
  ppaxesmdl->SRect[4]   = -1.0;/* zmin */
  ppaxesmdl->SRect[5]   = 1.0;  /* zmax */
  
  ppaxesmdl->tight_limits = FALSE;

  paxesmdl->pObservers = NULL ;
  paxesmdl->pDrawer = NULL ;

  /* F.Leray 10.06.04 */
  /* Adding default Labels inside Axes */
  /*------------------------------------------------------------------------------------*/
 
  pobj = paxesmdl;
  ppobj = pSUBWIN_FEATURE(paxesmdl);
  
   /******************************  title *************************/
  
  ppobj->mon_title = initLabel( paxesmdl ) ;

  if ( ppobj->mon_title == NULL )
  {
    return -1 ;
  }
  
  pLABEL_FEATURE(ppobj->mon_title)->ptype = 1;
  
  /******************************  x_label *************************/
  
  ppobj->mon_x_label = initLabel( paxesmdl ) ;
  
  if ( ppobj->mon_x_label == NULL )
  {
    return -1 ;
  }
  
  pLABEL_FEATURE( ppobj->mon_x_label )->ptype = 2 ;
  
  /******************************  y_label *************************/
  
  ppobj->mon_y_label = initLabel( paxesmdl ) ;
  
  if ( ppobj->mon_y_label == NULL )
  {
    return -1 ;
  }
  
  pLABEL_FEATURE( ppobj->mon_y_label )->ptype = 3 ;
  
  /******************************  z_label *************************/
  
  ppobj->mon_z_label = initLabel( paxesmdl ) ;
  
  if ( ppobj->mon_z_label == NULL )
  {
    return -1 ;
  }
  
  pLABEL_FEATURE( ppobj->mon_z_label )->ptype = 4 ;
  
  return 0; 
}



int ResetFigureToDefaultValues(sciPointObj * pobj)
{
  /* integer i , m, n; */
  integer x[2], verbose=0, narg=0; 
  struct BCG *XGC=NULL;
  

  if(sciGetEntityType(pobj)!=SCI_FIGURE) /* MUST BE used for figure entities only */
    return -1;

  XGC = pFIGURE_FEATURE (pobj)->pScilabXgc;
  
  pFIGURE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
  pFIGURE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

  /** Initialize the colormap */
  /* try to install the colormap in the graphic context */

  pFIGURE_FEATURE (pobj)->pcolormap = NULL ;
  sciInitNumColors(pobj, 0) ;

  sciSetColormap( pobj, pFIGURE_FEATURE(pfiguremdl)->pcolormap, sciGetNumColors(pfiguremdl), 3 ) ;

   
  /* initialisation de context et mode graphique par defaut (figure model)*/
  if (sciInitGraphicContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pFIGURE_FEATURE(pobj)->pcolormap);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return -1;
    }
  if (sciInitGraphicMode (pobj) == -1)
    {
      sciDelHandle (pobj);    
      FREE(pFIGURE_FEATURE(pobj)->pcolormap);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return -1;
    }   

  /* F.Leray 08.04.04 */
  if (sciInitFontContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);	  
      FREE(pobj);
      return -1;
    }
  sciSetNum (pobj, &(XGC->CurWindow));		   
  sciSetName(pobj, sciGetName(pfiguremdl), sciGetNameLength(pfiguremdl));
  sciSetResize((sciPointObj *) pobj,sciGetResize(pobj));
  pFIGURE_FEATURE(pobj)->windowdimwidth=pFIGURE_FEATURE(pfiguremdl)->windowdimwidth;  
  pFIGURE_FEATURE(pobj)->windowdimheight=pFIGURE_FEATURE(pfiguremdl)->windowdimheight;
  C2F(dr)("xset","wdim",&(pFIGURE_FEATURE(pobj)->windowdimwidth),
	  &(pFIGURE_FEATURE(pobj)->windowdimheight),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
  pFIGURE_FEATURE (pobj)->figuredimwidth = pFIGURE_FEATURE (pfiguremdl)->figuredimwidth;
  pFIGURE_FEATURE (pobj)->figuredimheight = pFIGURE_FEATURE (pfiguremdl)->figuredimheight;
  C2F(dr)("xset","wpdim",&(pFIGURE_FEATURE(pobj)->figuredimwidth),
	  &(pFIGURE_FEATURE(pobj)->figuredimheight),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wpos",&verbose,x,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,4L);
  x[0]=(pFIGURE_FEATURE (pfiguremdl)->inrootposx <0)?x[0]:pFIGURE_FEATURE (pfiguremdl)->inrootposx;
  x[1]=(pFIGURE_FEATURE (pfiguremdl)->inrootposy <0)?x[1]:pFIGURE_FEATURE (pfiguremdl)->inrootposy;
  x[0]=(pFIGURE_FEATURE (pfiguremdl)->inrootposx <0)?x[0]:pFIGURE_FEATURE (pfiguremdl)->inrootposx;
  x[1]=(pFIGURE_FEATURE (pfiguremdl)->inrootposy <0)?x[1]:pFIGURE_FEATURE (pfiguremdl)->inrootposy;
  sciSetScreenPosition(pobj,x[0],x[1]);
  pFIGURE_FEATURE (pobj)->isiconified = pFIGURE_FEATURE (pfiguremdl)->isiconified;
  pFIGURE_FEATURE (pobj)->isselected = pFIGURE_FEATURE (pfiguremdl)->isselected; 
  pFIGURE_FEATURE (pobj)->rotstyle = pFIGURE_FEATURE (pfiguremdl)->rotstyle;
  pFIGURE_FEATURE (pobj)->visible = pFIGURE_FEATURE (pfiguremdl)->visible;
  pFIGURE_FEATURE (pobj)->auto_redraw = pFIGURE_FEATURE (pfiguremdl)->auto_redraw;
  pFIGURE_FEATURE (pobj)->numsubwinselected = pFIGURE_FEATURE (pfiguremdl)->numsubwinselected;
  pFIGURE_FEATURE (pobj)->pixmap = pFIGURE_FEATURE (pfiguremdl)->pixmap ; 
  pFIGURE_FEATURE (pobj)->wshow = pFIGURE_FEATURE (pfiguremdl)->wshow ; 

  return 0;
}



/**sciInitGraphicMode
 * Inits the graphic mode of this object with the default value
 */
int
sciInitGraphicMode (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (pobj == pfiguremdl)
	{
	  (sciGetGraphicMode (pobj))->wresize = TRUE;
	  (sciGetGraphicMode (pobj))->addplot = TRUE;
	  (sciGetGraphicMode (pobj))->autoscaling = TRUE;
	  (sciGetGraphicMode (pobj))->zooming = FALSE;
	  (sciGetGraphicMode (pobj))->xormode = 3;
	}
      else
	{
	  (sciGetGraphicMode (pobj))->wresize = (sciGetGraphicMode (pfiguremdl))->wresize;
	  (sciGetGraphicMode (pobj))->addplot = (sciGetGraphicMode (pfiguremdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (pfiguremdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming = (sciGetGraphicMode (pfiguremdl))->zooming;
	  (sciGetGraphicMode (pobj))->xormode = (sciGetGraphicMode (pfiguremdl))->xormode;
	}
      break;
    case SCI_SUBWIN:
      if (pobj == paxesmdl)
	{
	  (sciGetGraphicMode (pobj))->wresize =sciGetResize (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->addplot =sciGetAddPlot (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->autoscaling =sciGetAutoScale (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->zooming =sciGetZooming (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->xormode =sciGetXorMode (sciGetParent (pobj));
      	}
      else
	{
	  (sciGetGraphicMode (pobj))->addplot =(sciGetGraphicMode (paxesmdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (paxesmdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming =(sciGetGraphicMode (paxesmdl))->zooming;
	  (sciGetGraphicMode (pobj))->xormode =(sciGetGraphicMode (paxesmdl))->xormode;
      	}
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
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      sciprint ("This object haven't any graphic mode\n");
      return -1;
      break;
    }
  return 0;
}

/*------------------------------------------------------------------------------------------*/
/* allocate and set a new label to default values */
sciPointObj * initLabel( sciPointObj * pParentObj )
{
  sciPointObj * newLabel = MALLOC (sizeof (sciPointObj)) ;
  sciLabel    * ppLabel ;
  char        * emptyString = "" ;

  if ( newLabel == NULL )
  {
    return NULL ;
  }

  sciSetEntityType( newLabel, SCI_LABEL ) ;

  newLabel->pfeatures = MALLOC ( sizeof (sciLabel) ) ;
  if ( newLabel->pfeatures == NULL)
  {
    FREE( newLabel );
    return NULL ;
  }
  
  ppLabel = pLABEL_FEATURE( newLabel ) ;
  
  /* we must first construct the text object inside the label */
  ppLabel->text = allocateText( pParentObj, &emptyString, 1, 1,
                                0.0, 0.0, TRUE, NULL, FALSE, NULL, NULL,
                                FALSE, FALSE, FALSE, ALIGN_LEFT ) ;
  
  if ( ppLabel->text == NULL )
  {
    deallocateText( ppLabel->text ) ;
    FREE( ppLabel ) ;
    FREE( newLabel  );
    return NULL ;
  }

  if ( sciAddNewHandle ( newLabel ) == -1 )
  {
    FREE( ppLabel ) ;
    FREE( newLabel ) ;
    return NULL ;
  }
  
  if ( !sciAddThisToItsParent( newLabel, pParentObj ) )
  {
    sciDelHandle (newLabel);
    FREE( ppLabel ) ;
    FREE( newLabel  );
    return NULL ;
  }

  sciInitSelectedSons( newLabel ) ;

  ppLabel->auto_position = TRUE;
  ppLabel->auto_rotation = TRUE;

  ppLabel->isselected = TRUE;

  ppLabel->ptype = 0 ; /* must be changed : 1 for title, 2 x_label, 3 y_label, 4 z_label */
 
  sciInitIs3d( newLabel, FALSE ) ;

  newLabel->pObservers = NULL ;
  newLabel->pDrawer = NULL ;

  if ( sciInitGraphicContext( newLabel ) == -1 )
  {
    DestroyLabel( newLabel ) ;
    return NULL ;
  }
 
  if ( sciInitFontContext( newLabel ) == -1 )
  {
    DestroyLabel( newLabel ) ;
    return NULL ;
  }
  
  

  return newLabel ;
}

/*------------------------------------------------------------------------------------------*/
void destroyDefaultObjects( void )
{
  DestroyAllGraphicsSons( pfiguremdl ) ;
  DestroyFigure( pfiguremdl ) ;
  pfiguremdl = NULL ;
}
/*------------------------------------------------------------------------------------------*/
