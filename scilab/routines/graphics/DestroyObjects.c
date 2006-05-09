/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to BUILD new objects : 
 - break the binding between the deleted object and its parent in the 
 existing hierarchy
 - freeing memory
 --------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "DestroyObjects.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "SetProperty.h"
#include "Interaction.h" /* for callback funtions */

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


/********************* modifier le 01/02/2002 ************************
 * On detruit pas la sous fenetre, elle est initialiser avec la figure
 * pour cette version, on considere qu'il y'a 1 seule sous fenetre et 
 * elle suit la fenetre principale (voir xbasc() ), la fenetre n'est pas 
 * consideree comme un des fils.  
 **DestroyAllGraphicsSons
 * This function destroies childs and pthis
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyAllGraphicsSons (sciPointObj * pthis)
{
  /* to destroy only the sons put the while into the switch !*/
  sciSons *toto = NULL;
 
  toto = sciGetSons (pthis);
  while ((toto != (sciSons *) NULL) &&
	 (toto->pointobj != (sciPointObj *) NULL))
    {
      DestroyAllGraphicsSons (toto->pointobj);
      toto = sciGetSons (pthis);
    }
  switch (sciGetEntityType (pthis))
    {

    case SCI_FIGURE:
      return 0;
      break;
    case SCI_SUBWIN:
      DestroySubWin (pthis);
      return 0;
      break;
    case SCI_TEXT:
      DestroyText (pthis);
      return 0;
      break;
    case SCI_TITLE:
      DestroyTitle (pthis);
      return 0;
      break;
    case SCI_LEGEND:
      DestroyLegend (pthis);
      return 0;
      break;
    case SCI_ARC:
      DestroyArc (pthis);
      return 0; 
      break;
    case SCI_SEGS:
      DestroySegs (pthis);
      return 0;
      break;  
    case SCI_FEC:  
      DestroyFec (pthis);
      return 0;
      break; 
    case SCI_GRAYPLOT: 
      DestroyGrayplot (pthis);
      return 0;
      break; 
    case SCI_POLYLINE:
      DestroyPolyline (pthis);
      return 0;
      break;
    case SCI_RECTANGLE:
      DestroyRectangle (pthis);
      return 0;
      break;
    case SCI_SURFACE:
      DestroySurface (pthis);
      return 0;
      break;
    case SCI_LIGHT:
      break;
    case SCI_AXES:
      DestroyAxes (pthis);
      return 0;
      break;
    case SCI_PANNER:
      break;
    case SCI_SBH:
      DestroyScrollH (pthis);
      return 0;
      break;
    case SCI_SBV:
      DestroyScrollV (pthis);
      return 0;
      break;
    case SCI_MENU:
      DestroySciMenu (pthis);
      return 0;
      break;
    case SCI_MENUCONTEXT:
      DestroyMenuContext (pthis);
      return 0;
      break;
    case SCI_STATUSB:
      DestroyStatusBar (pthis);
      return 0;
      break;
    case SCI_AGREG:
      DestroyCompound (pthis);
      return 0;
      break; 
    case SCI_MERGE:
      DestroyMerge (pthis);
      return 0;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      DestroyLabel (pthis);
      return 0;
      break;
    case SCI_UIMENU:
      DestroyUimenu (pthis);
      return 0;
      break;
    default:
      sciprint ("Entity with type %d cannot be destroyed\n",sciGetEntityType (pthis));
      return -1;
      break;
    }

  return 0;
}



