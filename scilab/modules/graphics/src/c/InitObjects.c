/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
 *    This file contains all functions used to Init or Re-Init the window
 *    (Figure and/or Subwin) to the default graphics properties.
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "InitObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "PloEch.h"
#include "Axes.h"
#include "DestroyObjects.h"
#include "CloneObjects.h"
#include "BuildObjects.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "Interaction.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

static char error_message[70]; /* DJ.A 08/01/04 */
unsigned short defcolors[] = {
  0,   0,   0, /* Black: DEFAULTBLACK */
  0,   0, 255, /* Blue */
  0, 255,   0, /* Green */
  0, 255, 255, /* Cyan */
  255,   0,   0, /* Red */
  255,   0, 255, /* Magenta */
  255,   255,   0, /* Yellow */
  255, 255, 255, /* White: DEFAULTWHITE */
  0,   0, 144, /* Blue4 */
  0,   0, 176, /* Blue3 */
  0,   0, 208, /* Blue2 */
  135, 206, 255, /* LtBlue */
  0, 144,   0, /* Green4 */
  0, 176,   0, /* Green3 */
  0, 208,   0, /* Green2 */
  0, 144, 144, /* Cyan4 */
  0, 176, 176, /* Cyan3 */
  0, 208, 208, /* Cyan2 */
  144,   0,   0, /* Red4 */
  176,   0,   0, /* Red3 */
  208,   0,   0, /* Red2 */
  144,   0, 144, /* Magenta4 */
  176,   0, 176, /* Magenta3 */
  208,   0, 208, /* Magenta2 */
  128,  48,   0, /* Brown4 */
  160,  64,   0, /* Brown3 */
  192,  96,   0, /* Brown2 */
  255, 128, 128, /* Pink4 */
  255, 160, 160, /* Pink3 */
  255, 192, 192, /* Pink2 */
  255, 224, 224, /* Pink */
  255, 215,   0  /* Gold */
};

static sciPointObj * pfiguremdl = NULL;
static sciPointObj * paxesmdl = NULL;

sciPointObj * getFigureModel( void )
{
  return pfiguremdl ;
}

sciPointObj * getAxesModel( void )
{
  return paxesmdl ;
}

BOOL isFigureModel(sciPointObj * pObj)
{
  return (pObj == pfiguremdl);
}

BOOL isAxesModel(sciPointObj * pObj)
{
  return (pObj == paxesmdl);
}


