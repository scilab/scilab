/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
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
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to BUILD new objects : 
 - break the binding between the deleted object and its parent in the 
 existing hierarchy
 - freeing memory
 --------------------------------------------------------------------------*/

#include "math_graphics.h"
#include "DestroyObjects.h"
#include "DestroyUicontrol.h"
#include "DestroyUimenu.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "SetProperty.h"
#include "Interaction.h" /* for callback funtions */
#include "StringMatrix.h"
#include "WindowList.h"
#include "sciprint.h"
#include "InitObjects.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "BuildDrawingObserver.h"
#include "DrawingBridge.h"
#include "GraphicSynchronizerInterface.h"
#include "SetUiobjectTag.h"
#include "get_ticks_utils.h"
#include "BuildObjects.h"
#include "HandleManagement.h"
#include "freeArrayOfString.h"

//#include "../../../tclsci/includes/GedManagement.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

/*----------------------------------------------------------------------------*/

/**
 * Destroy only the sons of an object.
 * Does not delete the object itself.
 */
int destroyGraphicsSons(sciPointObj * pthis)
{
  /* to destroy only the sons put the while into the switch !*/
  sciSons * toto = NULL;

  toto = sciGetSons (pthis);
  while ((toto != NULL) && (toto->pointobj != NULL))
  {
    destroyGraphicHierarchy(toto->pointobj);
    toto = sciGetSons(pthis);
  }
  return 0;
}

/********************* modifie le 01/02/2002 ************************
 * On detruit pas la sous fenetre, elle est initialiser avec la figure
 * pour cette version, on considere qu'il y'a 1 seule sous fenetre et 
 * elle suit la fenetre principale (voir clf() ), la fenetre n'est pas 
 * consideree comme un des fils.  
 */
 /**
  * This function destroies childs and pthis
  * @param sciPointObj * pthis: the pointer to the entity
  */
int destroyGraphicHierarchy(sciPointObj * pthis)
{
  destroyGraphicsSons(pthis);
  switch (sciGetEntityType (pthis))
    {

    case SCI_FIGURE:
      DestroyFigure(pthis);
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
    case SCI_AXES:
      DestroyAxes (pthis);
      return 0;
      break;
    case SCI_AGREG:
      DestroyCompound (pthis);
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
    case SCI_UICONTROL:
      DestroyUicontrol (pthis);
      return 0;
      break;
    default:
      sciprint (_("Entity with type %d cannot be destroyed.\n"),sciGetEntityType (pthis));
      return -1;
      break;
    }

  return 0;
}



/**sciDelGraphicObj
 * This function delete only users graphics object and its dependency limited to 
 * SCI_TITLE SCI_LEGEND SCI_ARC SCI_POLYLINE SCI_RECTANGLE SCI_SURFACE 
 * SCI_AXIS SCI_SUBWIN
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
sciDelGraphicObj (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
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
    case SCI_TEXT:
    case SCI_LABEL:
    case SCI_FIGURE:
			destroyGraphicHierarchy (pthis);
      return 0;
		case SCI_SUBWIN:
			{
				/* Special case here since for now there should be always one subwindow */
				/* Inside a figure */
				sciPointObj * parentFigure = sciGetParentFigure(pthis);
				destroyGraphicHierarchy (pthis);
				
				createFirstSubwin(parentFigure);
				return 0;
			}
    default:
      sciprint(_("This object cannot be deleted.\n"));
      return -1;
    }
}

/*-----------------------------------------------------------------------------*/
/**
 * erase a graphic window if necessary.
 */
int C2F(scigerase)( void )
{
  if ( sciGetIsAutoDrawable(sciGetCurrentFigure()) )
  {
    sciClearFigure(sciGetCurrentFigure());
  }
  return 0;
} 
/*-----------------------------------------------------------------------------*/

/**DestroyFigure
 * This function destroys the parents window (manager) and the elementary structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int DestroyFigure (sciPointObj * pthis)
{
  // remove the figure if it is in the list
  removeFigureFromList(pthis);
  
  if (sciIsCurrentFigure(pthis) )
  {

    /* destroyed figure is current one */
    sciSetCurrentFigure(getFirstFigure()) ;
  }

  sciSetIsEventHandlerEnable(pthis, FALSE ) ;
  
  if (pFIGURE_FEATURE(pthis)->eventHandler != NULL)
  {
    FREE( pFIGURE_FEATURE(pthis)->eventHandler ) ;
  }
  
  if (pFIGURE_FEATURE(pthis)->name != NULL)
  {
    FREE( pFIGURE_FEATURE(pthis)->name ) ;
  }
  
	destroyUiobjectTag(pthis);
  destroyFigureModelData(pFIGURE_FEATURE(pthis)->pModelData) ;
  pFIGURE_FEATURE(pthis)->pModelData = NULL;
  sciStandardDestroyOperations(pthis) ;
  return 0;
}