/**sciDelGraphicObj
 * This function delete only users graphics object and its dependency limited to 
 * SCI_TITLE SCI_LEGEND SCI_ARC SCI_POLYLINE SCI_RECTANGLE SCI_SURFACE 
 * SCI_AXIS SCI_MENU SCI_MENUCONTEXT SCI_SUBWIN
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
sciDelGraphicObj (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:

    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_MERGE: 
    case SCI_LABEL:
    case SCI_UIMENU:
      DestroyAllGraphicsSons (pthis);
      return 0;
      break;
    case SCI_SUBWIN:
      if (sciGetAxes (sciGetParent(pthis),pthis) == (sciPointObj *) NULL)
      	sciXbasc();
      else
        DestroyAllGraphicsSons (pthis);
      return 0;
      break;         
    case SCI_FIGURE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_STATUSB:
    default:
      sciprint ("This object cannot be deleted\n");
      return -1;
      break;
    }

  return 0;
}

/**DestroyFigure
 * @memo This function destroies the parents window (manager) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyFigure (sciPointObj * pthis)
{
  /* there is no needed for 
   * sciDelThisToItsParent(pthis, );
   */
  if (sciDelHandle (pthis) == -1)
    return -1;
  /* This code has to be validated on all systems
   * because sciGetPointerToFeature returns a void
   */
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE(pFIGURE_FEATURE(pthis)->pcolormap);
  FREE(pFIGURE_FEATURE(pthis)->user_data);
  pFIGURE_FEATURE(pthis)->size_of_user_data = 0;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* delete windows() */
  return 0;
}



/**DestroyStatusBar
 * @memo This function destroies the StatusBar of Figure and the elementaries structures and returns pthis with NULL pointer and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyStatusBar (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer la figure parente */
  return 0;
}



/**DestroySubWin
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroySubWin (sciPointObj * pthis)
{ 
  /* Add. grads arrays */ /* F.Leray 11.10.04 */
  /* specific user arrays */
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pthis);


  FREE( ppsubwin->axes.u_xgrads); ppsubwin->axes.u_xgrads = (double *) NULL;
  FREE( ppsubwin->axes.u_ygrads); ppsubwin->axes.u_ygrads = (double *) NULL;
  FREE( ppsubwin->axes.u_zgrads); ppsubwin->axes.u_zgrads = (double *) NULL;
  ppsubwin->axes.u_xlabels = FreeUserLabels(ppsubwin->axes.u_xlabels, &ppsubwin->axes.u_nxgrads);
  ppsubwin->axes.u_ylabels = FreeUserLabels(ppsubwin->axes.u_ylabels, &ppsubwin->axes.u_nygrads);
  ppsubwin->axes.u_zlabels = FreeUserLabels(ppsubwin->axes.u_zlabels, &ppsubwin->axes.u_nzgrads);

  ppsubwin->axes.u_xlabels = NULL;
  ppsubwin->axes.u_ylabels = NULL;
  ppsubwin->axes.u_zlabels = NULL;

  ppsubwin->axes.u_nxgrads = 0;
  ppsubwin->axes.u_nygrads = 0;
  ppsubwin->axes.u_nzgrads = 0;
 
 
  /* auto (computed) arrays are defined with max. length == 20 */
  ppsubwin->axes.nxgrads = 0;
  ppsubwin->axes.nygrads = 0;
  ppsubwin->axes.nzgrads = 0;

  FREE(ppsubwin->user_data);
  ppsubwin->size_of_user_data = 0;

  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  if ( sciGetCallback(pthis) != (char *)NULL)
    FREE(sciGetCallback(pthis));

  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis); 
  return 0;
}



/**DestroyScrollV
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyScrollV (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer la figure parente */
  return 0;
}



/**DestroyScrollH
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyScrollH (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer la figure parente */
  return 0;
}


/**DestroyText
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyText (sciPointObj * pthis)
{
  FREE (pTEXT_FEATURE (pthis)->ptextstring);
  FREE (pTEXT_FEATURE (pthis)->user_data);
  pTEXT_FEATURE (pthis)->size_of_user_data = 0;
  
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}




/**DestroyTitle
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyTitle (sciPointObj * pthis)
{
  FREE (pTITLE_FEATURE (pthis)->text.ptextstring);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}




/**DestroyLegend
 * @memo This function destroies legend and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyLegend (sciPointObj * pthis)
{
  sciLegend * ppLegend = pLEGEND_FEATURE (pthis) ;
  FREE(ppLegend->pptabofpointobj);
  FREE (ppLegend->text.ptextstring);
  FREE (ppLegend->user_data);
  ppLegend->size_of_user_data = 0;

  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**DestroyPolyline
 * @memo This function destroies Polyline and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyPolyline (sciPointObj * pthis)
{
  FREE (pPOLYLINE_FEATURE (pthis)->pvx);
  FREE (pPOLYLINE_FEATURE (pthis)->pvy);
  FREE (pPOLYLINE_FEATURE (pthis)->user_data);
  pPOLYLINE_FEATURE (pthis)->size_of_user_data = 0;

  if (pPOLYLINE_FEATURE (pthis)->pvz != NULL) /**DJ.Abdemouche 2003**/
    FREE (pPOLYLINE_FEATURE (pthis)->pvz);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}


