/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to BUILD new objects : 
 - allocating memory
 - setting default value
 - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#if WIN32
#include <windows.h>
#endif

#include "BuildObjects.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DestroyObjects.h"
#include "bcg.h"
#include "BuildObjects.h"
#include "SetProperty.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


extern int LinearScaling2Colormap(sciPointObj* pobj);
extern double * AllocUserGrads(double * u_xgrads, int nb);
extern char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads);
extern int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim);

extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;
extern unsigned short defcolors[];

/**ConstructStatusBar
 * @memo this function creates the StatusBar of Figure and the elementaries structures
 */
sciPointObj *
ConstructStatusBar (sciPointObj * pparentfigure)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobj, SCI_STATUSB);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciStatusBar)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSTATUSB_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSTATUSB_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciInitFontContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return (sciPointObj *)pobj;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
}





/**ConstructFigure
 * @memo This function creates the parents window (manager) and the elementaries structures
 */
/************ 18/01/2002 ***********/
sciPointObj *
ConstructFigure (XGC)
     struct BCG *XGC;
{
 
  sciPointObj *pobj = (sciPointObj *) NULL;
  integer i , m, n;
  integer x[2], verbose=0, narg=0;
  int succeed = 0;


  /* memory allocation for the new Figure   affectation du type allocation de la structure */