/**DestroySubWin
 * @memo This function destroies the Subwindow (the Axes) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
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
  
  if ( sciGetCallback(pthis) != (char *)NULL)
	{
    FREE(sciGetCallback(pthis));
	}

	return sciStandardDestroyOperations(pthis) ;

}






/**
 * free the structure used by the text object but does not remove the relationship links.
 */
int deallocateText( sciPointObj * pthis )
{
  deleteMatrix( pTEXT_FEATURE(pthis)->pStrings ) ;
  FREE (pTEXT_FEATURE (pthis)->user_data);
  pTEXT_FEATURE (pthis)->size_of_user_data = 0;
  destroyHandleDrawer(pthis);

	destroyRelationShip(pthis);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);

  return 0;
}

/**DestroyText
 * This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int DestroyText (sciPointObj * pthis)
{
  deleteMatrix( pTEXT_FEATURE(pthis)->pStrings ) ;
  return sciStandardDestroyOperations(pthis) ;
}


/**DestroyLegend
 * @memo This function destroies legend and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyLegend (sciPointObj * pthis)
{
  sciLegend * ppLegend = pLEGEND_FEATURE (pthis) ;
  FREE ( ppLegend->tabofhandles );
  deleteMatrix( ppLegend->text.pStrings ) ;

  return sciStandardDestroyOperations(pthis) ;
  /* on peut alors destroyer le parent */
}

int deallocatePolyline (sciPointObj * pthis)
{
  FREE (pPOLYLINE_FEATURE (pthis)->pvx);
  FREE (pPOLYLINE_FEATURE (pthis)->pvy);

  if (pPOLYLINE_FEATURE (pthis)->pvz != NULL) /**DJ.Abdemouche 2003**/
  {
    FREE (pPOLYLINE_FEATURE (pthis)->pvz);
  }
  FREE (pPOLYLINE_FEATURE (pthis)->user_data);
  pPOLYLINE_FEATURE (pthis)->size_of_user_data = 0;
  destroyHandleDrawer(pthis);

	destroyRelationShip(pthis);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
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

  if (pPOLYLINE_FEATURE (pthis)->pvz != NULL) /**DJ.Abdemouche 2003**/
    FREE (pPOLYLINE_FEATURE (pthis)->pvz);
  return sciStandardDestroyOperations(pthis) ;
}


/**DestroyArc
 * @memo This function destroies Pacthc and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyArc (sciPointObj * pthis)
{
  FREE(pARC_FEATURE (pthis)->callback);
  return sciStandardDestroyOperations(pthis) ;
}



/**DestroyRectangle
 * @memo This function destroies Rectangle and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyRectangle (sciPointObj * pthis)
{
  return sciStandardDestroyOperations(pthis) ;
}



/**DestroySurface
 * @memo This function destroies surface and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroySurface (sciPointObj * pthis)
{
  sciPointObj * psubwin ;
  sciSubWindow * ppSubWin ;
  sciSurface * ppSurface = pSURFACE_FEATURE (pthis) ;
  int res = -1 ;
  
  psubwin  = sciGetParentSubwin(pthis) ;
  ppSubWin = pSUBWIN_FEATURE ( psubwin ) ;

  FREE(ppSurface->pvecz);
  FREE(ppSurface->pvecy);
  FREE(ppSurface->pvecx);
  FREE(ppSurface->inputCMoV); /* Adding F.Leray 24.03.04*/
  FREE(ppSurface->color); /* Adding F.Leray 18.03.05 */
  
  if ( ppSurface->izcol != 0 )
  { 
    FREE(ppSurface->zcol);
  }
  /* DJ.A 2003 */
  res = sciStandardDestroyOperations(pthis) ;


  return res;
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
  if (pGRAYPLOT_FEATURE (pthis)->type != 2)
    FREE (pGRAYPLOT_FEATURE (pthis)->pvecy);
  FREE (pGRAYPLOT_FEATURE (pthis)->pvecz);
  return sciStandardDestroyOperations(pthis) ;

}