/**DestroyArc
 * @memo This function destroies Pacthc and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyArc (sciPointObj * pthis)
{
  FREE (pARC_FEATURE (pthis)->user_data);
  pARC_FEATURE (pthis)->size_of_user_data = 0;
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE(pARC_FEATURE (pthis)->callback);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**DestroyRectangle
 * @memo This function destroies Rectangle and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyRectangle (sciPointObj * pthis)
{
  FREE (pRECTANGLE_FEATURE (pthis)->user_data);
  pRECTANGLE_FEATURE (pthis)->size_of_user_data = 0;
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**DestroySurface
 * @memo This function destroies surface and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroySurface (sciPointObj * pthis)
{
  sciPointObj *psubwin, *pobj;
  sciSons *psonstmp;
  integer cmpt;
  
  psubwin = (sciPointObj *) sciGetParentSubwin(pthis);
  FREE (pSURFACE_FEATURE (pthis)->user_data);
  pSURFACE_FEATURE (pthis)->size_of_user_data = 0;
  FREE(pSURFACE_FEATURE (pthis)->pvecz);
  FREE(pSURFACE_FEATURE (pthis)->pvecy);
  FREE(pSURFACE_FEATURE (pthis)->pvecx);
  FREE(pSURFACE_FEATURE (pthis)->inputCMoV); /* Adding F.Leray 24.03.04*/
  FREE(pSURFACE_FEATURE (pthis)->color); /* Adding F.Leray 18.03.05 */
  
  if (pSURFACE_FEATURE (pthis)->izcol != 0 ) 
    FREE(pSURFACE_FEATURE (pthis)->zcol);
  pSUBWIN_FEATURE (psubwin)->surfcounter--;
  /* DJ.A 2003 */
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;

  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  cmpt=0;
  psonstmp = sciGetSons (psubwin);
  while (psonstmp != (sciSons *) NULL)	
    {   
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SURFACE) 
	cmpt=cmpt+1;
      psonstmp = psonstmp->pnext;
    }
  if (cmpt < 2){
    if ((pobj= sciGetMerge(psubwin)) != (sciPointObj *) NULL)
      DestroyMerge(pobj); 
  }
  else
    Merge3d(psubwin);
  /* on peut alors detruire l'entite merge */

  return 0;
}


int DestroyMerge (sciPointObj * pthis)
{
  pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->facetmerge = FALSE;
  FREE (pMERGE_FEATURE (pthis)->user_data);
  pMERGE_FEATURE (pthis)->size_of_user_data = 0;
  FREE(pMERGE_FEATURE (pthis)->index_in_entity);
  FREE(pMERGE_FEATURE (pthis)->from_entity);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  return 0;
}



/**DestroyGrayplot
 * @memo This function destroies Grayplot and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 * Djalel ABDEMOUCHE
 */
int
DestroyGrayplot (sciPointObj * pthis)
{
  FREE (pGRAYPLOT_FEATURE (pthis)->pvecx);
  FREE (pGRAYPLOT_FEATURE (pthis)->user_data);
  pGRAYPLOT_FEATURE (pthis)->size_of_user_data = 0;
  if (pGRAYPLOT_FEATURE (pthis)->type != 2)
    FREE (pGRAYPLOT_FEATURE (pthis)->pvecy);
  FREE (pGRAYPLOT_FEATURE (pthis)->pvecz);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}