  if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      return NULL;	  
    }
  sciSetEntityType (pobj, SCI_FIGURE);
  if ((pobj->pfeatures = MALLOC ((sizeof (sciFigure)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  /* add a number in the HandelTable  reallocation de la table + 1 puis affectation de l'indice    */
  
  if (sciAddNewHandle (pobj) == -1)
    {
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
 
  /*  le seul parent est la root (l'ecran, le bureau quoi !) Pour l'instant il n'y a pas de fils selectionne */
  /*sciSetParent (pobj, (sciPointObj *) NULL); */
  /* il n'y a pas de parents !!!*/
  
  if (!(sciAddThisToItsParent(pobj, (sciPointObj *)NULL))) 
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  
  sciSetCurrentSon (pobj, (sciPointObj *) NULL);

  pFIGURE_FEATURE (pobj)->user_data = (int *) NULL; /* adding 27.06.05 */
  pFIGURE_FEATURE (pobj)->size_of_user_data = 0;
  pFIGURE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
  pFIGURE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
  pFIGURE_FEATURE (pobj)->pScilabXgc = XGC;
  
  /** Initialize the colormap */
  n=3;
  m = pFIGURE_FEATURE (pfiguremdl)->numcolors;
  /* try to install the colormap in the graphic context */
  C2F(dr)("xset","colormap",&m,&n,&succeed,PI0,PI0,PI0,
	  pFIGURE_FEATURE(pfiguremdl)->pcolormap,PD0,PD0,PD0,0L,0L);
  
  if(succeed == 1){ /* failed to allocate or xinit (for Gif driver) was missing */
    sciprint ("Failed to load default colormap : Allocation failed or missing xinit detected\n");
    return (sciPointObj *) NULL;
  }
  
  if((pFIGURE_FEATURE(pobj)->pcolormap = (double *) MALLOC (XGC->Numcolors * n * sizeof (double))) == (double *) NULL)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }  

  if (XGC->Numcolors == m) { 
    /* xset('colormap') was  able to install the colormap */
    for (i=0; i <m*n ; i++) {
      pFIGURE_FEATURE(pobj)->pcolormap[i] = pFIGURE_FEATURE(pfiguremdl)->pcolormap[i];
    }
  }
  else {
    m=XGC->Numcolors;
    for (i=0; i <m*n ; i++)
      pFIGURE_FEATURE(pobj)->pcolormap[i] = defcolors[i]/255.0;
  }

  sciSetNumColors (pobj,m);
   
  /* initialisation de context et mode graphique par defaut (figure model)*/
  if (sciInitGraphicContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pFIGURE_FEATURE(pobj)->pcolormap);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  if (sciInitGraphicMode (pobj) == -1)
    {
      sciDelHandle (pobj);    
      FREE(pFIGURE_FEATURE(pobj)->pcolormap);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }   

  /* F.Leray 08.04.04 */
  if (sciInitFontContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);	  
      FREE(pobj);
      return (sciPointObj *) NULL;
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
  sciSetFigurePos (pobj,x[0],x[1]);
  pFIGURE_FEATURE (pobj)->isiconified = pFIGURE_FEATURE (pfiguremdl)->isiconified;
  pFIGURE_FEATURE (pobj)->isselected = pFIGURE_FEATURE (pfiguremdl)->isselected; 
  pFIGURE_FEATURE (pobj)->rotstyle = pFIGURE_FEATURE (pfiguremdl)->rotstyle;
  pFIGURE_FEATURE (pobj)->visible = pFIGURE_FEATURE (pfiguremdl)->visible;
  pFIGURE_FEATURE (pobj)->auto_redraw = pFIGURE_FEATURE (pfiguremdl)->auto_redraw;

  pFIGURE_FEATURE (pobj)->numsubwinselected = pFIGURE_FEATURE (pfiguremdl)->numsubwinselected;
  pFIGURE_FEATURE (pobj)->pixmap = pFIGURE_FEATURE (pfiguremdl)->pixmap ; 
  pFIGURE_FEATURE (pobj)->wshow = pFIGURE_FEATURE (pfiguremdl)->wshow ; 
  pFIGURE_FEATURE (pobj)->allredraw = pFIGURE_FEATURE (pfiguremdl)->allredraw;

  return pobj;
}







/**ConstructSubWin
 * @memo This function creates the Subwindow (the Axe) and the elementaries structures
 */
sciPointObj *
ConstructSubWin (sciPointObj * pparentfigure, int pwinnum)
{

  char dir;
  int i;
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciSubWindow * ppsubwin = NULL; 
  sciSubWindow * ppaxesmdl = pSUBWIN_FEATURE (paxesmdl); 
  

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      /*if (sciInitChildWin (pparentfigure, pwinname) == -1)
	return NULL;*/
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobj, SCI_SUBWIN);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      
      ppsubwin =  pSUBWIN_FEATURE (pobj); /* debug */

      ppsubwin->vertices_list = (Vertices*) NULL;

      /*      ppsubwin->value_xm = (int *) NULL; */
      /*       ppsubwin->value_ym = (int *) NULL; */
      /*       ppsubwin->value_x = (double *) NULL; */
      /*       ppsubwin->value_y = (double *) NULL; */
      /*       ppsubwin->value_z = (double *) NULL; */
  
      /*       ppsubwin->nb_vertices_in_merge = 0; */
      ppsubwin->user_data = (int *) NULL; /* adding 27.06.05 */
      ppsubwin->size_of_user_data = 0;
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      ppsubwin->relationship.psons = (sciSons *) NULL;
      ppsubwin->relationship.plastsons = (sciSons *) NULL;
      ppsubwin->callback = (char *)NULL;
      ppsubwin->callbacklen = 0;
      ppsubwin->callbackevent = 100;

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);          
	  return (sciPointObj *) NULL;
	}   
      if (sciInitGraphicMode (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	} 
      
      /* F.Leray 08.04.04 */
      if (sciInitFontContext (pobj) == -1)
	{
	  /* sciDelThisToItsParent (pobj, sciGetParent (pobj));*/
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);	  
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      dir= ppaxesmdl->logflags[0];
      ppsubwin->logflags[0] = dir;

      dir= ppaxesmdl->logflags[1]; 
      ppsubwin->logflags[1] = dir;

      dir= ppaxesmdl->logflags[2];
      ppsubwin->logflags[2] = dir;

      ppsubwin->axes.ticscolor  = ppaxesmdl->axes.ticscolor;
      ppsubwin->axes.subint[0]  = ppaxesmdl->axes.subint[0];   
      ppsubwin->axes.subint[1]  = ppaxesmdl->axes.subint[1]; 
      ppsubwin->axes.subint[2]  = ppaxesmdl->axes.subint[2];

      dir= ppaxesmdl->axes.xdir; 
      ppsubwin->axes.xdir = dir; 
      dir= ppaxesmdl->axes.ydir; 
      ppsubwin->axes.ydir = dir;
 
      ppsubwin->axes.rect  = ppaxesmdl->axes.rect;
      for (i=0 ; i<7 ; i++)
	ppsubwin->axes.limits[i]  = ppaxesmdl->axes.limits[i] ;
 
      for (i=0 ; i<3 ; i++)
	ppsubwin->grid[i]  = ppaxesmdl->grid[i] ;
      ppsubwin->alpha  = ppaxesmdl->alpha;
      ppsubwin->theta  = ppaxesmdl->theta;
      ppsubwin->alpha_kp  = ppaxesmdl->alpha_kp;
      ppsubwin->theta_kp  = ppaxesmdl->theta_kp;
      ppsubwin->is3d  = ppaxesmdl->is3d;
       
      for (i=0 ; i<4 ; i++)
        {  
	  ppsubwin->axes.xlim[i]= ppaxesmdl->axes.xlim[i]; 
	  ppsubwin->axes.ylim[i]= ppaxesmdl->axes.ylim[i]; 
	}

      /* F.Leray 22.09.04 */

      ppsubwin->axes.u_xlabels = (char **) NULL; /* init. ci-apres */
      ppsubwin->axes.u_ylabels = (char **) NULL; /* init. ci-apres */
      ppsubwin->axes.u_zlabels = (char **) NULL; /* init. ci-apres */
      ppsubwin->axes.u_xgrads  = (double *)NULL;
      ppsubwin->axes.u_ygrads  = (double *)NULL;
      ppsubwin->axes.u_zgrads  = (double *)NULL;
       

      (ppsubwin->axes).axes_visible[0] = ppaxesmdl->axes.axes_visible[0];
      (ppsubwin->axes).axes_visible[1] = ppaxesmdl->axes.axes_visible[1];
      (ppsubwin->axes).axes_visible[2] = ppaxesmdl->axes.axes_visible[2];
      (ppsubwin->axes).reverse[0] = ppaxesmdl->axes.reverse[0];
      (ppsubwin->axes).reverse[1] = ppaxesmdl->axes.reverse[1];
      (ppsubwin->axes).reverse[2] = ppaxesmdl->axes.reverse[2];
      ppsubwin->flagNax = ppaxesmdl->flagNax;

      /* do not forget the nbsubtics ! */
      (ppsubwin->axes).nbsubtics[0] = ppaxesmdl->axes.nbsubtics[0];
      (ppsubwin->axes).nbsubtics[1] = ppaxesmdl->axes.nbsubtics[1];
      (ppsubwin->axes).nbsubtics[2] = ppaxesmdl->axes.nbsubtics[2];
      
      (ppsubwin->axes).nxgrads = ppaxesmdl->axes.nxgrads;
      (ppsubwin->axes).nygrads = ppaxesmdl->axes.nygrads;
      (ppsubwin->axes).nzgrads = ppaxesmdl->axes.nzgrads;
      
      for(i=0;i<(ppsubwin->axes).nxgrads;i++) ppsubwin->axes.xgrads[i] = ppaxesmdl->axes.xgrads[i];
      for(i=0;i<(ppsubwin->axes).nygrads;i++) ppsubwin->axes.ygrads[i] = ppaxesmdl->axes.ygrads[i];
      for(i=0;i<(ppsubwin->axes).nzgrads;i++) ppsubwin->axes.zgrads[i] = ppaxesmdl->axes.zgrads[i];
     
      (ppsubwin->axes).u_nxgrads = ppaxesmdl->axes.u_nxgrads;
      (ppsubwin->axes).u_nygrads = ppaxesmdl->axes.u_nygrads;
      (ppsubwin->axes).u_nzgrads = ppaxesmdl->axes.u_nzgrads;
      (ppsubwin->axes).u_xgrads  = AllocUserGrads(ppsubwin->axes.u_xgrads,ppaxesmdl->axes.u_nxgrads);
      CopyUserGrads(ppaxesmdl->axes.u_xgrads,
		    ppsubwin->axes.u_xgrads,
		    ppsubwin->axes.u_nxgrads);
      (ppsubwin->axes).u_ygrads  = AllocUserGrads(ppsubwin->axes.u_ygrads,ppaxesmdl->axes.u_nygrads);
      CopyUserGrads(ppaxesmdl->axes.u_ygrads,
		    ppsubwin->axes.u_ygrads,
		    ppsubwin->axes.u_nygrads);
      (ppsubwin->axes).u_zgrads  = AllocUserGrads(ppsubwin->axes.u_zgrads,ppaxesmdl->axes.u_nzgrads);
      CopyUserGrads(ppaxesmdl->axes.u_zgrads,
		    ppsubwin->axes.u_zgrads,
		    ppsubwin->axes.u_nzgrads);
      (ppsubwin->axes).u_xlabels = AllocAndSetUserLabelsFromMdl(ppsubwin->axes.u_xlabels,
								ppaxesmdl->axes.u_xlabels,
								ppsubwin->axes.u_nxgrads);

      (ppsubwin->axes).u_ylabels = AllocAndSetUserLabelsFromMdl(ppsubwin->axes.u_ylabels,
								ppaxesmdl->axes.u_ylabels,
								ppsubwin->axes.u_nygrads);
      
      (ppsubwin->axes).u_zlabels = AllocAndSetUserLabelsFromMdl(ppsubwin->axes.u_zlabels,
								ppaxesmdl->axes.u_zlabels,
								ppsubwin->axes.u_nzgrads);

      (ppsubwin->axes).auto_ticks[0] = ppaxesmdl->axes.auto_ticks[0];
      (ppsubwin->axes).auto_ticks[1] = ppaxesmdl->axes.auto_ticks[1];
      (ppsubwin->axes).auto_ticks[2] = ppaxesmdl->axes.auto_ticks[2];
      /* end 22.09.04 */

      ppsubwin->axes.zlim[0]= ppaxesmdl->axes.zlim[0];
      ppsubwin->axes.zlim[1]= ppaxesmdl->axes.zlim[1];
      ppsubwin->axes.flag[0]= ppaxesmdl->axes.flag[0];
      ppsubwin->axes.flag[1]= ppaxesmdl->axes.flag[1];
      ppsubwin->axes.flag[2]= ppaxesmdl->axes.flag[2];
      ppsubwin->project[0]= ppaxesmdl->project[0];
      ppsubwin->project[1]= ppaxesmdl->project[1];
      ppsubwin->project[2]= ppaxesmdl->project[2];
      ppsubwin->hiddencolor= ppaxesmdl->hiddencolor;
      ppsubwin->hiddenstate= ppaxesmdl->hiddenstate;
      ppsubwin->isoview= ppaxesmdl->isoview;
      ppsubwin->facetmerge = ppaxesmdl->facetmerge; 
      ppsubwin->WRect[0]   = ppaxesmdl->WRect[0];
      ppsubwin->WRect[1]   = ppaxesmdl->WRect[1];
      ppsubwin->WRect[2]   = ppaxesmdl->WRect[2];
      ppsubwin->WRect[3]   = ppaxesmdl->WRect[3];

      ppsubwin->ARect[0]   = ppaxesmdl->ARect[0];
      ppsubwin->ARect[1]   = ppaxesmdl->ARect[1];
      ppsubwin->ARect[2]   = ppaxesmdl->ARect[2];
      ppsubwin->ARect[3]   = ppaxesmdl->ARect[3];

      ppsubwin->FRect[0]   = ppaxesmdl->FRect[0];
      ppsubwin->FRect[1]   = ppaxesmdl->FRect[1] ;
      ppsubwin->FRect[2]   = ppaxesmdl->FRect[2];
      ppsubwin->FRect[3]   = ppaxesmdl->FRect[3];
      ppsubwin->FRect[4]   = ppaxesmdl->FRect[4] ;
      ppsubwin->FRect[5]   = ppaxesmdl->FRect[5];
     

      ppsubwin->isselected = ppaxesmdl->isselected;  
      ppsubwin->visible = ppaxesmdl->visible;
      /*       /\*       ppsubwin->drawlater = ppaxesmdl->drawlater; *\/ */
      /*       ppsubwin->drawlater = sciGetDrawLater(sciGetParentFigure(pobj)); */
      
      ppsubwin->clip_region_set = 0 ;
      sciSetIsClipping( pobj, sciGetIsClipping(paxesmdl) ) ;
      sciSetClipping(   pobj, sciGetClipping(  paxesmdl) ) ;
      /*ppsubwin->isclip = ppaxesmdl->isclip;
        ppsubwin->clip_region_set = ppaxesmdl->clip_region_set ;*/
            
      ppsubwin->cube_scaling = ppaxesmdl->cube_scaling;
      
      ppsubwin->SRect[0]  =  ppaxesmdl->SRect[0];
      ppsubwin->SRect[1]  =  ppaxesmdl->SRect[1];
      ppsubwin->SRect[2]  =  ppaxesmdl->SRect[2];
      ppsubwin->SRect[3]  =  ppaxesmdl->SRect[3];
      ppsubwin->SRect[4]  =  ppaxesmdl->SRect[4];
      ppsubwin->SRect[5]  =  ppaxesmdl->SRect[5];
      
      ppsubwin->tight_limits = ppaxesmdl->tight_limits;
      ppsubwin->FirstPlot = ppaxesmdl->FirstPlot;
      ppsubwin->with_leg =  ppaxesmdl->with_leg;
      
      if (sciSetSelectedSubWin(pobj) != 1) 
	return (sciPointObj *)NULL; 
      
      /* Construction des labels: x,y,z et Title */

      if ((ppsubwin->mon_title =  ConstructLabel (pobj, "",1)) == NULL){
	sciDelThisToItsParent (pobj, sciGetParent (pobj)); /* pobj type = scisubwindow*/
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      
      sciSetText(ppsubwin->mon_title, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_title)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_title)->text.textlen);
      
      /*------------------------------------*/
      if ((ppsubwin->mon_x_label =  ConstructLabel (pobj, "",2)) == NULL){
	DestroyLabel(ppsubwin->mon_title);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      
      sciSetText(ppsubwin->mon_x_label, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)->text.textlen);

      /*------------------------------------*/
      if ((ppsubwin->mon_y_label =  ConstructLabel (pobj, "",3)) == NULL){
	DestroyLabel(ppsubwin->mon_title);
	DestroyLabel(ppsubwin->mon_x_label);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
  
      sciSetText(ppsubwin->mon_y_label, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_y_label)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_y_label)->text.textlen);

      /*------------------------------------*/
      if ((ppsubwin->mon_z_label =  ConstructLabel (pobj, "",4)) == NULL){
	DestroyLabel(ppsubwin->mon_title);
	DestroyLabel(ppsubwin->mon_x_label);
	DestroyLabel(ppsubwin->mon_y_label);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }

      sciSetText(ppsubwin->mon_z_label, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_z_label)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_z_label)->text.textlen);
      
      /* labels auto_position modes */
      pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_position = 
	pLABEL_FEATURE(ppaxesmdl->mon_x_label)->auto_position;

      pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_position = 
	pLABEL_FEATURE(ppaxesmdl->mon_y_label)->auto_position;

      pLABEL_FEATURE(ppsubwin->mon_z_label)->auto_position = 
	pLABEL_FEATURE(ppaxesmdl->mon_z_label)->auto_position;

      pLABEL_FEATURE(ppsubwin->mon_title)->auto_position = 
	pLABEL_FEATURE(ppaxesmdl->mon_title)->auto_position;
 
      /* labels auto_rotation modes */
      pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_rotation = 
	pLABEL_FEATURE(ppaxesmdl->mon_x_label)->auto_rotation;

      pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_rotation = 
	pLABEL_FEATURE(ppaxesmdl->mon_y_label)->auto_rotation;

      pLABEL_FEATURE(ppsubwin->mon_z_label)->auto_rotation = 
	pLABEL_FEATURE(ppaxesmdl->mon_z_label)->auto_rotation;

      pLABEL_FEATURE(ppsubwin->mon_title)->auto_rotation = 
	pLABEL_FEATURE(ppaxesmdl->mon_title)->auto_rotation;
      
      ppsubwin->pPopMenu = (sciPointObj *)NULL;/* initialisation of popup menu*/
      ppsubwin->surfcounter = 0;
      return (sciPointObj *)pobj;
      
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
  
  return (sciPointObj *) NULL;
}



/**ConstructScrollV
 * @memo This function creates the scroll bar erticall
 */
sciPointObj *
ConstructScrollV (sciPointObj * pparentfigure)
{
  sciPointObj *pobjsbv = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      /* definition of scrollbars with original code */
      /* definition of the vertical scroll bar in the new struct */
      if ((pobjsbv = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobjsbv, SCI_SBV);
      if ((pobjsbv->pfeatures = MALLOC ((sizeof (sciScrollBarVert)))) == NULL)
	{
	  FREE(pobjsbv);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobjsbv) == -1)
	{
	  FREE(pobjsbv->pfeatures);
	  FREE(pobjsbv);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobjsbv, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobjsbv, pparentfigure)))
	{
	  sciDelHandle (pobjsbv);
	  FREE(pobjsbv->pfeatures);
	  FREE(pobjsbv);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobjsbv, (sciPointObj *) NULL);
      pSBH_FEATURE (pobjsbv)->relationship.psons = (sciSons *) NULL;
      pSBH_FEATURE (pobjsbv)->relationship.plastsons = (sciSons *) NULL;
      return pobjsbv;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructScrollH
 * @memo This function creates horizontal scroll bar 
 */