/**DestroyAxes
 * @memo This function destroies axes and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
int
DestroyAxes (sciPointObj * pthis)
{
  int i;
  char **str;

  FREE (pAXES_FEATURE(pthis)->vx);
  FREE (pAXES_FEATURE(pthis)->vy);
  str=pAXES_FEATURE(pthis)->str;
  for (i=Max(pAXES_FEATURE(pthis)->nx,pAXES_FEATURE(pthis)->ny)-1;i<0;i--) 
    FREE (pAXES_FEATURE(pthis)->str); 
  FREE (pAXES_FEATURE(pthis)->str);
  return sciStandardDestroyOperations(pthis) ;
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
  FREE (pFEC_FEATURE (pthis)->pvecx);
  FREE (pFEC_FEATURE (pthis)->pvecy);
  FREE (pFEC_FEATURE (pthis)->pnoeud); 
  FREE (pFEC_FEATURE (pthis)->pfun); 
  return sciStandardDestroyOperations(pthis) ;
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
  return sciStandardDestroyOperations(pthis) ;
}

/**DestroyCompound
 * @memo This function destroy the Compound and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
int DestroyCompound (sciPointObj * pthis)
{
  return sciStandardDestroyOperations(pthis) ;
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
  int textStatus = -1 ;
  deleteObservers( pthis ) ;
  destroyHandleDrawer( pthis ) ;
  sciUnselectSons( pthis ) ;
  sciDelThisToItsParent( pthis, sciGetParent(pthis) ) ;
  if ( sciDelHandle(pthis) == -1 ) { return -1 ; }
  textStatus = deallocateText( ppLabel->text ) ;
  if ( textStatus != 0 )
  {
    return textStatus ;
  }
  ppLabel->text = NULL ;
	destroyRelationShip(pthis);
  FREE(ppLabel) ;
  FREE(pthis) ;
  return 0 ;
}

/*---------------------------------------------------------------------------*/
/* free the user_data */
void clearUserData( sciPointObj * pObj )
{
  int ** pUserData ;
  int *  pSizeUD   ;
  sciGetPointerToUserData( pObj, &pUserData, &pSizeUD ) ;
  if ( pUserData != NULL && *pUserData != NULL )
  {
    FREE( *pUserData ) ;
    *pUserData = NULL ;
    *pSizeUD = 0 ;
  }
}
/*---------------------------------------------------------------------------*/
/**
 * Close the figure with number winNum.
 */
void sciDeleteWindow( int winNum )
{
  sciPointObj * deletedFig = getFigureFromIndex(winNum);
  startGraphicDataWriting();
  sciDelGraphicObj(deletedFig);
  endGraphicDataWriting();
}
/*--------------------------------------------------------------------------------*/
void AllGraphWinDelete( void )
{

  int iflag=0 ;
  int num = sciGetNbFigure() ;
  int *ArrayWGraph = NULL ;

  /* sciGetIdFigure (ArrayWGraph,&num,&iflag); */

  if (num > 0)
  {
    int i=0;
    ArrayWGraph=(int*)MALLOC(sizeof(int)*num);

    iflag = 1;
    sciGetFiguresId( ArrayWGraph ) ;

    for (i=0;i<num;i++)
    {
      sciDeleteWindow(ArrayWGraph[i]);
    }
    FREE (ArrayWGraph);
    ArrayWGraph=NULL;
  }
}
/*--------------------------------------------------------------------------------*/
/**
 * Recurrent destroying actions for objects
 */
int sciStandardDestroyOperations( sciPointObj * pThis )
{
  int res = 0 ;

  /* Update current object if needed */
  if (pThis == sciGetCurrentObj())
  {
    if (sciGetParent(pThis) != NULL)
    {
      sciSetCurrentObj(sciGetParent(pThis));
    }
    else if (getFirstFigure() != NULL)
    {
      sciSetCurrentObj(sciGetCurrentSubWin());
    }
    else
    {
      /* no more object */
      sciSetCurrentObj(NULL);
    }
  }

  deleteObservers( pThis ) ;
  destroyHandleDrawer( pThis ) ;
  clearUserData( pThis ) ;
  sciUnselectSons( pThis ) ;
  sciDelThisToItsParent( pThis, sciGetParent(pThis) ) ;
  if ( sciDelHandle(pThis) == -1 ) { res = -1 ; }
	destroyRelationShip(pThis);
  FREE( pThis->pfeatures ) ;
  FREE( pThis ) ;
  return res ;
}
/*--------------------------------------------------------------------------------*/
/**
 * destroy a pointer or array allocated by MALLOC
 * This function was created since cpp does not
 * seems to allow the use of MALLOC or FREE
 */
void destroyGraphicPointer(void * pointer)
{
  FREE(pointer);
}
/*--------------------------------------------------------------------------------*/
/**
 * destroy an array of C strings.
 * This function was created since cpp does not
 * seems to allow the use of MALLOC or FREE
 */
void destroyGraphicStringArray(char ** strArray, int nbStrings)
{
	freeArrayOfString(strArray,nbStrings);
}
/*--------------------------------------------------------------------------------*/
void destroyRelationShip(sciPointObj * pObj)
{
	/* Don't destroy relationShip for labels since the relationShip of a label
	is contained in (and destroyed with) its text attribute (see ConstructLabel) */
	if (pObj->relationShip != NULL && sciGetEntityType(pObj) != SCI_LABEL)
	{
		FREE(pObj->relationShip);
		pObj->relationShip = NULL;
	}
}
/*--------------------------------------------------------------------------------*/