/* DJ.A 08/01/04 */
int C2F(graphicsmodels) (void)
{
  sciSubWindow * ppaxesmdl = NULL ;

  if ((pfiguremdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      strcpy(error_message,_("Default figure cannot be create.\n"));
      return 0;
    }
  sciSetEntityType (pfiguremdl, SCI_FIGURE);
  if ((pfiguremdl->pfeatures = MALLOC ((sizeof (sciFigure)))) == NULL)
    {
      FREE(pfiguremdl);
      strcpy(error_message,_("Default figure cannot be create.\n"));
      return 0;
    }

	createDefaultRelationShip(pfiguremdl);

	/* add the handle in the handle list */
  if ( sciAddNewHandle(pfiguremdl) == -1 )
  {
    return NULL ;
  }

  if (!(sciAddThisToItsParent(pfiguremdl, (sciPointObj *)NULL)))
    {
      sciDelHandle (pfiguremdl);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,_("Default figure cannot be create.\n"));
      return 0;
    }

  sciInitSelectedSons( pfiguremdl ) ;

  /* set default properties */
  if ( InitFigureModel() < 0 )
  {
    sciDelHandle (pfiguremdl);
    FREE(pfiguremdl->pfeatures);
    FREE(pfiguremdl);
    strcpy(error_message,_("Default figure cannot be create.\n"));
    return 0;
  }

  /* F.Leray Adding some FontContext Info for InitFontContext function */
  /*  pFIGURE_FEATURE (pfiguremdl)->fontcontext.backgroundcolor = */


  if ((paxesmdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      strcpy(error_message,_("Default axes cannot be create.\n"));
      return 0;
    }
  sciSetEntityType (paxesmdl, SCI_SUBWIN);
  if ((paxesmdl->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
    {
      FREE(paxesmdl);
      strcpy(error_message,_("Default axes cannot be create.\n"));
      return 0;
    }
	createDefaultRelationShip(paxesmdl);

	/* add the handle in the handle list */
  if ( sciAddNewHandle(paxesmdl) == -1 )
  {
    return NULL ;
  }

  if (!(sciAddThisToItsParent (paxesmdl, pfiguremdl)))
    {
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);
      strcpy(error_message,_("Default axes cannot be create.\n"));
      return 0;
    }

  ppaxesmdl =  pSUBWIN_FEATURE (paxesmdl);

  /*sciInitSelectedSons( paxesmdl ) ;
  ppaxesmdl->relationship.psons = (sciSons *) NULL;
  ppaxesmdl->relationship.plastsons = (sciSons *) NULL;*/

  if ( InitAxesModel() < 0 )
  {
    sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
    sciDelHandle (paxesmdl);
    FREE(paxesmdl->pfeatures);
    FREE(paxesmdl);
    strcpy(error_message,_("Default axes cannot be create.\n"));
    return 0;
  }

  /* there are properties not initialized bu InitAxesModel */
  /* Is it a missing ? */

  ppaxesmdl->FirstPlot = TRUE;

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
      (sciGetGraphicContext(pobj))->backgroundcolor = /*-3;*/ 33;  /* F.Leray 29.03.04: Wrong index here: 32+1 (old method) must be changed to -1 new method*/
      (sciGetGraphicContext(pobj))->foregroundcolor = /*-2;*/ 32;  /* F.Leray 29.03.04: Wrong index here: 32+2 (old method) must be changed to -2 new method*/
      (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
      (sciGetGraphicContext(pobj))->linewidth = 1;
      (sciGetGraphicContext(pobj))->linestyle = 0; /* solid */
      (sciGetGraphicContext(pobj))->ismark    = FALSE;
      (sciGetGraphicContext(pobj))->isline    = TRUE;
      (sciGetGraphicContext(pobj))->isfilled  = FALSE;
      (sciGetGraphicContext(pobj))->markstyle = 0;
      (sciGetGraphicContext(pobj))->marksize = 0 ; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->markbackground = /*-3;*/ 33; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->markforeground = /*-2;*/ 32; /* New F.Leray 21.01.05 */
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
      sciGetGraphicContext(pobj)->backgroundcolor = /*-3 ;*/ 33;
      sciGetGraphicContext(pobj)->foregroundcolor = /*-2 ;*/ 32;
      sciGetGraphicContext(pobj)->markbackground  = /*-3*/ 33;
      sciGetGraphicContext(pobj)->markforeground  = /*-2*/ 32;
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
  case SCI_AXES:
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
  case SCI_UIMENU:
  default:
    return -1;
    break;
  }
}

/**
 * Inits the font context of an object byt copying the one of an other.
 * @param pObjSource the object from which the FC is taken
 * @param pObjDest the object in which the FC is paste
 */
int initFCfromCopy(  sciPointObj * pObjSource, sciPointObj * pObjDest )
{
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
  case SCI_LEGEND:
  case SCI_AXES:
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
      (sciGetFontContext(pobj))->backgroundcolor = 33;
      (sciGetFontContext(pobj))->foregroundcolor = 32;
      (sciGetFontContext(pobj))->fontSize = 1.0;
      (sciGetFontContext(pobj))->textorientation = 0.0;
      (sciGetFontContext(pobj))->useFractionalMetrics = FALSE;
      /* END ADDING F.Leray 08.04.04*/
    }
    else
    {
      initFCfromCopy( pfiguremdl, pobj ) ;
    }
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
  return 0;
}

int InitFigureModel( void )
{
  int i ;
  int m = NUMCOLORS_SCI ;
  double * colorMap ;
  sciFigure * ppFigure = pFIGURE_FEATURE( pfiguremdl) ;

  ppFigure->allredraw = FALSE;

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
  
  sciInitName(pfiguremdl, _("Graphic window number %d"));
  pFIGURE_FEATURE (pfiguremdl)->number          = 0   ;

  /* Set figure model attributes */
  ppFigure->pModelData = newFigureModelData() ;

  pFIGURE_FEATURE (pfiguremdl)->isselected = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->rotstyle = 0;
  pFIGURE_FEATURE (pfiguremdl)->visible = TRUE;
  /* auto_redraw */
  sciInitImmediateDrawingMode(pfiguremdl, TRUE);/* by default, we draw live */

  pFIGURE_FEATURE (pfiguremdl)->user_data = (int *) NULL; /* pour completude */
  pFIGURE_FEATURE (pfiguremdl)->size_of_user_data = 0; /* pour completude */

  pFIGURE_FEATURE (pfiguremdl)->numsubwinselected = 0;
  sciInitPixmapMode(pfiguremdl, FALSE);
  sciInitInfoMessage( pfiguremdl, "") ;

  /*
  ** Must set Event Handler before making it enable
  ** Otherwise causes a warning.
  */
  // sciInitEventHandler(pfiguremdl, "");
  pFIGURE_FEATURE(pfiguremdl)->eventHandler = strdup("");
  // sciInitIsEventHandlerEnable( pfiguremdl, FALSE ) ;
  pFIGURE_FEATURE(pfiguremdl)->isEventHandlerEnable = FALSE;

  pFIGURE_FEATURE(pfiguremdl)->tag = NULL ;

  pfiguremdl->pObservers = NULL ;
  pfiguremdl->pDrawer = NULL ;
  colorMap = MALLOC( m * 3 * sizeof(double) ) ;
  if ( colorMap == NULL )
  {
	  sprintf(error_message,_("%s: No more memory.\n"),"InitFigureModel");
	  return -1 ;
  }

  for ( i = 0 ; i < m ; i++ )
  {
    colorMap[i        ] = (double) (defcolors[3*i]/255.0) ;
    colorMap[i + m    ] = (double) (defcolors[3*i+1]/255.0) ;
    colorMap[i + 2 * m] = (double) (defcolors[3*i+2]/255.0) ;
  }

  sciSetColormap( pfiguremdl, colorMap, m, 3 ) ;
  pFIGURE_FEATURE (pfiguremdl)->numcolors = m;

  FREE( colorMap ) ;

  return 0;
}



int InitAxesModel()
{
  int i;
  double tab[] = {0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.}; /* graduations init. tmptab */
  sciSubWindow * ppaxesmdl = pSUBWIN_FEATURE (paxesmdl);
  char linLogFlags[3] = {'n','n','n'};

  sciInitGraphicContext (paxesmdl);
  sciInitGraphicMode (paxesmdl);
  sciInitFontContext (paxesmdl);  /* F.Leray 10.06.04 */
  ppaxesmdl->cube_scaling = FALSE;
  ppaxesmdl->callback = (char *)NULL;
  ppaxesmdl->callbacklen = 0;
  ppaxesmdl->callbackevent = 100;
  sciInitLogFlags(paxesmdl, linLogFlags);
  ppaxesmdl->axes.ticscolor  = -1;
  ppaxesmdl->axes.subint[0]  = 1;
  ppaxesmdl->axes.subint[1]  = 1;
  ppaxesmdl->axes.subint[2]  = 1;
  ppaxesmdl->axes.xdir='d';
  ppaxesmdl->axes.ydir='l';
  ppaxesmdl->axes.rect  = BT_OFF;
  sciInitIsFilled(paxesmdl, TRUE);

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

	ppaxesmdl->gridFront = FALSE; /* draw in background */

  ppaxesmdl->alpha  = 0.0;
  ppaxesmdl->theta  = 270.0;
  ppaxesmdl->alpha_kp  = 45.0;
  ppaxesmdl->theta_kp  = 215.0;
  ppaxesmdl->is3d  = FALSE;

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

  sciInitAutoTicks(paxesmdl, TRUE, TRUE, TRUE);
  /* end 22.09.04 */

  ppaxesmdl->axes.flag[0]= 2;
  ppaxesmdl->axes.flag[1]= 2;
  ppaxesmdl->axes.flag[2]= 4;
  sciInitHiddenAxisColor(paxesmdl, 4);
  ppaxesmdl->project[0]= 1;
  ppaxesmdl->project[1]= 1;
  ppaxesmdl->project[2]= 0;
  sciInitHiddenColor(paxesmdl, 4);
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

  ppaxesmdl->isclip = -1; /* off */

  /* Les SRect sont rentres dans l'ordre:
     [xmin ymin zmin xmax ymax zmax] */
  ppaxesmdl->SRect[0]   = 0.0; /* xmin */
  ppaxesmdl->SRect[1]   = 1.0; /* xmax */
  ppaxesmdl->SRect[2]   = 0.0; /* ymin */
  ppaxesmdl->SRect[3]   = 1.0; /* ymax */
  ppaxesmdl->SRect[4]   = -1.0;/* zmin */
  ppaxesmdl->SRect[5]   = 1.0;  /* zmax */

  ppaxesmdl->tight_limits = FALSE;

  /* By default arcs don't use nurbs since it is faster */
  sciInitUseNurbs(paxesmdl, FALSE);

  paxesmdl->pObservers = NULL ;
  paxesmdl->pDrawer = NULL ;

  /* F.Leray 10.06.04 */
  /* Adding default Labels inside Axes */
  /*---------------------------------------------------------------------------*/

   /******************************  title *************************/

  ppaxesmdl->mon_title = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_title == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE(ppaxesmdl->mon_title)->ptype = 1;

  /******************************  x_label *************************/

  ppaxesmdl->mon_x_label = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_x_label == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE( ppaxesmdl->mon_x_label )->ptype = 2 ;

  /******************************  y_label *************************/

  ppaxesmdl->mon_y_label = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_y_label == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE( ppaxesmdl->mon_y_label )->ptype = 3 ;

  /******************************  z_label *************************/

  ppaxesmdl->mon_z_label = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_z_label == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE( ppaxesmdl->mon_z_label )->ptype = 4 ;

  return 0;
}



int ResetFigureToDefaultValues(sciPointObj * pobj)
{

  int x[2];


  if(sciGetEntityType(pobj)!=SCI_FIGURE) /* MUST BE used for figure entities only */
    return -1;

  /** Initialize the colormap */
  /* try to install the colormap in the graphic context */
  sciSetColormap( pobj, pFIGURE_FEATURE(pfiguremdl)->pModelData->colorMap, sciGetNumColors(pfiguremdl), 3 ) ;


  /* initialisation de context et mode graphique par defaut (figure model)*/
  if (sciInitGraphicContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return -1;
    }
  if (sciInitGraphicMode (pobj) == -1)
    {
      sciDelHandle (pobj);
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

  sciInitName(pobj, sciGetName(pfiguremdl));
  sciSetResize(pobj,sciGetResize(pfiguremdl));
	sciSetDimension( pobj, sciGetWidth(pfiguremdl), sciGetHeight(pfiguremdl) );
	if (!sciGetResize(pobj))
	{
		/* window size and axes size may change independently */
		sciSetWindowDim( pobj, sciGetWindowWidth(pfiguremdl), sciGetWindowHeight(pfiguremdl) ) ;	
	}

  sciGetScreenPosition(pfiguremdl, &x[0], &x[1]) ;
  sciSetScreenPosition(pobj,x[0],x[1]);
  pFIGURE_FEATURE (pobj)->isselected = pFIGURE_FEATURE (pfiguremdl)->isselected;
  pFIGURE_FEATURE (pobj)->rotstyle = pFIGURE_FEATURE (pfiguremdl)->rotstyle;
  pFIGURE_FEATURE (pobj)->visible = pFIGURE_FEATURE (pfiguremdl)->visible;
  sciInitImmediateDrawingMode(pobj, sciGetImmediateDrawingMode(pfiguremdl));
  pFIGURE_FEATURE (pobj)->numsubwinselected = pFIGURE_FEATURE (pfiguremdl)->numsubwinselected;
  sciInitPixmapMode(pobj, sciGetPixmapMode(pfiguremdl));

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
	  (sciGetGraphicMode (pobj))->addplot = TRUE;
	  (sciGetGraphicMode (pobj))->autoscaling = TRUE;
	  (sciGetGraphicMode (pobj))->zooming = FALSE;
	  (sciGetGraphicMode (pobj))->xormode = 3; /* copy */
	}
      else
	{
	  (sciGetGraphicMode (pobj))->addplot = (sciGetGraphicMode (pfiguremdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (pfiguremdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming = (sciGetGraphicMode (pfiguremdl))->zooming;
	  (sciGetGraphicMode (pobj))->xormode = (sciGetGraphicMode (pfiguremdl))->xormode;
	}
      break;
    case SCI_SUBWIN:
      if (pobj == paxesmdl)
	{
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
      sciprint (_("This object has not any graphic mode\n"));
      return -1;
      break;
    }
  return 0;
}

/*---------------------------------------------------------------------------------*/
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

	/* RelationShip is actually stored in the text object */
	newLabel->relationShip = ppLabel->text->relationShip;

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

/*---------------------------------------------------------------------------------*/
void destroyDefaultObjects( void )
{
  /* will destroy the figure and its children (so the axes). */
  destroyGraphicHierarchy( pfiguremdl ) ;
  pfiguremdl = NULL ;
  paxesmdl = NULL;
}
/*---------------------------------------------------------------------------------*/
/**
 * Create new data with defautl properties.
 */
FigureModelData * newFigureModelData( void )
{
  FigureModelData * modelData = MALLOC(sizeof(FigureModelData)) ;

  if ( modelData == NULL ) { return NULL ; }

  modelData->figureWidth  = 610 ;
  modelData->figureHeight = 460 ;
  modelData->windowWidth  = 620 ;
  modelData->windowHeight = 590 ;
  modelData->windowPosition[0] = 200 ; /* Set [-1,-1] to let the os use the position */
  modelData->windowPosition[1] = 200 ;
  modelData->colorMap = NULL ;
  modelData->numColors = 0 ;
  modelData->autoResizeMode = TRUE;
  modelData->viewport[0] = 0;
  modelData->viewport[1] = 0;
  modelData->viewport[2] = 610;
  modelData->viewport[3] = 461;
	modelData->infoMessage = NULL;

  return modelData ;
}
/*---------------------------------------------------------------------------------*/
/**
 * Free an existing model Data
 */
void destroyFigureModelData( FigureModelData * data )
{
  if ( data != NULL )
  {
		if (data->colorMap != NULL)
		{
			FREE(data->colorMap);
			data->colorMap = NULL;
		}

		if (data->infoMessage != NULL)
		{
			FREE(data->infoMessage);
			data->infoMessage = NULL;
		}

    FREE( data ) ;
    data = NULL ;
  }
}
/*---------------------------------------------------------------------------------*/
/**
 * Set the colormap of a figure to the default one.
 */
void sciSetDefaultColorMap(sciPointObj * pFigure)
{
  int numColor = sciGetNumColors(getFigureModel());
  double * colorMap = MALLOC( 3 * numColor * sizeof(double) );
  if (colorMap == NULL)
  {
    sciprint(_("%s: No more memory.\n"),"sciSetDefaultColorMap");
    return;
  }
  sciGetColormap(getFigureModel(), colorMap);
  sciSetColormap(pFigure, colorMap, numColor, 3);

  FREE(colorMap);
}
/*---------------------------------------------------------------------------------*/
/**
 * @return TRUE if pObj is one of the model objects, FALSE otherwise
 */
BOOL isModelObject(sciPointObj * pObj)
{
	return pObj == pfiguremdl
		|| pObj == paxesmdl
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_title
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label;
}
/*---------------------------------------------------------------------------------*/