/**DestroyAxes
 * @memo This function destroies axes and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
int
DestroyAxes (sciPointObj * pthis)
{
  int i;
  char **str;

  FREE (pAXES_FEATURE (pthis)->user_data);
  pAXES_FEATURE (pthis)->size_of_user_data = 0;
  
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (pAXES_FEATURE(pthis)->vx);
  FREE (pAXES_FEATURE(pthis)->vy);
  str=pAXES_FEATURE(pthis)->str;
  for (i=Max(pAXES_FEATURE(pthis)->nx,pAXES_FEATURE(pthis)->ny)-1;i<0;i--) 
    FREE (pAXES_FEATURE(pthis)->str); 
  FREE (pAXES_FEATURE(pthis)->str);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}

/**DestroyGrayplot
 * @memo This function destroies Grayplot and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 * @author Djalel ABDEMOUCHE
 * @version 0.1
 * @see 
 */
int
DestroyFec (sciPointObj * pthis)
{
  FREE (pFEC_FEATURE (pthis)->user_data);
  pFEC_FEATURE (pthis)->size_of_user_data = 0;
  FREE (pFEC_FEATURE (pthis)->pvecx);
  FREE (pFEC_FEATURE (pthis)->pvecy);
  FREE (pFEC_FEATURE (pthis)->pnoeud); 
  FREE (pFEC_FEATURE (pthis)->pfun); 
  FREE (pFEC_FEATURE (pthis)->zminmax);  
  FREE (pFEC_FEATURE (pthis)->colminmax);
  FREE (pFEC_FEATURE (pthis)->colout);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}


/**DestroySegments
 * @memo This function destroies Grayplot and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 * @author Djalel ABDEMOUCHE
 * @version 0.1
 * @see 
 */
int
DestroySegs (sciPointObj * pthis)
{  
  
  FREE (pSEGS_FEATURE (pthis)->user_data);
  pSEGS_FEATURE (pthis)->size_of_user_data = 0;
  FREE (pSEGS_FEATURE (pthis)->vx);
  FREE (pSEGS_FEATURE (pthis)->vy); 
  if (pSEGS_FEATURE (pthis)->vz != (double *)NULL) 
    FREE (pSEGS_FEATURE (pthis)->vz);  
  if (pSEGS_FEATURE (pthis)->ptype <=0) 
    {
      FREE(pSEGS_FEATURE (pthis)->pstyle);
    } 
  else 
    {
      FREE(pSEGS_FEATURE (pthis)->vfx); pSEGS_FEATURE (pthis)->vfx = NULL;
      FREE(pSEGS_FEATURE (pthis)->vfy); pSEGS_FEATURE (pthis)->vfy = NULL;
      FREE(pSEGS_FEATURE (pthis)->vfz); pSEGS_FEATURE (pthis)->vfz = NULL;
    } 
  sciDelThisToItsParent (pthis, sciGetParent (pthis)); 
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}

/**DestroyCompound
 * @memo This function destroies the Compound and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
int
DestroyCompound (sciPointObj * pthis)
{
  sciAgreg * ppagreg = pAGREG_FEATURE (pthis);

  FREE (ppagreg->user_data);
  pAGREG_FEATURE (pthis)->size_of_user_data = 0;

  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1) return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  return 0;
}

void DeleteObjs(integer win_num)
{
  sciPointObj *figure;
  struct BCG *Xgc;

  figure = (sciPointObj *)  sciIsExistingFigure(&win_num);
  if (  figure != (sciPointObj *) NULL )
    {
      Xgc = (struct BCG *) pFIGURE_FEATURE(figure)->pScilabXgc;
      DestroyAllGraphicsSons (figure);
      DestroyFigure (figure);
      Xgc->mafigure = (sciPointObj *) NULL;
    }
}


/**sciUnCompound
 * @memo This function destroies the Compound and  and unpackes the elementaries structures to associates them to its parent
 */
int
sciUnCompound (sciPointObj * pobj)
{
  sciPointObj *pparent, *pobjson;
  sciSons *psons = (sciSons *)NULL;
	
  if (sciGetEntityType(pobj) != SCI_AGREG)
    return -1;

  psons = sciGetLastSons(pobj);
  pobjson = psons->pointobj;
  pparent = sciGetParent(pobj);
  while ((psons != (sciSons *) NULL) && (pobjson != (sciPointObj *) NULL))
    {
      /* we delete this son to this */ 
      pobjson = psons->pointobj;
      /* take the previous sons before the current is freed */
      psons = psons->pprev;
      sciDelThisToItsParent (pobjson, pobj);
      /* and link to its old parent */
      sciAddThisToItsParent (pobjson, pparent);
    }

  sciSetCurrentObj(pparent); /* pparent is the new current object */

  DestroyCompound(pobj);
  return 0;
}


