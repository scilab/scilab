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

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


sciPointObj *pfiguremdl = (sciPointObj *) NULL;
sciPointObj *paxesmdl = (sciPointObj *) NULL;

static char error_message[70]; /* DJ.A 08/01/04 */
extern unsigned short defcolors[];


/* DJ.A 08/01/04 */
int C2F(graphicsmodels) ()
{
 
  integer i ,m;
  char dir;
  sciHandleTab *newhd1, *newhd2;
  sciPointObj * pobj = NULL;
  sciSubWindow * ppobj = NULL;
  double tab[] = {0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.}; /* graduations init. tmptab */
  sciSubWindow * ppaxesmdl = NULL;
  /*  sciSubWindow * ppaxesmdl = NULL; */

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
  
  sciSetCurrentSon (pfiguremdl, (sciPointObj *) NULL);

  pFIGURE_FEATURE (pfiguremdl)->user_data = (int *) NULL; /* adding 30.06.05 */
  pFIGURE_FEATURE (pfiguremdl)->size_of_user_data = 0;
  pFIGURE_FEATURE (pfiguremdl)->relationship.psons = (sciSons *) NULL;
  pFIGURE_FEATURE (pfiguremdl)->relationship.plastsons = (sciSons *) NULL;
  pFIGURE_FEATURE (pfiguremdl)->numcolors=  NUMCOLORS_SCI;
  /** the colormap is mx3 matrix */
  m = NUMCOLORS_SCI; /* F.Leray 30.03.04 */
  if((pFIGURE_FEATURE(pfiguremdl)->pcolormap = (double *) MALLOC (m * 3 * sizeof (double))) == (double *) NULL)
    {
      sciDelHandle (pfiguremdl);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }  
  for (i= 0 ; i < m ; i++)
    {
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i] = (double) (defcolors[3*i]/255.0);
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+m] = (double) (defcolors[3*i+1]/255.0); 
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+2*m] = (double) (defcolors[3*i+2]/255.0);
    }

  /* initialisation de context et mode graphique par defaut */
  if (sciInitGraphicContext (pfiguremdl) == -1)
    {
      sciDelHandle (pfiguremdl);
      FREE(pFIGURE_FEATURE(pfiguremdl)->pcolormap);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  if (sciInitGraphicMode (pfiguremdl) == -1)
    {
      sciDelHandle (pfiguremdl);    
      FREE(pFIGURE_FEATURE(pfiguremdl)->pcolormap);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }   

  /* F.Leray 09.04.04 */
  if (sciInitFontContext (pfiguremdl) == -1)
    {
      sciDelHandle (pfiguremdl);
      FREE(pfiguremdl->pfeatures);	  
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }

  pFIGURE_FEATURE (pfiguremdl)->allredraw = FALSE;
  
  strncpy (pFIGURE_FEATURE (pfiguremdl)->name, "Scilab Graphic (%d)", sizeof ("Scilab Graphic (%d)") + 4);
  pFIGURE_FEATURE (pfiguremdl)->namelen = Min (60, 19); 
  pFIGURE_FEATURE (pfiguremdl)->number=0;
  pFIGURE_FEATURE (pfiguremdl)->figuredimwidth = 610;
  pFIGURE_FEATURE (pfiguremdl)->figuredimheight = 461;
  pFIGURE_FEATURE (pfiguremdl)->windowdimwidth = 600;
  pFIGURE_FEATURE (pfiguremdl)->windowdimheight = 400;
  pFIGURE_FEATURE (pfiguremdl)->inrootposx = -1;
  pFIGURE_FEATURE (pfiguremdl)->inrootposy = -1;
  pFIGURE_FEATURE (pfiguremdl)->isiconified = FALSE;
  pFIGURE_FEATURE (pfiguremdl)->isselected = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->rotstyle = 0;
  pFIGURE_FEATURE (pfiguremdl)->visible = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->auto_redraw = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->numsubwinselected = 0; 
  pFIGURE_FEATURE (pfiguremdl)->pixmap = 0; 
  pFIGURE_FEATURE (pfiguremdl)->wshow = 0; 

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
  sciSetCurrentSon (paxesmdl, (sciPointObj *) NULL);
  
  ppaxesmdl =  pSUBWIN_FEATURE (paxesmdl);

  ppaxesmdl->user_data = (int *) NULL; /* adding 30.06.05 */
  ppaxesmdl->size_of_user_data = 0;
  ppaxesmdl->relationship.psons = (sciSons *) NULL;
  ppaxesmdl->relationship.plastsons = (sciSons *) NULL;
  ppaxesmdl->callback = (char *)NULL;
  ppaxesmdl->callbacklen = 0;
  ppaxesmdl->callbackevent = 100;
  
  if (sciInitGraphicContext (paxesmdl) == -1)
    {
      sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);          
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }   
  if (sciInitGraphicMode (paxesmdl) == -1)
    {
      sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    } 
  
  
  /* F.Leray 09.04.04 */
  if (sciInitFontContext (paxesmdl) == -1)
  {
    sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
    sciDelHandle (paxesmdl);
    FREE(paxesmdl->pfeatures);	  
    FREE(paxesmdl);
    strcpy(error_message,"Default axes cannot be create");
    return 0;
  }
  
  ppaxesmdl->logflags[0] = 'n';
  ppaxesmdl->logflags[1] = 'n';
  ppaxesmdl->logflags[2] = 'n';
  
  /* axes labelling values*/
  ppaxesmdl->axes.ticscolor  = -1;
  /* F.Leray 08.04.04 : No need anymore:  */
  /*ppaxesmdl->axes.textcolor  = -1;
    ppaxesmdl->axes.fontsize  = -1;  */
  ppaxesmdl->axes.subint[0]  = 1;   
  ppaxesmdl->axes.subint[1]  = 1; 
  ppaxesmdl->axes.subint[2]  = 1;
  ppaxesmdl->axes.xdir='d'; 
  ppaxesmdl->axes.ydir='l';
     
  ppaxesmdl->axes.rect  = 0;
  for (i=0 ; i<7 ; i++)
    ppaxesmdl->axes.limits[i]  = 0;
  
  ppaxesmdl->cube_scaling = FALSE;
  for (i=0 ; i<3 ; i++)  ppaxesmdl->grid[i]  = -1;
  ppaxesmdl->alpha  = 0.0;
  ppaxesmdl->theta  = 270.0;
  ppaxesmdl->alpha_kp  = 45.0;
  ppaxesmdl->theta_kp  = 215.0;
  ppaxesmdl->is3d  = FALSE;
  ppaxesmdl->FirstPlot = TRUE;
  ppaxesmdl->with_leg = 0;

  dir= 'd'; ppaxesmdl->axes.xdir=dir;
  dir= 'l'; ppaxesmdl->axes.ydir=dir;      

  /* BU HERE */
  /*   ppaxesmdl = ppaxesmdl; */
  for (i=0 ; i<4 ; i++){  
    ppaxesmdl->axes.xlim[i]=  Cscale.xtics[i];
    ppaxesmdl->axes.ylim[i]=  Cscale.ytics[i]; }
  
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
  
  for(i=0;i<11;i++) (ppaxesmdl->axes).xgrads[i] = tab[i];
  for(i=0;i<11;i++) (ppaxesmdl->axes).ygrads[i] = tab[i];
  (ppaxesmdl->axes).zgrads[0] = -1.;
  (ppaxesmdl->axes).zgrads[1]  = 0.;
  (ppaxesmdl->axes).zgrads[2]  = 1.;

  (ppaxesmdl->axes).u_nxgrads = 0;
  (ppaxesmdl->axes).u_nygrads = 0;
  (ppaxesmdl->axes).u_nzgrads = 0;
  (ppaxesmdl->axes).u_xgrads = (double *)NULL;
  (ppaxesmdl->axes).u_ygrads = (double *)NULL;
  (ppaxesmdl->axes).u_zgrads = (double *)NULL;
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
 
  ppaxesmdl->project[0]= 1;
  ppaxesmdl->project[1]= 1;
  ppaxesmdl->project[2]= 0;
  ppaxesmdl->hiddencolor=4;
  ppaxesmdl->hiddenstate=0;
  ppaxesmdl->isoview= FALSE; 
  ppaxesmdl->facetmerge = FALSE;

  ppaxesmdl->WRect[0]   = 0;
  ppaxesmdl->WRect[1]   = 0;
  ppaxesmdl->WRect[2]   = 1;
  ppaxesmdl->WRect[3]   = 1;

  ppaxesmdl->ARect[0]   = 0.125;
  ppaxesmdl->ARect[1]   = 0.125;
  ppaxesmdl->ARect[2]   = 0.125;
  ppaxesmdl->ARect[3]   = 0.125;
  
  ppaxesmdl->FRect[0]   = 0.0;
  ppaxesmdl->FRect[1]   = 0.0;
  ppaxesmdl->FRect[2]   = 1.0;
  ppaxesmdl->FRect[3]   = 1.0;
  ppaxesmdl->FRect[4]   = -1.0;
  ppaxesmdl->FRect[5]   = 1.0;
  
  ppaxesmdl->SRect[0]   = 0.0;
  ppaxesmdl->SRect[1]   = 1.0;
  ppaxesmdl->SRect[2]   = 0.0;
  ppaxesmdl->SRect[3]   = 1.0;
  ppaxesmdl->SRect[4]   = -1.0;
  ppaxesmdl->SRect[5]   = 1.0;
  
  ppaxesmdl->clip_region[0] = 0.;
  ppaxesmdl->clip_region[1] = 0.;
  ppaxesmdl->clip_region[2] = 0.;
  ppaxesmdl->clip_region[3] = 0.;

  /* the model has not been changed !!! */
  ppaxesmdl->clip_region_set = 0 ;
  
  ppaxesmdl->tight_limits = FALSE;
  
  ppaxesmdl->isselected = FALSE;

  ppaxesmdl->visible = sciGetVisibility(pfiguremdl);
  /*   ppaxesmdl->drawlater = FALSE; */
  ppaxesmdl->isclip = -1;
  
  ppaxesmdl->pPopMenu = (sciPointObj *)NULL;
  
  /* F.Leray 10.06.04 */
  /* Adding default Labels inside Axes */
  /*------------------------------------------------------------------------------------*/
 
  pobj = paxesmdl;
  ppobj = pSUBWIN_FEATURE(paxesmdl);
  
  /******************************  title *************************/
  
  if ((ppobj->mon_title = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_title, SCI_LABEL);
  if (((ppobj->mon_title)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
  {
    FREE((ppobj->mon_title));
    return -1;
  }
  
  
  if (sciAddNewHandle ((ppobj->mon_title)) == -1)
  {
    FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
  }
  
  if (!(sciAddThisToItsParent ((ppobj->mon_title), paxesmdl)))
  {
    sciDelHandle ((ppobj->mon_title));
    FREE(pLABEL_FEATURE((ppobj->mon_title)));
    FREE((ppobj->mon_title));
    return -1;
  }
      
  sciSetCurrentSon ((ppobj->mon_title), (sciPointObj *) NULL);
  
  pLABEL_FEATURE ((ppobj->mon_title))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->size_of_user_data = 0;
  
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_title))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.callbacklen = 0;
  /*   pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_title))); */
  pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_title)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_title)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_title)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_title)->position[1] = 0;
  
  if ((pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
  /* init a "" pour le chanmp title */
  strcpy (pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_title))->ptype = 1; /* type = 1 <=> title, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_title))->titleplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_title))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_title)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_title))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }


      
  /******************************  x_label *************************/
  
  if ((ppobj->mon_x_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_x_label, SCI_LABEL);
  if (((ppobj->mon_x_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_x_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_x_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_x_label), (sciPointObj *) NULL);
  
  pLABEL_FEATURE ((ppobj->mon_x_label))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->size_of_user_data = 0;
  
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callbacklen = 0;
  /*  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_x_label))); */
  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_x_label)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_x_label)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_x_label)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_x_label)->position[1] = 0;

  if ((pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
  /* init a "" pour le chanmp x_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_x_label))->ptype = 2; /* type = 1 <=> x_label, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_x_label))->x_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_x_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }


  
  /******************************  y_label *************************/
  
  if ((ppobj->mon_y_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_y_label, SCI_LABEL);
  if (((ppobj->mon_y_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_y_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_y_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_y_label), (sciPointObj *) NULL);

  pLABEL_FEATURE ((ppobj->mon_y_label))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->size_of_user_data = 0;
 
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callbacklen = 0;
  /*  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_y_label))); */
  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_y_label)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_y_label)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_y_label)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_y_label)->position[1] = 0;

  if ((pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
  /* init a "" pour le chanmp y_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_y_label))->ptype = 3; /* type = 1 <=> y_label, 2 <=> y_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_y_label))->y_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_y_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }

  
  /******************************  z_label *************************/
  
  if ((ppobj->mon_z_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_z_label, SCI_LABEL);
  if (((ppobj->mon_z_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_z_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_z_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_z_label), (sciPointObj *) NULL);

  pLABEL_FEATURE ((ppobj->mon_z_label))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->size_of_user_data = 0;
  
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callbacklen = 0;
  /*  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_z_label))); */
  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_z_label)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_z_label)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_z_label)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_z_label)->position[1] = 0;
  
  if ((pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
  /* init a "" pour le chanmp z_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_z_label))->ptype = 4; /* type = 1 <=> z_label, 2 <=> z_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_z_label))->z_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_z_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }

  
  return 1;
}


/**sciInitGraphicContext
 * @memo Inits the graphic context of this object with the default value. the new graphic object inherits parent's features by sciGetParent()
 */
int
sciInitGraphicContext (sciPointObj * pobj)
{
  /* 
   * initialisation du contexte graphique par defaut 
   * que l'on peut recuperer sur les structure de base de scilab 
   * la colormap des fils est heritee du parent
   */

  /* F.Leray debug 30.03.04*/
  /*
    sciGraphicContext * psciGC_pobj = sciGetGraphicContext(pobj); 
    sciGraphicContext * psciGC_paxe = sciGetGraphicContext(paxesmdl);
    sciGraphicContext * psciGC_pfig = sciGetGraphicContext(pfiguremdl);
  */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (pobj == pfiguremdl)
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
	  (sciGetGraphicContext(pobj))->marksize = 0; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markbackground = -3; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markforeground = -2; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->marksizeunit = 2; /* New F.Leray 22.02.05 */ /* 1 : points, 2 : tabulated */
	}
      else
	{
	  (sciGetGraphicContext(pobj))->backgroundcolor = (sciGetGraphicContext(pfiguremdl))->backgroundcolor;
	  (sciGetGraphicContext(pobj))->foregroundcolor = (sciGetGraphicContext(pfiguremdl))->foregroundcolor;
	  (sciGetGraphicContext(pobj))->fillstyle = (sciGetGraphicContext(pfiguremdl))->fillstyle;
	  (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pfiguremdl))->fillcolor;
	  (sciGetGraphicContext(pobj))->linewidth = (sciGetGraphicContext(pfiguremdl))->linewidth;
	  (sciGetGraphicContext(pobj))->linestyle = (sciGetGraphicContext(pfiguremdl))->linestyle;
 	  (sciGetGraphicContext(pobj))->ismark    = (sciGetGraphicContext(pfiguremdl))->ismark ;
 	  (sciGetGraphicContext(pobj))->isline    = (sciGetGraphicContext(pfiguremdl))->isline ;
	  (sciGetGraphicContext(pobj))->markstyle = (sciGetGraphicContext(pfiguremdl))->markstyle;
	  (sciGetGraphicContext(pobj))->marksize = (sciGetGraphicContext(pfiguremdl))->marksize; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markbackground = (sciGetGraphicContext(pfiguremdl))->markbackground; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markforeground = (sciGetGraphicContext(pfiguremdl))->markforeground; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->marksizeunit = (sciGetGraphicContext(pfiguremdl))->marksizeunit; /* New F.Leray 22.02.05 */
	  
	}
      return 0;
      break;
    case SCI_SUBWIN:
      if (pobj == paxesmdl)
	{
	  (sciGetGraphicContext(pobj))->backgroundcolor =	-3;  /* F.Leray 09.04.04 POSSIBLE DISCUSSION HERE : may it be (sciGetGraphicContext(sciGetParent(pobj)))->backgroundcolor ??*/
	  (sciGetGraphicContext(pobj))->foregroundcolor =	-2;  /* HERE too : may it be  (sciGetGraphicContext(sciGetParent(pobj)))->foregroundcolor ??*/
	  (sciGetGraphicContext(pobj))->fillstyle =	sciGetFillStyle (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
	  (sciGetGraphicContext(pobj))->linewidth =	sciGetLineWidth (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->linestyle =	sciGetLineStyle (sciGetParent (pobj));
 	  (sciGetGraphicContext(pobj))->ismark    =	sciGetIsMark (sciGetParent (pobj));
 	  (sciGetGraphicContext(pobj))->isline    =	sciGetIsLine (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->markstyle =	sciGetMarkStyle (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->marksize = 	sciGetMarkSize (sciGetParent (pobj)); /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markbackground = -3; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markforeground = -2; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->marksizeunit = 	sciGetMarkSizeUnit (sciGetParent (pobj)); /* New F.Leray 22.02.05 */ /* 1 : points, 2 : tabulated */
	}
      else
	{
	  (sciGetGraphicContext(pobj))->backgroundcolor = (sciGetGraphicContext(paxesmdl))->backgroundcolor;
	  (sciGetGraphicContext(pobj))->foregroundcolor = (sciGetGraphicContext(paxesmdl))->foregroundcolor;
	  (sciGetGraphicContext(pobj))->fillstyle = (sciGetGraphicContext(paxesmdl))->fillstyle;
	  (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(paxesmdl))->fillcolor;
	  (sciGetGraphicContext(pobj))->linewidth = (sciGetGraphicContext(paxesmdl))->linewidth;
	  (sciGetGraphicContext(pobj))->linestyle = (sciGetGraphicContext(paxesmdl))->linestyle;
 	  (sciGetGraphicContext(pobj))->ismark    = (sciGetGraphicContext(paxesmdl))->ismark;
 	  (sciGetGraphicContext(pobj))->isline    = (sciGetGraphicContext(paxesmdl))->isline;
	  (sciGetGraphicContext(pobj))->markstyle = (sciGetGraphicContext(paxesmdl))->markstyle;
	  (sciGetGraphicContext(pobj))->marksize = (sciGetGraphicContext(paxesmdl))->marksize; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markbackground = (sciGetGraphicContext(paxesmdl))->markbackground; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->markforeground = (sciGetGraphicContext(paxesmdl))->markforeground; /* New F.Leray 21.01.05 */
	  (sciGetGraphicContext(pobj))->marksizeunit = (sciGetGraphicContext(paxesmdl))->marksizeunit; /* New F.Leray 22.02.05 */
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
      (sciGetGraphicContext(pobj))->backgroundcolor =	sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetGraphicContext(pobj))->foregroundcolor =	sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetGraphicContext(pobj))->fillstyle =	sciGetFillStyle (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
      (sciGetGraphicContext(pobj))->linewidth =	sciGetLineWidth (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->linestyle =	sciGetLineStyle (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->ismark    =	sciGetIsMark (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->isline    =	sciGetIsLine (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->markstyle =	sciGetMarkStyle (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->marksize  = sciGetMarkSize  (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->markbackground  = sciGetMarkBackground (sciGetParent (pobj)) - 1;
      (sciGetGraphicContext(pobj))->markforeground  = sciGetMarkForeground (sciGetParent (pobj)) - 1;
      (sciGetGraphicContext(pobj))->marksizeunit  = sciGetMarkSizeUnit  (sciGetParent (pobj));
      return 0;
      break;
    case SCI_AGREG:
    case SCI_TITLE:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciInitFontContext
 * @memo Inits the graphic context of this object with the default value
 * @param sciPointObj * pobj: the pointer to the entity
 */
int
sciInitFontContext (sciPointObj * pobj)
{
  /*   int aa[5]; */ /* for debug purpose only F.Leray 13.01.05 */

  /* 
   * initialisation du contexte font par defaut 
   * que l'on peut recuperer sur les structures de base de scilab 
   * la colormap des fils est heritee du parent
   * nous prenons le couleur de background et foreground
   * plutot que fontforeground pour l'initialisation
   */

  /* unknown function initfontname "Win-stand"!! */
  /* static TCHAR inifontname[] = TEXT ("Times New Roman");*/  

  /*  sciPointObj * psubwin = NULL;*/
  

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
      
      (sciGetFontContext(pobj))->backgroundcolor/*  = aa[0] */ =  sciGetFontBackground (sciGetParent (pobj)) -1;
      (sciGetFontContext(pobj))->foregroundcolor/*  = aa[1] */ =  sciGetFontForeground (sciGetParent (pobj)) -1;
      (sciGetFontContext(pobj))->fonttype       /*  = aa[2] */ =  (sciGetFontContext(sciGetParent(pobj)))->fonttype; 
      (sciGetFontContext(pobj))->fontdeciwidth  /*  = aa[3] */ =  (sciGetFontContext(sciGetParent(pobj)))->fontdeciwidth;
      (sciGetFontContext(pobj))->textorientation/*  = aa[4] */ =  (sciGetFontContext(sciGetParent(pobj)))->textorientation;
      (sciGetFontContext(pobj))->fontnamelen    /*  = aa[5] */ =  (sciGetFontContext(sciGetParent(pobj)))->fontnamelen; 
      if (((sciGetFontContext(pobj))->pfontname =
	   CALLOC ((sciGetFontContext(pobj))->fontnamelen + 1,
		   sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n"); 
	  return 0;
	}
      break;
    case SCI_LABEL: /* Re-init here must be better F.Leray 28.05.04 */
      if(sciGetParent(pobj) == paxesmdl)
	{
	  /* init plabelmdl that could be models for title, x_label, y_label or z_label */
	  sciSetFontStyle (pobj, 6);/* set helvetica font */
	  (sciGetFontContext(pobj))->backgroundcolor = -3;
	  (sciGetFontContext(pobj))->foregroundcolor = -2; 
	  (sciGetFontContext(pobj))->fontdeciwidth = 100;
	  (sciGetFontContext(pobj))->textorientation = 0;
	  (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
	  (sciGetFontContext(pobj))->textorientation = 0;

	  if (((sciGetFontContext(pobj))->pfontname =
	       CALLOC ((sciGetFontContext(pobj))->fontnamelen + 1,
		       sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n"); 
	      return 0;
	    }
	}
      else
	{
	  sciPointObj * plabelmdl = NULL;
	  
	  if(pLABEL_FEATURE(pobj)->ptype == 1) /* title */
	    plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_title;
	  else if(pLABEL_FEATURE(pobj)->ptype == 2) /* x_label */
	    plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_x_label;
	  else if(pLABEL_FEATURE(pobj)->ptype == 3) /* y_label */
	    plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_y_label;
	  else if(pLABEL_FEATURE(pobj)->ptype == 4) /* z_label */
	    plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_z_label;
	  
	  (sciGetFontContext(pobj))->backgroundcolor/*  = aa[0] */ = (sciGetFontContext(plabelmdl))->backgroundcolor;
	  (sciGetFontContext(pobj))->foregroundcolor/*  = aa[1] */ = (sciGetFontContext(plabelmdl))->foregroundcolor;
	  (sciGetFontContext(pobj))->fonttype  /*=        aa[2] */ = (sciGetFontContext(plabelmdl))->fonttype;
 	  (sciGetFontContext(pobj))->fontdeciwidth  /*=   aa[3] */ = (sciGetFontContext(plabelmdl))->fontdeciwidth;
	  (sciGetFontContext(pobj))->fontnamelen  /*=     aa[5] */ = (sciGetFontContext(plabelmdl))->fontnamelen; /*fontname not used */
	  (sciGetFontContext(pobj))->textorientation/*  = aa[4] */ = (sciGetFontContext(plabelmdl))->textorientation;
	  
	  if (
	      ((sciGetFontContext(pobj))->pfontname = CALLOC ((sciGetFontContext(pobj))->fontnamelen + 1,
							      sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n");
	      return 0;
	    }
	}
      break;
    case SCI_SUBWIN: 
      if (pobj == paxesmdl)
	{
	  sciSetFontStyle (pobj, 6);/* set helvetica font */
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
	}
      else
	{
	  (sciGetFontContext(pobj))->backgroundcolor /* = aa[0] */ = (sciGetFontContext(paxesmdl))->backgroundcolor;
	  (sciGetFontContext(pobj))->foregroundcolor/*  = aa[1] */ = (sciGetFontContext(paxesmdl))->foregroundcolor;
	  (sciGetFontContext(pobj))->fonttype /* =        aa[2] */ = (sciGetFontContext(paxesmdl))->fonttype;
 	  (sciGetFontContext(pobj))->fontdeciwidth /* =   aa[3] */ =(sciGetFontContext(paxesmdl))->fontdeciwidth;
	  (sciGetFontContext(pobj))->textorientation/*  = aa[4] */ =(sciGetFontContext(paxesmdl))->textorientation;
	  (sciGetFontContext(pobj))->fontnamelen /* =     aa[5] */ =(sciGetFontContext(paxesmdl))->fontnamelen; /*fontname not used */
	  
	  if (
	      ((sciGetFontContext(pobj))->pfontname = CALLOC ((sciGetFontContext(pobj))->fontnamelen + 1,
							      sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n");
	      return 0;
	    }
	  /* END ADDING F.Leray 08.04.04*/ 
	} 
      break;
    case SCI_FIGURE:
      if (pobj == pfiguremdl)
	{
	  sciSetFontStyle (pobj, 6);/* set helvetica font */
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
	  /* START ADDING F.Leray 08.04.04*/ 
	  /* F.Leray 08.04.04 : I create a  sciFont fontcontext; to act on Axes font*/
	  (sciGetFontContext(pobj))->backgroundcolor = (sciGetFontContext(pfiguremdl))->backgroundcolor;	
	  (sciGetFontContext(pobj))->foregroundcolor = (sciGetFontContext(pfiguremdl))->foregroundcolor;
	  (sciGetFontContext(pobj))->fonttype =        (sciGetFontContext(pfiguremdl))->fonttype;
  	  (sciGetFontContext(pobj))->fontdeciwidth =   (sciGetFontContext(pfiguremdl))->fontdeciwidth;
	  (sciGetFontContext(pobj))->textorientation = (sciGetFontContext(pfiguremdl))->textorientation;
	  (sciGetFontContext(pobj))->fontnamelen =     (sciGetFontContext(pfiguremdl))->fontnamelen; /*fontname not used */
	  
	  if (
	      ((sciGetFontContext(pobj))->pfontname = CALLOC ((sciGetFontContext(pobj))->fontnamelen + 1,
							      sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n");
	      return 0;
	    }
	  /* END ADDING F.Leray 08.04.04*/  
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
  sciPointObj *psubwin;
  char dir;

  Cscale2default(); 
  psubwin=sciGetSelectedSubWin (sciGetCurrentFigure ());
  
  pSUBWIN_FEATURE (psubwin)->user_data = (int *) NULL; /* adding 30.06.05 */
  pSUBWIN_FEATURE (psubwin)->size_of_user_data = 0;
  
  pSUBWIN_FEATURE (psubwin)->WRect[0]   = 0;
  pSUBWIN_FEATURE (psubwin)->WRect[1]   = 0;
  pSUBWIN_FEATURE (psubwin)->WRect[2]   = 1;
  pSUBWIN_FEATURE (psubwin)->WRect[3]   = 1;

  pSUBWIN_FEATURE (psubwin)->FRect[0]   = 0.;
  pSUBWIN_FEATURE (psubwin)->FRect[1]   = 0.;
  pSUBWIN_FEATURE (psubwin)->FRect[2]   = 1.;
  pSUBWIN_FEATURE (psubwin)->FRect[3]   = 1.; 
  pSUBWIN_FEATURE (psubwin)->FRect[4]   =-1.;
  pSUBWIN_FEATURE (psubwin)->FRect[5]   = 1.; 
  /* F.Leray 05.04.04 */
  pSUBWIN_FEATURE (psubwin)->SRect[0]   = 0.0; /* xmin */
  pSUBWIN_FEATURE (psubwin)->SRect[1]   = 1.0; /* xmax */
  pSUBWIN_FEATURE (psubwin)->SRect[2]   = 0.0;/* ymin */
  pSUBWIN_FEATURE (psubwin)->SRect[3]   = 1.0; /* ymax */
  pSUBWIN_FEATURE (psubwin)->SRect[4]   = -1.0; /* zmin */
  pSUBWIN_FEATURE (psubwin)->SRect[5]   = 1.0; /* zmax */
            
  dir= 'd'; pSUBWIN_FEATURE (psubwin)->axes.xdir=dir;
  dir= 'l'; pSUBWIN_FEATURE (psubwin)->axes.ydir=dir;
  (pSUBWIN_FEATURE (psubwin)->axes).axes_visible[0] = FALSE;
  (pSUBWIN_FEATURE (psubwin)->axes).axes_visible[1] = FALSE;
  (pSUBWIN_FEATURE (psubwin)->axes).axes_visible[2] = FALSE;
  (pSUBWIN_FEATURE (psubwin)->axes).reverse[0] = FALSE;
  (pSUBWIN_FEATURE (psubwin)->axes).reverse[1] = FALSE;
  (pSUBWIN_FEATURE (psubwin)->axes).reverse[2] = FALSE;
  
  pSUBWIN_FEATURE (psubwin)->axes.rect = 0;  
  pSUBWIN_FEATURE (psubwin)->axes.ticscolor = -1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[0] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[1] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[2] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.limits[0]  = 0;  
  pSUBWIN_FEATURE (psubwin)->visible = TRUE;
  /*   pSUBWIN_FEATURE (psubwin)->drawlater = FALSE; */
  pSUBWIN_FEATURE (psubwin)->is3d = FALSE;  
  pSUBWIN_FEATURE (psubwin)->alpha  = 0.0;
  pSUBWIN_FEATURE (psubwin)->theta  = 270.0;
  pSUBWIN_FEATURE (psubwin)->FirstPlot = TRUE;
}

int InitFigureModel()
{ 
  int i, m = NUMCOLORS_SCI;
  /*sciPointObj *pfiguremdl = (sciPointObj *) NULL;*/ /* DJ.A 08/01/04 */
  pFIGURE_FEATURE (pfiguremdl)->allredraw = FALSE;

  sciInitGraphicContext (pfiguremdl);
  sciInitGraphicMode (pfiguremdl);
  sciInitFontContext (pfiguremdl);  /* F.Leray 10.06.04 */
  strncpy (pFIGURE_FEATURE (pfiguremdl)->name, "Scilab Graphic", sizeof ("Scilab Graphic") + 4);
  pFIGURE_FEATURE (pfiguremdl)->namelen = Min (sizeof ("Scilab Graphic") + 4, 14); 
  pFIGURE_FEATURE (pfiguremdl)->number=0;
  pFIGURE_FEATURE (pfiguremdl)->figuredimwidth = 610;
  pFIGURE_FEATURE (pfiguremdl)->figuredimheight = 461;
  pFIGURE_FEATURE (pfiguremdl)->inrootposx = 200;
  pFIGURE_FEATURE (pfiguremdl)->inrootposy = 200;
  if((pFIGURE_FEATURE(pfiguremdl)->pcolormap = (double *) MALLOC (m * 3 * sizeof (double))) == (double *) NULL)
    {
      strcpy(error_message,"Cannot init color map");
      return 0;
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
  pFIGURE_FEATURE (pfiguremdl)->pixmap = 0; 
  pFIGURE_FEATURE (pfiguremdl)->wshow = 0;
  return 1;
}



int InitAxesModel()
{ 
  char dir;
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
  ppaxesmdl->axes.rect  = 0;
  
  ppaxesmdl->user_data = (int *) NULL; /* to be complete */
  ppaxesmdl->size_of_user_data = 0;

  for (i=0 ; i<7 ; i++)
    ppaxesmdl->axes.limits[i]  = 0;

  for (i=0 ; i<3 ; i++)
    ppaxesmdl->grid[i]  = -1;
  ppaxesmdl->alpha  = 0.0;
  ppaxesmdl->theta  = 270.0;
  ppaxesmdl->alpha_kp  = 45.0;
  ppaxesmdl->theta_kp  = 215.0;
  ppaxesmdl->is3d  = FALSE;  
  dir= 'd'; ppaxesmdl->axes.xdir=dir;
  dir= 'l'; ppaxesmdl->axes.ydir=dir;      
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

  for(i=0;i<11;i++) (ppaxesmdl->axes).xgrads[i] = tab[i];
  for(i=0;i<11;i++) (ppaxesmdl->axes).ygrads[i] = tab[i];
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
  ppaxesmdl->project[0]= 1;
  ppaxesmdl->project[1]= 1;
  ppaxesmdl->project[2]= 0;
  ppaxesmdl->hiddencolor=4;
  ppaxesmdl->hiddenstate=0; 
  ppaxesmdl->isoview= FALSE;/*TRUE;*/
 
  ppaxesmdl->facetmerge = FALSE; 
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
  ppaxesmdl->isselected = FALSE;
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

  /* F.Leray 10.06.04 */
  /* Adding default Labels inside Axes */
  /*------------------------------------------------------------------------------------*/
 
  pobj = paxesmdl;
  ppobj = pSUBWIN_FEATURE(paxesmdl);
  
  /******************************  title *************************/
  
  if ((ppobj->mon_title = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_title, SCI_LABEL);
  if (((ppobj->mon_title)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_title));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_title)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
  
  if (!(sciAddThisToItsParent ((ppobj->mon_title), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_title), (sciPointObj *) NULL);
  
  pLABEL_FEATURE ((ppobj->mon_title))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->size_of_user_data = 0;
  
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_title))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.callbacklen = 0; 
  /*  pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_title)));  */
  pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_title)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_title)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_title)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_title)->position[1] = 0;

  if ((pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
  /* init a "" pour le chanmp title */
  strcpy (pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_title))->ptype = 1; /* type = 1 <=> title, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_title))->titleplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_title))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_title)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_title))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }


      
  /******************************  x_label *************************/
  
  if ((ppobj->mon_x_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_x_label, SCI_LABEL);
  if (((ppobj->mon_x_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_x_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_x_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_x_label), (sciPointObj *) NULL);
  
  pLABEL_FEATURE ((ppobj->mon_x_label))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->size_of_user_data = 0;
  
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callbacklen = 0; 
  /*  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_x_label)));  */
  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_x_label)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_x_label)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_x_label)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_x_label)->position[1] = 0;

  if ((pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
  /* init a "" pour le chanmp x_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_x_label))->ptype = 2; /* type = 1 <=> x_label, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_x_label))->x_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_x_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }


  
  /******************************  y_label *************************/
  
  if ((ppobj->mon_y_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_y_label, SCI_LABEL);
  if (((ppobj->mon_y_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_y_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_y_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_y_label), (sciPointObj *) NULL);
  
  pLABEL_FEATURE ((ppobj->mon_y_label))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->size_of_user_data = 0;
  
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callbacklen = 0; 
  /*  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_y_label)));  */
  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_y_label)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_y_label)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_y_label)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_y_label)->position[1] = 0;
  
  if ((pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
  /* init a "" pour le chanmp y_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_y_label))->ptype = 3; /* type = 1 <=> y_label, 2 <=> y_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_y_label))->y_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_y_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }

  
  /******************************  z_label *************************/
  
  if ((ppobj->mon_z_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_z_label, SCI_LABEL);
  if (((ppobj->mon_z_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_z_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_z_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
  
  sciSetCurrentSon ((ppobj->mon_z_label), (sciPointObj *) NULL);
  
  pLABEL_FEATURE ((ppobj->mon_z_label))->user_data = (int *) NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->size_of_user_data = 0;
  
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callbacklen = 0; 
  /*  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_z_label)));  */
  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(paxesmdl);
  pLABEL_FEATURE (ppobj->mon_z_label)->auto_position = TRUE;
  pLABEL_FEATURE (ppobj->mon_z_label)->auto_rotation = TRUE;
  pLABEL_FEATURE (ppobj->mon_z_label)->position[0] = 0;
  pLABEL_FEATURE (ppobj->mon_z_label)->position[1] = 0;

  if ((pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring =CALLOC (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
  /* init a "" pour le chanmp z_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_z_label))->ptype = 4; /* type = 1 <=> z_label, 2 <=> z_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_z_label))->z_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_z_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }

  return 1; 
}



int ResetFigureToDefaultValues(sciPointObj * pobj)
{
  integer i , m, n;
  integer x[2], verbose=0, narg=0; 
  struct BCG *XGC=NULL;
  int succeed = 0;
  

  if(sciGetEntityType(pobj)!=SCI_FIGURE) /* MUST BE used for figure entities only */
    return -1;

  XGC = pFIGURE_FEATURE (pobj)->pScilabXgc;
  
  pFIGURE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
  pFIGURE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
  /*   pFIGURE_FEATURE (pobj)->pScilabXgc = XGC; */
  
  /** Initialize the colormap */
  n=3;
  m = pFIGURE_FEATURE (pfiguremdl)->numcolors;
  /* try to install the colormap in the graphic context */
  C2F(dr)("xset","colormap",&m,&n,&succeed,PI0,PI0,PI0,
	  pFIGURE_FEATURE(pfiguremdl)->pcolormap,PD0,PD0,PD0,0L,0L);

  if(succeed == 1){ /* failed to allocate or xinit (for Gif driver) was missing */
    sciprint ("Failed to change colormap : Allocation failed or missing xinit detected\n");
    return -1;
  }
  
  if((pFIGURE_FEATURE(pobj)->pcolormap = (double *) MALLOC (XGC->Numcolors * n * sizeof (double))) == (double *) NULL)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return -1;
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
  sciSetFigurePos (pobj,x[0],x[1]);
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
 * @memo Inits the graphic mode of this object with the default value
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
	  (sciGetGraphicMode (pobj))->oldstyle = FALSE;
	  (sciGetGraphicMode (pobj))->xormode = 3;
	}
      else
	{
	  (sciGetGraphicMode (pobj))->wresize = (sciGetGraphicMode (pfiguremdl))->wresize;
	  (sciGetGraphicMode (pobj))->addplot = (sciGetGraphicMode (pfiguremdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (pfiguremdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming = (sciGetGraphicMode (pfiguremdl))->zooming;
	  (sciGetGraphicMode (pobj))->oldstyle = (sciGetGraphicMode (pfiguremdl))->oldstyle;
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
	  (sciGetGraphicMode (pobj))->oldstyle =sciGetGraphicsStyle (sciGetParent (pobj)); 
	  (sciGetGraphicMode (pobj))->xormode =sciGetXorMode (sciGetParent (pobj));
      	}
      else
	{
	  (sciGetGraphicMode (pobj))->addplot =(sciGetGraphicMode (paxesmdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (paxesmdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming =(sciGetGraphicMode (paxesmdl))->zooming;
	  (sciGetGraphicMode (pobj))->oldstyle =(sciGetGraphicMode (paxesmdl))->oldstyle;
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