sciPointObj *
ConstructScrollH (sciPointObj * pparentfigure)
{
  sciPointObj *pobjsbh = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      /* definition of scrollbars with original code */
      /* definition of the horizontal scroll bar in the new struct */
      if ((pobjsbh = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobjsbh, SCI_SBH);
      if ((pobjsbh->pfeatures = MALLOC ((sizeof (sciScrollBarHorz)))) == NULL)
	{
	  FREE(pobjsbh);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobjsbh) == -1)
	{
	  FREE(pobjsbh->pfeatures);
	  FREE(pobjsbh);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobjsbh, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobjsbh, pparentfigure)))
	{
	  sciDelHandle (pobjsbh);
	  FREE(pobjsbh->pfeatures);
	  FREE(pobjsbh);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobjsbh, (sciPointObj *) NULL);
      pSBH_FEATURE (pobjsbh)->relationship.psons = (sciSons *) NULL;
      pSBH_FEATURE (pobjsbh)->relationship.plastsons = (sciSons *) NULL;
      return pobjsbh;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructText
 * @memo This function creates the parents window (manager) and the elementaries structures
 * @param  sciPointObj *pparentsubwin :
 * @param  char text[] : intial text string.
 * @param  int n : the number of element in text
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructText (sciPointObj * pparentsubwin, char text[], int n, double x,
	       double y, double *wh, int fill, int *foreground, int *background, 
	       BOOL isboxed, BOOL isline, BOOL isfilled)
{
  sciPointObj * pobj   = (sciPointObj *) NULL;
  sciText     * ppText ; 

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN) 
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
      {
	return (sciPointObj *) NULL;
      }

      sciSetEntityType (pobj, SCI_TEXT);
      
      if ((pobj->pfeatures = MALLOC ((sizeof (sciText)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      
      /* get the feature */
      ppText = pTEXT_FEATURE( pobj ) ;

      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      sciSetCurrentSon (pobj, (sciPointObj *) NULL);

      ppText->user_data = (int *) NULL;
      ppText->size_of_user_data = 0;
      ppText->relationship.psons = (sciSons *) NULL;
      ppText->relationship.plastsons = (sciSons *) NULL;

      ppText->callback = (char *)NULL;
      ppText->callbacklen = 0;
      ppText->callbackevent = 100;
      ppText->visible = sciGetVisibility(sciGetParentSubwin (pobj));
      
      ppText->clip_region_set = 0;
      /*ppText->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      sciSetIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj) ) ) ;
      sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
      
      /*       pTEXT_FEATURE (pobj)->clip_region = (double *) NULL; */

      if ((ppText->ptextstring = CALLOC (n+1, sizeof (char))) ==
	  NULL)
      {
        sciprint("No more place to allocates text string, try a shorter string");
        sciDelThisToItsParent (pobj, sciGetParent (pobj));
        sciDelHandle (pobj);
        FREE(pobj->pfeatures);	  
        FREE(pobj);
        return (sciPointObj *) NULL;
      }
      
      /* initialize text */
      strncpy (ppText->ptextstring, text, n);
      ppText->textlen = n;

      /* initialize position */
      ppText->x = x;
      ppText->y = y;
      ppText->z = 0.0; /**DJ.Abdemouche 2003**/
      
      
      if (wh != (double *)NULL){
	ppText->wh[0] = wh[0];
	ppText->wh[1] = wh[1];
      }
      else {
	ppText->wh[0] = 0.0;
	ppText->wh[1] = 0.0;
      }

      if (sciInitFontContext (pobj) == -1)
	{
	  FREE(pTEXT_FEATURE (pobj)->ptextstring);	  
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);	  
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      
      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppText);
	  FREE(pobj);
	  sciprint("pas de context");
	  return (sciPointObj *) NULL;
	}
      
      ppText->fill=fill; /* to distinguish between xstring and xstringb */

      sciSetIsBoxed(pobj,isboxed);
      sciSetIsLine(pobj,isline);
      sciSetIsFilled(pobj,isfilled);
      
      if(foreground != NULL)
	sciSetForeground(pobj,(*foreground));
      
      if(background != NULL)
	sciSetBackground(pobj,(*background));
      
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructTitle
 * @memo This function creates Title structure
 * @param  sciPointObj *pparentsubwin
 * @param  char text[] : intial text string.
 * @param  int n the number of element in text
 * @param  sciTitlePlace place : the title's place  (SCI_TITLE_IN_TOP, SCI_TITLE_IN_BOTTOM)
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructTitle (sciPointObj * pparentsubwin, char text[], int type)
{
  sciPointObj * pobj    = (sciPointObj *) NULL;
  sciTitle    * ppTitle ;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_TITLE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciTitle)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      
      ppTitle = pTITLE_FEATURE(pobj) ;

      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(ppTitle);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(ppTitle);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      
      ppTitle->text.relationship.psons = (sciSons *) NULL;
      ppTitle->text.relationship.plastsons = (sciSons *) NULL;

      ppTitle->text.callback = (char *)NULL;
      ppTitle->text.callbacklen = 0; 
      ppTitle->visible = sciGetVisibility(sciGetParentSubwin(pobj));
      ppTitle->text.isboxed = FALSE ;
      ppTitle->text.isline  = TRUE  ;
     
      if ((ppTitle->text.ptextstring =CALLOC (strlen(text)+1, sizeof (char))) == NULL)
	{
	  sciprint("No more place to allocates text string, try a shorter string");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppTitle);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* on copie le texte du titre dans le champs specifique de l'objet */
      strncpy (ppTitle->text.ptextstring, text, strlen(text));
      ppTitle->text.textlen = strlen(text);
      ppTitle->ptype = type;

      ppTitle->text.fontcontext.textorientation = 0;

      ppTitle->titleplace = SCI_TITLE_IN_TOP;
      ppTitle->isselected = TRUE;
      if (sciInitFontContext (pobj) == -1)
	{
	  FREE(ppTitle->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppTitle);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return (sciPointObj *) pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**constructLegend
 * @memo This function creates  Legend structure
 */
sciPointObj *
ConstructLegend (sciPointObj * pparentsubwin, char text[], int n, int nblegends, int *pstyle
		 , sciPointObj **pptabofpointobj)
{
  sciPointObj * pobj = (sciPointObj *) NULL;
  sciLegend   * ppLegend ;

  /*
   * verifier qu'il n'y a pas d'objet existant !!!! 
   * si oui alors le detruire puis le reconstruire.
   * car il ne peut y avoir qu'une legende
   */
  sciSons *psonstmp;
  int i=0;

  psonstmp = sciGetSons (pparentsubwin);
  /* init */
  if (psonstmp != (sciSons *) NULL)	/* on peut commencer sur le next */
    /* tant que le fils n'est pas une legende */
    while ((psonstmp->pnext != (sciSons *) NULL)
	   && sciGetEntityType (psonstmp->pointobj) != SCI_LEGEND)
      psonstmp = psonstmp->pnext;

  if (sciGetEntityType (psonstmp->pointobj) == SCI_LEGEND)
    DestroyLegend (psonstmp->pointobj);

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_LEGEND);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciLegend)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* get the pointer on the features */
      ppLegend = pLEGEND_FEATURE( pobj );

      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(ppLegend);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(ppLegend);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      ppLegend->user_data = (int *) NULL;
      ppLegend->size_of_user_data = 0;
      ppLegend->text.relationship.psons = (sciSons *) NULL;
      ppLegend->text.relationship.plastsons = (sciSons *) NULL;
      ppLegend->text.callback = (char *)NULL;
      ppLegend->text.callbacklen = 0;
      ppLegend->text.callbackevent = 100;
      ppLegend->text.isboxed = FALSE ;
      ppLegend->text.isline  = TRUE ;
      
      ppLegend->visible = sciGetVisibility(sciGetParentSubwin(pobj)); 

      /* Allocation de la structure sciText */
      if ((ppLegend->text.ptextstring = CALLOC (n+1, sizeof (char))) == NULL)
	{
	  sciprint("\nNo more place to allocates text string, try a shorter string\n");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppLegend);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* on copie le texte du titre dans le champs specifique de l'objet */
      strncpy (ppLegend->text.ptextstring, text, n);
      ppLegend->nblegends = nblegends;

      if ((ppLegend->pptabofpointobj = 
	   MALLOC(nblegends*sizeof(sciPointObj*))) == NULL)
	{
	  sciprint("No more memory for legend\n");
	  FREE(ppLegend->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppLegend);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      if ((ppLegend->pstyle = MALLOC(nblegends*sizeof(int))) != NULL)
	{
	  for (i=0; i < nblegends; i++)
	    {
	      ppLegend->pstyle[i] = pstyle[i];
	      ppLegend->pptabofpointobj[i] = pptabofpointobj[i];
	    }
	}
      else
	{		  
	  sciprint("\nNo more place to allocates style\n");
	  FREE(ppLegend->pptabofpointobj);
	  FREE(ppLegend->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppLegend);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}



      ppLegend->text.textlen = n;
      ppLegend->text.fontcontext.textorientation = 0;
      ppLegend->pos.x = 0;
      ppLegend->pos.y = 0;
      ppLegend->width = 0;
      ppLegend->height = 0;
      ppLegend->place = SCI_LEGEND_IN_INSIDE;
      ppLegend->isselected = TRUE;
      ppLegend->issurround = FALSE;

      if (sciInitGraphicContext (pobj) == -1) /* NEW :  used to draw the line and marks of the curve F.Leray 21.01.05 */
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);          
	  return (sciPointObj *) NULL;
	} 
      
      if (sciInitFontContext (pobj) == -1)
	{
	  sciprint("Problem with sciInitFontContext\n");
	  FREE(ppLegend->pptabofpointobj);
	  FREE(ppLegend->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppLegend);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructPolyline
 * @memo This function creates  Polyline 2d structure
 */
sciPointObj *
ConstructPolyline (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
		   int closed, int n1, int n2,int plot, int *foreground, int *background,
		   int *mark_style, int *mark_foreground, int *mark_background,
		   BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciPolyline *ppoly = (sciPolyline *) NULL;
  /* Adding F.Leray */
  /* sciPointObj *psubwin = ( sciPointObj *) NULL;*/
  int i = 0;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_POLYLINE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciPolyline)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pPOLYLINE_FEATURE (pobj)->x_shift = (double *) NULL;
      pPOLYLINE_FEATURE (pobj)->y_shift = (double *) NULL;
      pPOLYLINE_FEATURE (pobj)->z_shift = (double *) NULL;
      pPOLYLINE_FEATURE (pobj)->bar_width = 0.;
      pPOLYLINE_FEATURE (pobj)->user_data = (int *) NULL;
      pPOLYLINE_FEATURE (pobj)->size_of_user_data = 0;
      pPOLYLINE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pPOLYLINE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pPOLYLINE_FEATURE (pobj)->callback = (char *)NULL;
      pPOLYLINE_FEATURE (pobj)->callbacklen = 0; 
      pPOLYLINE_FEATURE (pobj)->callbackevent = 100; 
      pPOLYLINE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      pPOLYLINE_FEATURE (pobj)->clip_region_set = 0;
      /*pPOLYLINE_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      sciSetIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)) ) ;
      sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
      
      
      pPOLYLINE_FEATURE (pobj)->arsize_factor = 1;

      /*       pPOLYLINE_FEATURE (pobj)->clip_region = (double *) NULL; */

      pPOLYLINE_FEATURE (pobj)->isselected = TRUE;
      ppoly = pPOLYLINE_FEATURE (pobj);
      if ((ppoly->pvector = MALLOC (n1 * sizeof (POINT2D))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* pour le moment je garde les vecteurs separes, et non en POINT2D */
      if ((ppoly->pvx = MALLOC (n1 * sizeof (double))) == NULL)
	{
	  FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((ppoly->pvy = MALLOC (n1 * sizeof (double))) == NULL)
	{
	  FREE(pPOLYLINE_FEATURE (pobj)->pvx);
	  FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
 	}

      for (i = 0; i < n1; i++)
	{
	  ppoly->pvector[i].x = pvecx[i];
	  ppoly->pvector[i].y = pvecy[i];
	  ppoly->pvx[i]       = pvecx[i];
	  ppoly->pvy[i]       = pvecy[i];
	  
	}

      /**DJ.Abdemouche 2003**/
      if (pvecz == (double *) NULL)
	{
	  pPOLYLINE_FEATURE (pobj)->pvz=NULL;
	}
      else
	{
	  if ((ppoly->pvz = MALLOC (n1 * sizeof (double))) == NULL)
	    {
	      FREE(pPOLYLINE_FEATURE (pobj)->pvx);
	      FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	      FREE(pPOLYLINE_FEATURE (pobj)->pvy);
	      sciDelThisToItsParent (pobj, sciGetParent (pobj));
	      sciDelHandle (pobj);
	      FREE(pPOLYLINE_FEATURE(pobj));
	      FREE(pobj);
	      return (sciPointObj *) NULL;
	    } 
	  for (i = 0; i < n1; i++)
	    ppoly->pvz[i] = pvecz[i];
	}

      ppoly->n1 = n1;	  /* memorisation du nombre de points */
      ppoly->n2 = n2;	  /* memorisation du nombre des courbes */
      ppoly->closed = (closed > 0) ? 1 : 0;
      ppoly->plot = plot;
 
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pPOLYLINE_FEATURE (pobj)->pvy);
	  FREE(pPOLYLINE_FEATURE (pobj)->pvx);
	  FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      /* colors and marks setting */
      sciSetIsMark(pobj,ismark);
      sciSetIsLine(pobj,isline);
      sciSetIsFilled(pobj,isfilled);
      /*       sciSetIsInterpShaded(pobj,isinterpshaded); */
      
      ppoly->isinterpshaded = isinterpshaded; /* set the isinterpshaded mode */
      
      if(foreground != NULL)
	sciSetForeground(pobj,(*foreground));
      
      ppoly->scvector = (int *) NULL;
      
      ppoly->dim_icv = n1;
      
      if(background != NULL){
	if(isinterpshaded == TRUE){ /* 3 or 4 values to store */
	  
	  sciSetInterpVector(pobj,n1,background);
	}
	else
	  sciSetBackground(pobj,(*background));
      }
      
      if(mark_style != NULL)
	sciSetMarkStyle(pobj,(*mark_style));
      
      if(mark_foreground != NULL)
	sciSetMarkForeground(pobj,(*mark_foreground));
      
      if(mark_background != NULL)
	sciSetMarkBackground(pobj,(*mark_background));
      
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructArc
 * @memo This function creates an Arc structure
 */
sciPointObj *
ConstructArc (sciPointObj * pparentsubwin, double x, double y,
	      double height, double width, double alphabegin, double alphaend, 
	      int *foreground, int *background, BOOL isfilled, BOOL isline)
{
  sciPointObj * pobj  = (sciPointObj *) NULL;
  sciArc      * ppArc = NULL ;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_ARC);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciArc)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      /* get the pointer to features */
      ppArc = pobj->pfeatures ;

      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(ppArc);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(ppArc);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      ppArc->user_data = (int *) NULL;
      ppArc->size_of_user_data = 0;
      ppArc->relationship.psons = (sciSons *) NULL;
      ppArc->relationship.plastsons = (sciSons *) NULL;

      ppArc->callback = (char *)NULL;
      ppArc->callbacklen = 0;
      ppArc->callbackevent = 100;
    
      ppArc->x = x;
      ppArc->y = y;
      ppArc->z = 0;
      ppArc->height = height;
      ppArc->width = width;
      ppArc->alphabegin = alphabegin;
      ppArc->alphaend = alphaend;
      ppArc->isselected = TRUE; 
      ppArc->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      ppArc->clip_region_set = 0;
      /*ppArc->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      sciSetIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)) ) ;
      sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
      /*      pARC_FEATURE (pobj)->clip_region = (double *) NULL; */


      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppArc);
	  FREE(pobj);
	  sciprint("pas de context");
	  return (sciPointObj *) NULL;
	}

      sciSetIsFilled(pobj,isfilled);
      /* should be put after graphicContext initialization */
      sciSetIsLine(pobj,isline);
      
      if(foreground != NULL)
      {
	sciSetForeground(pobj,(*foreground));
      }
      
      if(background != NULL)
      {
	sciSetBackground(pobj,(*background));
      }
      
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructRectangle
 * @memo This function creates Rectangle structure and only this to destroy all sons use DelGraphicsSon
 */
sciPointObj *
ConstructRectangle (sciPointObj * pparentsubwin, double x, double y,
		    double height, double width, double horzcurvature,
		    double vertcurvature,  int *foreground, int *background,
		    int isfilled, int isline, int str, BOOL flagstring)
{
  sciPointObj *pobj = (sciPointObj *) NULL; 
 
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_RECTANGLE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciRectangle)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pRECTANGLE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pRECTANGLE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pRECTANGLE_FEATURE (pobj)->user_data = (int *) NULL;
      pRECTANGLE_FEATURE (pobj)->size_of_user_data = 0;
      pRECTANGLE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pRECTANGLE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pRECTANGLE_FEATURE (pobj)->callback = (char *)NULL;
      pRECTANGLE_FEATURE (pobj)->callbacklen = 0;
      pRECTANGLE_FEATURE (pobj)->callbackevent = 100;


      pRECTANGLE_FEATURE (pobj)->flagstring = flagstring;
      pRECTANGLE_FEATURE (pobj)->x = x;
      pRECTANGLE_FEATURE (pobj)->y = y;
      pRECTANGLE_FEATURE (pobj)->z = 0; 
      pRECTANGLE_FEATURE (pobj)->height = height;
      pRECTANGLE_FEATURE (pobj)->width = width;
      pRECTANGLE_FEATURE (pobj)->str = str;
      pRECTANGLE_FEATURE (pobj)->strheight = 0;
      pRECTANGLE_FEATURE (pobj)->strwidth = 0;
      pRECTANGLE_FEATURE (pobj)->horzcurvature = horzcurvature;
      pRECTANGLE_FEATURE (pobj)->vertcurvature = vertcurvature;
      pRECTANGLE_FEATURE (pobj)->isselected = TRUE;
      pRECTANGLE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      pRECTANGLE_FEATURE (pobj)->clip_region_set = 0;
      /*pRECTANGLE_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      sciSetIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)) ) ;
      sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
      
      /*     pRECTANGLE_FEATURE (pobj)->clip_region = (double *) NULL; */

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pRECTANGLE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      sciSetIsLine(pobj,isline);
      sciSetIsFilled(pobj,isfilled);
      
      if(foreground != NULL)
	sciSetForeground(pobj,(*foreground));
      
      if(background != NULL)
	sciSetBackground(pobj,(*background));
      
      /*       if (pRECTANGLE_FEATURE (pobj)->fillcolor < 0) */
      /* 	sciSetForeground (pobj,-(pRECTANGLE_FEATURE (pobj)->fillcolor)); */
      /*       else	 */
      /* 	if (pRECTANGLE_FEATURE (pobj)->fillcolor > 0) */
      /* 	  sciSetForeground (pobj,pRECTANGLE_FEATURE (pobj)->fillcolor); */
      /* 	else  */
      /* 	  sciSetForeground (pobj,sciGetForeground (sciGetParent (pobj)) ); */
      
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructSurface
 * @memo This function creates Surface Structure
 */
sciPointObj *
ConstructSurface (sciPointObj * pparentsubwin, sciTypeOf3D typeof3d, 
		  double * pvecx, double * pvecy, double * pvecz,double *zcol, 
		  integer izcol, integer dimzx, integer dimzy,  
		  integer *flag, double *ebox,integer flagcolor, 
		  integer *isfac, integer *m1, integer *n1, integer *m2, 
		  integer *n2, integer *m3, integer *n3, integer *m3n, integer *n3n)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  /*debug F.Leray*/
  sciSurface *psurf;

  int i=0, j=0;
  int nx,ny,nz,nc,izc=izcol;

  if (typeof3d == SCI_PLOT3D) {
    nx=dimzx;
    ny=dimzy;
    nz=dimzx*dimzy;
    if (flagcolor == 2)
      nc=nz; /* one color per facet */    /* nc = dimzx * dimzy */
    else if (flagcolor == 3)
      nc=nz*4; /*one color per edge */    /* nc = 4* dimzx * dimzy ?????? */ /* 3 or 4 vectices are needed: 
										I think we take 4 to have enough allocated memory*/ 
    /* made by Djalel : comes from the genfac3d case*/
    else 
      nc=0;
  }
  /* DJ.A 2003 */
  else { /* case SCI_FAC3D */
    nx=dimzx*dimzy;
    ny=dimzx*dimzy;
    nz=dimzx*dimzy;
    if (flagcolor == 2)
      nc=dimzy; /* one color per facet */ /* nc = dimzy */
    else if (flagcolor == 3)
      nc=nz; /*one color per edge */      /* nc = dimzx * dimzy */
    else 
      nc=0;
  }


  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_SURFACE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciSurface)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSURFACE_FEATURE (pobj)->user_data = (int *) NULL;
      pSURFACE_FEATURE (pobj)->size_of_user_data = 0;
      pSURFACE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSURFACE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pSURFACE_FEATURE (pobj)->callback = (char *)NULL;
      pSURFACE_FEATURE (pobj)->callbacklen = 0;
      pSURFACE_FEATURE (pobj)->callbackevent = 100; 
      pSURFACE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      /*debug F.Leray*/
      psurf = pSURFACE_FEATURE (pobj);
  
      /*F.Leray 12.03.04 Adding here to know the length of arrays pvecx, pvecy and pvecz*/
      psurf->nc = nc;
      psurf->nx = nx;
      psurf->ny = ny;
      psurf->nz = nz;
      psurf->isfac = *isfac;
      psurf->m1= *m1;
      psurf->m2= *m2;
      psurf->m3= *m3;
      psurf->n1= *n1;
      psurf->n2= *n2;
      psurf->n3= *n3;
      
      /*Adding F.Leray 19.03.04*/
      psurf->m3n= *m3n;
      psurf->n3n= *n3n;

      if (((psurf->pvecx = MALLOC ((nx * sizeof (double)))) == NULL))
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(psurf);
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      else
	{
	  for (i = 0;i < nx; i++)
	    psurf->pvecx[i] = pvecx[i];
	}
      if (((psurf->pvecy = MALLOC ((ny * sizeof (double)))) == NULL))
	{
	  FREE(psurf->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(psurf);
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      else
	{
	  for (j = 0;j < ny; j++)
	    psurf->pvecy[j] = pvecy[j];
	}

      if (((psurf->pvecz = MALLOC ((nz * sizeof (double)))) == NULL))
	{
	  FREE(psurf->pvecy);
	  FREE(psurf->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(psurf);
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      else
	{
	  for (j = 0;j < nz; j++)
	    psurf->pvecz[j] = pvecz[j];
	}
      
      /*Storage of the input Color Matrix or Vector Data */ /* F.Leray 23.03.04*/
      psurf->inputCMoV = NULL;
      
      if((*m3n)*(*n3n) != 0){
	if (((psurf->inputCMoV = MALLOC (( (*m3n)*(*n3n) * sizeof (double)))) == NULL))
	  {
	    FREE(psurf->pvecy); psurf->pvecy = NULL;
	    FREE(psurf->pvecx); psurf->pvecx = NULL;
	    FREE(psurf->pvecz); psurf->pvecz = NULL;
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(psurf);
	    FREE(pobj); pobj = NULL;
	    return (sciPointObj *) NULL;
	  }
      }
      
      for (j = 0;j < (*m3n)*(*n3n); j++)  
	psurf->inputCMoV[j] = zcol[j];
      
      /* Init. zcol & zcolReal to NULL F.Leray 17.03.04*/
      psurf->zcol = NULL;
      psurf->color = NULL;
      
      /*
	if (izc !=0&&nc>0 ) {
	if (((psurf->zcol = MALLOC ((nc * sizeof (integer)))) == NULL))
	{
	FREE(psurf->pvecy); psurf->pvecy = NULL;
	FREE(psurf->pvecx); psurf->pvecx = NULL;
	FREE(psurf->pvecz); psurf->pvecz = NULL;
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(psurf);
	FREE(pobj); pobj = NULL;
	return (sciPointObj *) NULL;
	}
	else
	{
	if (izcol !=0)
	for (j = 0;j < nc; j++)  
	psurf->zcol[j]= zcol[j];  */ /* DJ.A 2003 */
      /*}
	} */
      
      /*-------Replaced by: --------*/

      if (izc !=0&&nc>0 ) { /* Allocation of good size depending on flagcolor for nc (see above)*/
	if (((psurf->zcol = MALLOC ((nc * sizeof (double)))) == NULL))
	  {
	    FREE(psurf->pvecy); psurf->pvecy = NULL;
	    FREE(psurf->pvecx); psurf->pvecx = NULL;
	    FREE(psurf->pvecz); psurf->pvecz = NULL;
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(psurf);
	    FREE(pobj); pobj = NULL;
	    return (sciPointObj *) NULL;
	  }
      }
      
      if(nc>0)
	{
	  /* case flagcolor == 2*/
	  if(flagcolor==2 && ( *m3n==1 || *n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	    {
	      /* We have just enough information to fill the psurf->zcol array*/
	      for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3 * n3 */
		psurf->zcol[j]= psurf->inputCMoV[j];  /* DJ.A 2003 */
	    }
	  else if(flagcolor==2 && !( *m3n==1 || *n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	    {
	      /* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*/
	      /* NO !! Let's do better; F.Leray 08.05.04 : */
	      /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
	      /* in our example: m3n=4 and n3n=400 */
	      for (j = 0;j < nc; j++)   /* nc value is dimzy*/
		{
		  double tmp = 0;
		  int ii=0;
		  for(ii=0;ii<(*m3n);ii++)
		    tmp = tmp +  psurf->inputCMoV[j*(*m3n) + ii];
		  tmp = tmp / (*m3n);
		  psurf->zcol[j]= tmp;
		}
	    }
	  /* case flagcolor == 3*/
	  else if(flagcolor==3 && ( *m3n==1 || *n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	    {
	      /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
	      /* We repeat the data:*/
	      for(i = 0; i< dimzy; i++){
		for (j = 0;j < dimzx; j++)  /* nc value is dimzx*dimzy == m3 * n3 */
		  psurf->zcol[dimzx*i+j]= psurf->inputCMoV[i];  /* DJ.A 2003 */
	      }
	    }
	  else if(flagcolor==3 && !( *m3n==1 || *n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	    {
	      /* We have just enough information to fill the psurf->zcol array*/
	      for (j = 0;j < nc; j++)   /* nc value is dimzy*/
		psurf->zcol[j]= psurf->inputCMoV[j];
	    }
	  /* Notice that the new case flagcolor == 4 is not available at the construction state */
	  /* It is a flat mode display (like flagcolor == 2 case) with a different computation  */
	  /* manner to simulate Matlab flat mode. It can be enabled by setting color_flag to 4. */

	}


      psurf->cdatamapping = 1; /* direct mode enabled by default */
      
      
      /* We need to rebuild ...->color matrix */
      if(psurf->cdatamapping == 0){ /* scaled */
	FREE(psurf->color);
	LinearScaling2Colormap(pobj);
      }
      else{
	
	FREE(psurf->color);
	
	if(nc>0){
	  if ((psurf->color = MALLOC (nc * sizeof (double))) == NULL)
	    return (sciPointObj *) NULL;
	}
	
	for(i=0;i<nc;i++)
	  psurf->color[i] = psurf->zcol[i];
	/* copy zcol that has just been freed and re-alloc + filled in */
      }

      /*-------END Replaced by: --------*/

      psurf->dimzx = dimzx; /* dimzx is completly equal to m3*/
      psurf->dimzy = dimzy; /* dimzx is completly equal to n3*/
      psurf->izcol = izc;
      psurf->pproj = NULL;	/* Les projections ne sont pas encore calculees */
      psurf->isselected = TRUE;

      psurf->flag[0] = flag[0]; /* F.Leray 16.04.04 HERE We store the flag=[mode (hidden part ), type (scaling), box (frame around the plot)] */
      psurf->flag[1] = flag[1];
      psurf->flag[2] = flag[2];

      /* DJ.A 2003 */
   
      psurf->ebox[0] = ebox[0];
      psurf->ebox[1] = ebox[1];
      psurf->ebox[2] = ebox[2];
      psurf->ebox[3] = ebox[3];
      psurf->ebox[4] = ebox[4];
      psurf->ebox[5] = ebox[5];
      psurf->flagcolor =flagcolor;
      psurf->typeof3d = typeof3d;
      psurf->hiddencolor = pSUBWIN_FEATURE(pparentsubwin)->hiddencolor;
      
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(psurf->pvecz);
	  FREE(psurf->pvecy);
	  FREE(psurf->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(psurf);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      pSUBWIN_FEATURE(pparentsubwin)->surfcounter++;
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



sciPointObj *
ConstructMerge (sciPointObj * pparentsubwin,int N,int *index_in_entity,long *from_entity)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
 
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL) return (sciPointObj *) NULL;
  
      sciSetEntityType (pobj, SCI_MERGE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciMerge)))) == NULL) {
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      if (sciAddNewHandle (pobj) == -1) {
	FREE(pMERGE_FEATURE (pobj));
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      if (!(sciAddThisToItsParent (pobj, pparentsubwin))){
	sciDelHandle (pobj);
	FREE(pMERGE_FEATURE (pobj));
	FREE(pobj);
	return (sciPointObj *) NULL;
      }

      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pMERGE_FEATURE (pobj)->user_data = (int *) NULL;
      pMERGE_FEATURE (pobj)->size_of_user_data = 0;
      pMERGE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pMERGE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
      pMERGE_FEATURE (pobj)->index_in_entity=index_in_entity;
      pMERGE_FEATURE (pobj)->from_entity=from_entity;
      pMERGE_FEATURE (pobj)->N=N;
      return pobj;
    }
  else
    return (sciPointObj *) NULL;
}



/********************** 14/05/2002 *****
 **ConstructGayplot
 * @memo This function creates Grayplot
 */
sciPointObj *
ConstructGrayplot (sciPointObj * pparentsubwin, double *pvecx, double *pvecy,
		   double *pvecz, int n1, int n2, int type)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciGrayplot *pgray = (sciGrayplot *) NULL;
  int i = 0,cmpt;
  
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_GRAYPLOT);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciGrayplot)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pGRAYPLOT_FEATURE (pobj)->user_data = (int *) NULL;
      pGRAYPLOT_FEATURE (pobj)->size_of_user_data = 0;
      pGRAYPLOT_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pGRAYPLOT_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pGRAYPLOT_FEATURE (pobj)->callback = (char *)NULL;
      pGRAYPLOT_FEATURE (pobj)->callbacklen = 0; 
      pGRAYPLOT_FEATURE (pobj)->callbackevent = 100;

      pGRAYPLOT_FEATURE (pobj)->isselected = TRUE; 
      pGRAYPLOT_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      pGRAYPLOT_FEATURE (pobj)->type = type;
      pGRAYPLOT_FEATURE (pobj)->pvecx = (double *)NULL;
      pGRAYPLOT_FEATURE (pobj)->pvecy = (double *)NULL;

      strncpy (pGRAYPLOT_FEATURE (pobj)->datamapping, "scaled", 6);
      pgray = pGRAYPLOT_FEATURE (pobj);
      
      if (pvecx && (pgray->pvecx = MALLOC (n1 * sizeof (double))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      cmpt = (type == 2)? 4:n2 ;
      if (type != 2)
	if (pvecy && (pgray->pvecy = MALLOC (cmpt * sizeof (double))) == NULL)
	  {
	    if (pvecx) FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pGRAYPLOT_FEATURE(pobj));
	    FREE(pobj);
	    return (sciPointObj *) NULL;
	  }
      if ((pgray->pvecz = MALLOC ((n1*n2) * sizeof (double))) == NULL){
	if (pvecx) FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	if (pvecy) FREE(pGRAYPLOT_FEATURE (pobj)->pvecy);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pGRAYPLOT_FEATURE(pobj));
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      if (pvecx) {
	for (i = 0; i < n1; i++) pgray->pvecx[i] = pvecx[i];
      } 
   

      if (pvecy) {
	if (type != 2)
	  for (i = 0; i < n2; i++) pgray->pvecy[i] = pvecy[i];
      }
    
      pgray->nx = n1;pgray->ny = n2;
      for (i = 0; i < (n1*n2); i++) pgray->pvecz[i] = pvecz[i];
	
      if (sciInitGraphicContext (pobj) == -1)
	{
	  if (pvecx) FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	  if (pvecy) FREE(pGRAYPLOT_FEATURE (pobj)->pvecy); 
	  FREE(pGRAYPLOT_FEATURE (pobj)->pvecz);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}





/**ConstructAxes
 * @memo This function creates Axes structure
 * @author Djalel ABDEMOUCHE
 * @see sciSetCurrentObj
 *
 */
sciPointObj *
ConstructAxes (sciPointObj * pparentsubwin, char dir, char tics, double *vx, 
	       int nx, double *vy, int ny,char **str, int subint, char *format,
	       int fontsize, int textcolor, int ticscolor, char logscale, int seg, int nb_tics_labels)  
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciAxes *paxes = (sciAxes *) NULL;
  int i;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_AXES);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciAxes)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pAXES_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pAXES_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pAXES_FEATURE (pobj)->user_data = (int *) NULL;
      pAXES_FEATURE (pobj)->size_of_user_data = 0;
      pAXES_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pAXES_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pAXES_FEATURE (pobj)->callback = (char *)NULL;
      pAXES_FEATURE (pobj)->callbacklen = 0;
      pAXES_FEATURE (pobj)->callbackevent = 100;
      pAXES_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      /*pAXES_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      pAXES_FEATURE (pobj)->clip_region_set = 0;
      /*pAXES_FEATURE (pobj)->isclip = -1;*/  /*F.Leray Change here: by default Axis are not clipped. 10.03.04 */
      sciSetIsClipping( pobj, -1 ) ;
      sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
      /*       pAXES_FEATURE (pobj)->clip_region = (double *) NULL; */
     
      pAXES_FEATURE (pobj)->dir =dir;
      pAXES_FEATURE (pobj)->tics =tics;
  
      paxes = pAXES_FEATURE (pobj);
      if ((paxes->vector = MALLOC (Max(nx,ny) * sizeof (POINT2D))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* pour le moment je garde les vecteurs separes, et non en POINT2D */
      if ((paxes->vx = MALLOC (nx * sizeof (double))) == NULL)
	{
	  FREE(pAXES_FEATURE (pobj)->vector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pAXES_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((paxes->vy = MALLOC (ny * sizeof (double))) == NULL)
	{
	  FREE(pAXES_FEATURE (pobj)->vector);
	  FREE(pAXES_FEATURE (pobj)->vx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pAXES_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      
      for (i = 0; i < nx; i++)
	{
	  paxes->vx[i]       = vx[i];	 
	}
      for (i = 0; i < ny; i++)
	{
	  paxes->vy[i]       = vy[i];
	}
     
     
      
      pAXES_FEATURE (pobj)->nx =nx; 
      
     
      pAXES_FEATURE (pobj)->ny =ny;
     
      pAXES_FEATURE (pobj)->nb_tics_labels = nb_tics_labels; /* F.Leray 29.04.05 */

      /* pAXES_FEATURE(pobj)->str = str;*/ /* Pb here, F.Leray : Weird init.: can not copy a string using '='*/
      if(str != (char **) NULL)
	{
	  if(pAXES_FEATURE (pobj)->nb_tics_labels == -1){
	    sciprint("Impossible case when buyilding axis\n");
	    return (sciPointObj *) NULL;
	  }
	  
	  if ((pAXES_FEATURE(pobj)->str= MALLOC (pAXES_FEATURE (pobj)->nb_tics_labels * sizeof (char*))) == NULL)
	    return (sciPointObj *) NULL;

	  for(i=0;i<pAXES_FEATURE (pobj)->nb_tics_labels;i++) 
	    {
	      if(str[i] != (char *) NULL)
		{
		  if((pAXES_FEATURE (pobj)->str[i] = MALLOC( (strlen(str[i])+1) * sizeof(char))) == NULL)
		    return (sciPointObj *) NULL;
		  else
		    strcpy(pAXES_FEATURE (pobj)->str[i],str[i]);
		}
	      else
		pAXES_FEATURE (pobj)->str[i] = (char *) NULL;
	    }
	}
      else
	{
	  pAXES_FEATURE (pobj)->str = (char **) NULL;
	}

      pAXES_FEATURE (pobj)->subint = subint;
      pAXES_FEATURE (pobj)->fontsize =fontsize; 
      pAXES_FEATURE (pobj)->textcolor =textcolor;
      pAXES_FEATURE (pobj)->ticscolor =ticscolor;
      pAXES_FEATURE (pobj)->seg =seg;    
      /*    pAXES_FEATURE (pobj)->format =format; */ /* Pb here, F.Leray : Weird init.: can not copy a string using '='*/
      pAXES_FEATURE (pobj)->logscale=logscale;
      if(format != (char *) NULL)
	{
	  if((pAXES_FEATURE (pobj)->format = MALLOC( (strlen(format)+1) * sizeof(char))) == NULL)
	    return (sciPointObj *) NULL;
	  else
	    strcpy(pAXES_FEATURE (pobj)->format,format);
	}
      else
	pAXES_FEATURE (pobj)->format = (char *) NULL;
	  
      if (sciInitGraphicContext (pobj) == -1)
	{
    	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
    	  sciDelHandle (pobj);
    	  FREE(pAXES_FEATURE (pobj));
    	  FREE(pobj);
    	  return (sciPointObj *) NULL;
	}


      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/********************** 21/05/2002 *****
 **ConstructFec
 * @memo This function creates Grayplot
 * @author Djalel.ABDEMOUCHE
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud, 
	      double *pfun, int Nnode, int Ntr, double *zminmax, integer *colminmax, 
	      integer *colout, BOOL with_mesh)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciFec *pfec = (sciFec *) NULL;
  int i = 0;
  
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_FEC);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciFec)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pFEC_FEATURE (pobj)->user_data = (int *) NULL;
      pFEC_FEATURE (pobj)->size_of_user_data = 0;
      pFEC_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pFEC_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pFEC_FEATURE (pobj)->callback = (char *)NULL;
      pFEC_FEATURE (pobj)->callbacklen = 0;
      pFEC_FEATURE (pobj)->callbackevent = 100;

      pFEC_FEATURE (pobj)->isselected = TRUE; 
      pFEC_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));
   
      pfec = pFEC_FEATURE (pobj);
      
      if ((pfec->pvecx = MALLOC (Nnode * sizeof (double))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->pvecy = MALLOC (Nnode * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->pnoeud = MALLOC ((5*Ntr) * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->pfun = MALLOC (Nnode * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy); 
	  FREE(pFEC_FEATURE (pobj)->pnoeud);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->zminmax = MALLOC (2 * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy);
	  FREE(pFEC_FEATURE (pobj)->pnoeud); 
	  FREE(pFEC_FEATURE (pobj)->pfun);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	} 
      if ((pfec->colminmax = MALLOC (2 * sizeof (integer))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy);
	  FREE(pFEC_FEATURE (pobj)->pnoeud); 
	  FREE(pFEC_FEATURE (pobj)->pfun); 
	  FREE(pFEC_FEATURE (pobj)->zminmax);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      if ((pfec->colout = MALLOC (2 * sizeof (integer))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy);
	  FREE(pFEC_FEATURE (pobj)->pnoeud); 
	  FREE(pFEC_FEATURE (pobj)->pfun); 
	  FREE(pFEC_FEATURE (pobj)->zminmax);
	  FREE(pFEC_FEATURE (pobj)->colminmax);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      for (i = 0; i < Nnode; i++)
	{
	  pfec->pvecx[i] = pvecx[i];
	  pfec->pvecy[i] = pvecy[i];
	  pfec->pfun[i] = pfun[i];
	}
      for (i = 0; i < (5*Ntr); i++)
	{
	  pfec->pnoeud[i] = pnoeud[i];
	}
      for (i = 0; i < 2; i++)
	{
	  pfec->zminmax[i] = zminmax[i];
	  pfec->colminmax[i] = colminmax[i];
	  pfec->colout[i] = colout[i];
	}
      pfec->with_mesh = with_mesh;
      pfec->Nnode = Nnode;	      
      pfec->Ntr = Ntr;		
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy); 
	  FREE(pFEC_FEATURE (pobj)->pnoeud);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructSegs
 * @memo This function creates Segments
 * @author Djalel.ABDEMOUCHE
 * @version 0.1
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructSegs (sciPointObj * pparentsubwin, integer type,double *vx, double *vy, 
               integer Nbr1,integer Nbr2, double *vfx, double *vfy, integer flag,
	       integer *style, double arsize, integer colored, double arfact, int typeofchamp) 
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciSegs *psegs = (sciSegs *) NULL;
  integer i;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_SEGS);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciSegs)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSEGS_FEATURE (pobj)->user_data = (int *) NULL;
      pSEGS_FEATURE (pobj)->size_of_user_data = 0;
      pSEGS_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSEGS_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pSEGS_FEATURE (pobj)->callback = (char *)NULL;
      pSEGS_FEATURE (pobj)->callbacklen = 0;
      pSEGS_FEATURE (pobj)->callbackevent = 100;
       
      pSEGS_FEATURE (pobj)->isselected = TRUE;
      pSEGS_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj)); 

      /* this must be done prior to the call of sciSetClipping to know */
      /* if the clip_state has been set */
      pSEGS_FEATURE (pobj)->clip_region_set = 0;
      /*pSEGS_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      sciSetIsClipping( pobj, sciGetIsClipping(sciGetParentSubwin(pobj) ));
      sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
      
      /*       pSEGS_FEATURE (pobj)->clip_region = (double *) NULL; */
   
      psegs = pSEGS_FEATURE (pobj); 
      psegs->ptype = type;

      /* psegs->arrowsize = 50.;  */ /* default value F.Leray 25.03.04*/
          
      if ((psegs->vx = MALLOC (Nbr1 * sizeof (double))) == NULL)
	{ 
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((psegs->vy = MALLOC (Nbr2 * sizeof (double))) == NULL)
	{ 
	  FREE(pSEGS_FEATURE (pobj)->vx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
     
      for (i = 0; i < Nbr1; i++)
	{
	  psegs->vx[i] = vx[i];
	} 
      for (i = 0; i < Nbr2; i++)
	{
	  psegs->vy[i] = vy[i];
	} 
      pSEGS_FEATURE (pobj)->vz=(double *) NULL; /**DJ.Abdemouche 2003**/
      psegs->ptype = type;

      /* F.Leray Test imprortant sur type ici*/
      if (type == 0) /* attention ici type = 0 donc...*/
	{
	  psegs->typeofchamp = -1; /* useless property in the case type == 0 */
	  psegs->arrowsize = arsize /** 100*/;       /* A revoir: F.Leray 06.04.04 */
	  if ((psegs->pstyle = MALLOC (Nbr1 * sizeof (integer))) == NULL)
	    {
	      FREE(pSEGS_FEATURE (pobj)->vx); 
	      FREE(pSEGS_FEATURE (pobj)->vy); 
	      sciDelThisToItsParent (pobj, sciGetParent (pobj));
	      sciDelHandle (pobj);
	      FREE(pSEGS_FEATURE(pobj));
	      FREE(pobj);
	      return (sciPointObj *) NULL;
	    }
	  if (flag == 1)
	    {
	      for (i = 0; i < Nbr1; i++)
		psegs->pstyle[i] = style[i];
	    }
	  else {
	    for (i = 0; i < Nbr1; i++)
	      psegs->pstyle[i] =  style[0];
	  }
	
	  psegs->iflag = flag; 
	  psegs->Nbr1 = Nbr1;
	}	
      else /* Warning here type = 1 so... building comes from champg */
	{ 
	  /* Rajout de psegs->arrowsize = arsize; F.Leray 18.02.04*/
	  psegs->arrowsize = arsize /* * 100 */;
	  psegs->Nbr1 = Nbr1;   
	  psegs->Nbr2 = Nbr2;	 
	  sciSetForeground(pobj,sciGetForeground(sciGetSelectedSubWin (sciGetCurrentFigure ()))); /* set sciGetForeground(psubwin) as the current foreground */
	  psegs->typeofchamp = typeofchamp; /* to know if it is a champ or champ1 */
	  psegs->parfact = arfact;
	  if ((psegs->vfx = MALLOC ((Nbr1*Nbr2) * sizeof (double))) == NULL)
	    {
	      FREE(pSEGS_FEATURE (pobj)->vx); 
	      FREE(pSEGS_FEATURE (pobj)->vy); 
	      sciDelThisToItsParent (pobj, sciGetParent (pobj));
	      sciDelHandle (pobj);
	      FREE(pSEGS_FEATURE(pobj));
	      FREE(pobj);
	      return (sciPointObj *) NULL;
	    }
	  if ((psegs->vfy = MALLOC ((Nbr1*Nbr2) * sizeof (double))) == NULL)
	    {
	      FREE(pSEGS_FEATURE (pobj)->vx); 
	      FREE(pSEGS_FEATURE (pobj)->vy);
	      FREE(pSEGS_FEATURE (pobj)->vfx); 
	      sciDelThisToItsParent (pobj, sciGetParent (pobj));
	      sciDelHandle (pobj);
	      FREE(pSEGS_FEATURE(pobj));
	      FREE(pobj);
	      return (sciPointObj *) NULL;
	    }  
	  
	  for (i = 0; i < (Nbr1*Nbr2); i++)
	    {
	      psegs->vfx[i] = vfx[i];
	      psegs->vfy[i] = vfy[i]; 
	    }
	  pSEGS_FEATURE (pobj)->vfz=(double *) NULL; /**DJ.Abdemouche 2003**/
	}	
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pSEGS_FEATURE (pobj)->vx);
	  FREE(pSEGS_FEATURE (pobj)->vy);  
          if (type ==0)
            {
	      FREE(pSEGS_FEATURE (pobj)->pstyle);
            }
          else
	    {
	      FREE(pSEGS_FEATURE (pobj)->vfx);  
	      FREE(pSEGS_FEATURE (pobj)->vfy);  
            }         
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**sciConstructCompound
 * @memo constructes an Compound of entities
 * @memo do only a association with a parent and a handle reservation !
 * @memo check for valid handle can be done using CheckForCompound
 */
sciPointObj *
ConstructCompound (long *handelsvalue, int number) /* Conflicting types with definition */
{
  /* sciSons *sons, *sonsnext; */
  sciPointObj * pobj       ;
  sciAgreg    * ppCompound ;
  int i;
  long xtmp;

 
  if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    return (sciPointObj *) NULL;

  sciSetEntityType (pobj, SCI_AGREG);
  if ((pobj->pfeatures = MALLOC ((sizeof (sciAgreg)))) == NULL)
    return (sciPointObj *) NULL;

  /* get the pointer on features */
  ppCompound = pAGREG_FEATURE (pobj) ;

  if (sciAddNewHandle (pobj) == -1)
    {
      sciprint("no handle to allocate\n");
      return (sciPointObj *) NULL;
    }

  /* the parent of the Compound will be the parent of the sons entities */
  if (!(sciAddThisToItsParent (pobj, (sciPointObj *)sciGetParent(
                                 sciGetPointerFromHandle((long) handelsvalue[0])))))
    return NULL;

  sciSetCurrentSon (pobj, (sciPointObj *) NULL);
  ppCompound->user_data = (int *) NULL;
  ppCompound->size_of_user_data = 0;
  ppCompound->relationship.psons = (sciSons *) NULL;

  ppCompound->callback = (char *)NULL;
  ppCompound->callbacklen = 0;
  ppCompound->visible = sciGetVisibility(sciGetParentSubwin(pobj));

  /* sonsnext = (sciSons *) NULL */

  /* initialisation with the first son */
  xtmp = (long) handelsvalue[0];
  for ( i = 0 ; i < number ; i++ )
    {
      /* if ((sons = MALLOC ((sizeof (sciSons)))) == NULL) */
/* 	return (sciPointObj *)NULL; */
      /* if (i == 0) */
/*         ppCompound->relationship.plastsons = (sciSons *)sons; */

      /* xtmp = handelsvalue[i]; */
      /* sons->pointobj = sciGetPointerFromHandle(xtmp); */

      /* Nous changeons le parent de l'entite qui devient alors l'Compound */
      /* if (sons->pointobj != NULL) */
/* 	{ */
/* 	  sciDelThisToItsParent (sons->pointobj, sciGetParent(temp)); */
/* 	  sciAddThisToItsParent (sons->pointobj, pobj); */
/* 	} */

      /* sons->pprev         = (sciSons *)NULL; */
/*       sons->pnext         = sonsnext; */

/*       if (sonsnext != NULL ) */
/*       { */
/*         sonsnext->pprev = sons; */
/*       } */

/*       sonsnext            = sons; */


      /* jb Silvy 10/01/06 */
      /* the handle id moved from the current parent (ex axis) to the compund */
      sciPointObj * movedObject ;
      xtmp = handelsvalue[i] ;
      movedObject = sciGetPointerFromHandle(xtmp) ;
      if ( movedObject != NULL )
      {
        sciDelThisToItsParent( movedObject, sciGetParent(movedObject) ) ;
        sciAddThisToItsParent( movedObject, pobj ) ;
      }
    }

 /*  ppCompound->relationship.psons = sons; */
  ppCompound->isselected = TRUE;

  return (sciPointObj *)pobj;
}

/**sciConstructCompoundSeq
 * @memo constructes an Compound of with the last n entities created in the current subwindow
 on entry subwin children list is
 null->s1->s2->...->sn->sn+1->...->null
 on exit it is
 null->A->sn+1->...->null
 with A an Compound whose children list is:
 null->s1->s2->...->sn->null
*/
sciPointObj *
ConstructCompoundSeq (int number) 
{
  sciSons *sons, *lastsons;
  sciPointObj *pobj;
  int i;

  sciPointObj *psubwin;
  sciSubWindow *ppsubwin;
  sciAgreg     *ppagr;
  
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  ppsubwin=pSUBWIN_FEATURE(psubwin);

  /* initialize the A Compound data structure */
  if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    return (sciPointObj *) NULL;

  sciSetEntityType (pobj, SCI_AGREG);
  if ((pobj->pfeatures = MALLOC ((sizeof (sciAgreg)))) == NULL)
    return (sciPointObj *) NULL;
  ppagr=pAGREG_FEATURE(pobj);

  if (sciAddNewHandle (pobj) == -1)
    {
      sciprint("no handle to allocate\n");
      FREE(pobj->pfeatures);FREE(pobj);
      return (sciPointObj *) NULL;
    }


  sons=ppsubwin->relationship.psons;
  /* check if s1 predecessor is null*/
  if (sons->pprev != (sciSons *)NULL) {
    sciprint("Unexpected case, please report\n");
    FREE(pobj->pfeatures);FREE(pobj);
    return (sciPointObj *) NULL;
  }

  /* change parent of all sons s1,...,sn*/
  lastsons=sons;

  
  /*   sciprint("debut\n"); */
  
  for (i=0;i<number;i++) {
   
    /*     sciprint("%8x %8x %8x  |  %8x\n",lastsons->pprev,lastsons->pointobj,lastsons->pnext, */
    /* 	     sciGetRelationship (lastsons->pointobj)->pparent); */
    
    (sciGetRelationship (lastsons->pointobj))->pparent=pobj;
    lastsons=lastsons->pnext;
  }

  lastsons=lastsons->pprev; /* lastsons is sn */

  /* disconnect chain s1->s2->...->sn out of subwin children list */
  ppsubwin->relationship.psons = lastsons->pnext;
  ppsubwin->relationship.psons->pprev = (sciSons *)NULL;
  /* attach the Compound to the current subwin */
  /* the subwin children list is now null->A->sn+1->...->null */
  if (!(sciAddThisToItsParent (pobj, (sciPointObj *)psubwin))) {
    FREE(pobj->pfeatures);FREE(pobj);
    return (sciPointObj *) NULL;
  }
  sciSetCurrentSon (pobj, (sciPointObj *) NULL);
  /* the subwin children list is now null->A->sn+1->...->null */

  /* set Compound properties*/
  ppagr->user_data = (int *) NULL; /* add missing init. 29.06.05 */
  ppagr->size_of_user_data = 0;
  ppagr->callback = (char *)NULL;
  ppagr->callbacklen = 0;
  ppagr->visible = sciGetVisibility (sciGetParentFigure(pobj));

  ppagr->isselected = TRUE;
 
  /* re chain A sons lists */
  ppagr->relationship.psons = sons;
  
  ppagr->relationship.plastsons = lastsons;
  ppagr->relationship.plastsons->pnext = (sciSons *)NULL;
  ppagr->relationship.psons->pprev = (sciSons *)NULL; /* this should do nothing*/
  /* the Compound children list is now  null->s1->s2->...->sn->null*/


  return (sciPointObj *)pobj;
}



/**ConstructLabel
 * @memo This function creates Label structure used for x,y,z labels and for the Title.
 * @param  sciPointObj *pparentsubwin
 * @param  char text[] : intial text string.
 * @param  int type to get info. on the type of label
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructLabel (sciPointObj * pparentsubwin, char *text, int type)
{
  sciPointObj * pobj =    (sciPointObj *) NULL;
  /* get a pointer on the feature */
  sciLabel    * ppLabel ; 
  
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
  {
    if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)
    {
      return (sciPointObj *) NULL;
    }

    sciSetEntityType (pobj, SCI_LABEL);
    if ((pobj->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

    ppLabel =  pLABEL_FEATURE( pobj ) ;
    
    if (sciAddNewHandle (pobj) == -1)
    {
      FREE(ppLabel);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
    
    /*  sciSetParent (pobj, pparentsubwin); */
    if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
    {
      sciDelHandle (pobj);
      FREE(ppLabel);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
    
    sciSetCurrentSon (pobj, (sciPointObj *) NULL);
    sciSetIsFilled(pobj,FALSE); /* by default a simple text is display (if existing) */
    ppLabel->user_data = (int *) NULL;
    ppLabel->size_of_user_data = 0;
    ppLabel->text.relationship.psons = (sciSons *) NULL;
    ppLabel->text.relationship.plastsons = (sciSons *) NULL;
    ppLabel->text.isboxed = FALSE ;
    ppLabel->text.isline  = TRUE ;
    ppLabel->text.callback = (char *) NULL;
    ppLabel->text.callbacklen = 0; 
    ppLabel->visible = sciGetVisibility(sciGetParentSubwin(pobj));
    
    if ((ppLabel->text.ptextstring =CALLOC (strlen(text)+1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent (pobj, sciGetParent (pobj));
      sciDelHandle (pobj);
      FREE(ppLabel);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

    /* on copie le texte du label dans le champs specifique de l'objet */
    strcpy (ppLabel->text.ptextstring, text);
    
    ppLabel->text.textlen = strlen(text);
    ppLabel->ptype = type;
    
    ppLabel->text.fontcontext.textorientation = 0; 
    
    /*   pLABEL_FEATURE (pobj)->titleplace = SCI_LABEL_IN_TOP; */
    ppLabel->isselected = TRUE;
    if (sciInitFontContext (pobj) == -1)
    {
      FREE(ppLabel->text.ptextstring);
      sciDelThisToItsParent (pobj, sciGetParent (pobj));
      sciDelHandle (pobj);
      FREE(ppLabel);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
    
    return (sciPointObj *) pobj;
  }
  else
  {
    sciprint ("The parent has to be a SUBWIN \n");
    return (sciPointObj *) NULL;
  }
}


/**ConstructMenu
 * @memo This function creates Menu structure
 */
sciPointObj *
ConstructMenu (sciPointObj * pparentfigure, char plabel[], int n)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_MENU);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciMenu)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pMENU_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pMENU_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      if (sciAddLabelMenu (pobj, plabel, n) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelLabelsMenu (pobj);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciInitFontContext (pobj);

      return (sciPointObj *) pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**ConstructMenuContext
 * @memo This function creates MenuContext structure only for tests on Widows system during developpement.
 * @memo ths function may be not incuded in the final version
 * pour obtenir un menu de contexe vide pour chacune des entites. L'utilisateur specifie avec un addlabel les label et callback necessaire ensuite.
 * @param  sciPointObj * pparentfigure: the parent entity
 */
sciPointObj *
ConstructMenuContext (sciPointObj * pparentfigure)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_MENUCONTEXT);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciMenuContext)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pMENUCONTEXT_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pMENUCONTEXT_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      /*if (sciAddLabelMenu (pobj, plabel, n) == -1)
	return (sciPointObj *) NULL;*/
      pMENUCONTEXT_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pMENUCONTEXT_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pMENUCONTEXT_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciInitFontContext (pobj);

      /* windows dependant*/
      /*     if ((pMENUCONTEXT_FEATURE(pobj)->hPopMenu = CreatePopupMenu ()) == NULL) 
	     {
	     sciprint ("Scilab can't have more menu \n");
	     FREE ((sciGetFontContext(pobj))->pfontname);
	     sciDelThisToItsParent (pobj, sciGetParent (pobj));
	     sciDelHandle (pobj);
	     FREE(pMENUCONTEXT_FEATURE (pobj));
	     FREE(pobj);
	     return (sciPointObj *) NULL;
	     }
	     AppendMenu (pMENUCONTEXT_FEATURE(pobj)->hPopMenu, MF_SEPARATOR , 0, NULL);*/
      /*AppendMenu (sciGethPopMenu (pobj), MF_STRING, 4401, plabel);*/

      return (sciPointObj *) pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**sciAddLabelMenu
 * @memo This function add one label to this menu
 * @param sciPointObj * pthis: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciAddLabelMenu (sciPointObj * pthis, char plabel[], int n)
{
  sciLabelMenu *pscilabelmenutmp;

  switch (sciGetEntityType (pthis))
    {
    case SCI_MENU:
      pscilabelmenutmp = (sciLabelMenu *) (pMENU_FEATURE (pthis)->plabelmenu);
      break;
    case SCI_MENUCONTEXT:
      pscilabelmenutmp =
	(sciLabelMenu *) (pMENUCONTEXT_FEATURE (pthis)->plabelmenu);
      break;
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  while (pscilabelmenutmp != (sciLabelMenu *) NULL)
    pscilabelmenutmp = (sciLabelMenu *) pscilabelmenutmp->pnextlabelmenu;
  if ((pscilabelmenutmp = MALLOC (sizeof (sciLabelMenu))) == NULL)
    return -1;
  if ((pscilabelmenutmp->plabel = CALLOC (n+1, sizeof (char))) == NULL)
    {
      sciprint("\nNo more place to allocates text string, try a shorter string\n");
      return -1;
    }
  strncpy (pscilabelmenutmp->plabel, plabel, n);
  pscilabelmenutmp->pnextlabelmenu = (sciLabelMenu *) NULL;
  return 0;
}


/*sciAttachPopMenu
 * This function sets the handle of the menu associated with this object
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
sciAttachPopMenu (sciPointObj *pthis, sciPointObj *pPopMenu)
{
  /*  HWND hwnd_window_pointed;*/     /* structure BCG periWIN ! winuser.h "Window */
   
  if (sciGetEntityType(pPopMenu) == SCI_MENUCONTEXT) /* verification of the validate type */
    {
      /* On cherche le handle de la fenetre selectionnnee */
      /* hwnd_window_pointed = WindowFromPoint(Point);    */
      switch(sciGetEntityType(pthis)) {
      case SCI_SUBWIN:
	pSUBWIN_FEATURE (pthis)->pPopMenu = pPopMenu;
	return 0;
	break;
      case SCI_MENUCONTEXT:
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
      default: 
	return -1;
	break;
      }
    }
  sciprint("Your second entity is not a popup menu !\n");
  return -1;
}

/**ConstructUimenu
 * @memo This function creates Uimenu structure.
 * @param  sciPointObj *pparentfigure
 * @param  char label[] : intial label string.
 * @param  char callback[] : intial text callback string .
 * @param  BOOL handle_visible : handle visibility in child list .
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj * ConstructUimenu (sciPointObj * pparent, char *label,char *callback,BOOL handle_visible)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciUimenu *ppobj=NULL;

  if ( (sciGetEntityType (pparent) == SCI_FIGURE) || (sciGetEntityType (pparent) == SCI_UIMENU) )
    {
      if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)	return (sciPointObj *) NULL;

      sciSetEntityType (pobj, SCI_UIMENU);

      if ((pobj->pfeatures = MALLOC ((sizeof (sciUimenu)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      ppobj=pUIMENU_FEATURE (pobj);
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*  sciSetParent (pobj, pparentsubwin); */
      if (!(sciAddThisToItsParent (pobj, pparent)))
	{
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
		
      pUIMENU_FEATURE (pobj)->label.relationship.psons = (sciSons *) NULL;
      pUIMENU_FEATURE (pobj)->label.relationship.plastsons = (sciSons *) NULL;

      if ((pUIMENU_FEATURE (pobj)->label.callback = CALLOC(strlen(callback)+1,sizeof(char))) == NULL )
	{
	  sciprint("No more place to allocates text string, try a shorter string");
	  return (sciPointObj *) NULL;
	}

      strcpy(pUIMENU_FEATURE (pobj)->label.callback,callback);
      pUIMENU_FEATURE (pobj)->label.callbacklen = strlen(callback); 

      pUIMENU_FEATURE (pobj)->visible = TRUE; /* A changer */ 

      if ((pUIMENU_FEATURE (pobj)->label.ptextstring =CALLOC (strlen(label)+1, sizeof (char))) == NULL)
	{
	  sciprint("No more place to allocates label string, try a shorter string");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pUIMENU_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* on copie le texte du label dans le champs specifique de l'objet */
      strcpy (pUIMENU_FEATURE (pobj)->label.ptextstring, label);

      pUIMENU_FEATURE (pobj)->label.textlen = strlen(label);
	  pUIMENU_FEATURE (pobj)->handle_visible=handle_visible;
	  pUIMENU_FEATURE (pobj)->MenuPosition=0;
	  pUIMENU_FEATURE (pobj)->CallbackType=0;
	  pUIMENU_FEATURE (pobj)->Enable=TRUE;

      return (sciPointObj *) pobj;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE or a UIMENU\n");
      return (sciPointObj *) NULL;
    }
}