/**DestroyTitle
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int DestroyLabel (sciPointObj * pthis)
{
  sciLabel * ppLabel = pLABEL_FEATURE (pthis);
  FREE (ppLabel->user_data); 
  ppLabel->size_of_user_data = 0;
  FREE (ppLabel->text.ptextstring);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}

int DestroyUimenu (sciPointObj * pthis)
{
  FREE (pUIMENU_FEATURE (pthis)->label.ptextstring);
  FREE (pUIMENU_FEATURE (pthis)->label.callback);

  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1) return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**del sciDelLabelsMenu
 * @memo This function destroies ALL LABELS menu and the elementaries structures
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
sciDelLabelsMenu (sciPointObj * pthis)
{
  sciLabelMenu *pscilabelmenutmp;
  sciLabelMenu *pscilabelmenutofree;

  switch (sciGetEntityType (pthis))
    {
    case SCI_MENU:
      pscilabelmenutmp = (sciLabelMenu *) (pMENU_FEATURE (pthis)->plabelmenu);
      break;
    case SCI_MENUCONTEXT:
      pscilabelmenutmp =
	(sciLabelMenu *) (pMENUCONTEXT_FEATURE (pthis)->plabelmenu);
      break;
    default:
    case SCI_AGREG:
      return -1;
      break;
    }
  while (pscilabelmenutmp != (sciLabelMenu *) NULL)
    {
      /* Il faut retirer tous les menus lies par la liste aussi */
      pscilabelmenutofree = pscilabelmenutmp;
      pscilabelmenutmp = pscilabelmenutmp->pnextlabelmenu;
      FREE (pscilabelmenutofree->plabel);
      FREE (pscilabelmenutofree);
    }
  return 0;
}



/**DestroyMenuContext
 * @memo This function destroies menu context and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyMenuContext (sciPointObj * pthis)
{
  sciDelLabelsMenu (pthis);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}



/**DestroySciMenu
 * @memo This function destroies menu and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroySciMenu (sciPointObj * pthis)
{
  sciDelLabelsMenu (pthis);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}



/**delete_sgwin_entities(int win_num)
 * @memo This function is to be called after window deletion 
 */
void delete_sgwin_entities(int win_num,int v_flag)
{
  double dv=0;
  double *XGC;
  struct BCG *CurrentScilabXgc; 
  int v=0;

  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;

  if(v_flag == 0)
    {
      /* Need to reset the new current figure returned by sciGetCurrentFigure */
      sciHandleTab *hdl = NULL;
      sciPointObj  *pobj= NULL;
      
      hdl = sciGetpendofhandletab();
		 
      if(CurrentScilabXgc != NULL)
	while (hdl != NULL)
	  {
	    pobj=(sciPointObj *) sciGetPointerFromHandle (hdl->index);
	    if (sciGetEntityType(pobj) == SCI_FIGURE && sciGetNum(pobj) == CurrentScilabXgc->CurWindow ) /* Adding F.Leray 19.04.04 */
	      {
		sciSetCurrentFigure(pobj);
		/* sciGetScilabXgc (pobj)->CWindow */
		/* cur =  sciGetScilabXgc (pobj)->CWindow;*/
		/* to force a reset in the graphic scales : COPY from Actions.c line 237 */
		/* SwitchWindow(&cur);*/
		/*C2F(dr)("xset","window",&(pFIGURE_FEATURE(pobj)->number),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L);*/
		/*sciDrawObj(pobj);*/
		/*sciSetSelectedSubWin((sciPointObj *) sciGetSelectedSubWin
		  (pobj));*/
		sciSetCurrentObj(pobj); /* The current object will always be the figure too. */
		break;
		
	      }
	    hdl = hdl->pprev;
	  }
    }
}


