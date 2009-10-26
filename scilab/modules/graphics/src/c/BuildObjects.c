/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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
 - allocating memory
 - setting default value
 - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/
#include <string.h>

#include "BuildObjects.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "CloneObjects.h"
#include "StringMatrix.h"
#include "Scierror.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "BuildDrawingObserver.h"
#include "WindowList.h"
#include "localization.h"
#include "GraphicSynchronizerInterface.h"
#include "Interaction.h"
#include "ColorMapManagement.h"
#include "get_ticks_utils.h"
#include "HandleManagement.h"
#include "loadTextRenderingAPI.h"

#include "MALLOC.h" /* MALLOC */
#include "Scierror.h"

/*-----------------------------------------------------------------------------*/

/**ConstructFigure
 * This function creates the parents window (manager) and the elementaries structures
 * The function is graphic thread safe.
 * @param figureIndex if NULL then a default value is chosen, otherwise use the pointed integer.
 */
/************ 18/01/2002 ***********/
sciPointObj * ConstructFigure(sciPointObj * pparent, int * figureIndex)
{

  sciPointObj *pobj = (sciPointObj *) NULL;
  int x[2];
  sciPointObj * pfiguremdl = getFigureModel() ;
  sciFigure   * ppFigure = NULL ;
  sciFigure   * ppModel  = pFIGURE_FEATURE(pfiguremdl) ;

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

	/* Create the default relationShip */
	createDefaultRelationShip(pobj);

  /* add a number in the HandelTable  reallocation de la table + 1 puis affectation de l'indice    */

  ppFigure = pFIGURE_FEATURE(pobj) ;

  /* No synchronization is needed here because nobody knows about the figure */
  /* until we add it to the lists */

  /* Don't call standard build operation since it add the figure to the handle list */

  sciInitVisibility( pobj, TRUE ) ;

	/* Clone user data from parent */
  initUserData(pobj);
	cloneUserData(pfiguremdl, pobj);


  pobj->pObservers = DoublyLinkedList_new() ;
  createDrawingObserver( pobj ) ;


  pobj->pDrawer = NULL ;


  /* initialisation de context et mode graphique par defaut (figure model)*/
  if (sciInitGraphicContext (pobj) == -1)
    {
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  if (sciInitGraphicMode (pobj) == -1)
    {
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

  /* F.Leray 08.04.04 */
  if (sciInitFontContext (pobj) == -1)
    {
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  if (figureIndex != NULL)
  {
    sciInitNum(pobj, *figureIndex);
  }
  else
  {
    /* Set default figure index */
    sciInitNum(pobj, getUnusedFigureIndex());
  }

  sciInitName(pobj, sciGetName(pfiguremdl));
  sciInitResize(pobj,sciGetResize(pfiguremdl));

  ppFigure->isselected = ppModel->isselected;
  ppFigure->rotstyle = ppModel->rotstyle;
  ppFigure->visible = ppModel->visible;
  sciInitImmediateDrawingMode(pobj, sciGetImmediateDrawingMode(pfiguremdl));

  ppFigure->numsubwinselected = ppModel->numsubwinselected;
  ppFigure->pixmapMode = ppModel->pixmapMode ;
  ppFigure->allredraw = ppModel->allredraw;
  ppFigure->pModelData = NULL;

  ppFigure->eventHandler = NULL ;
  sciInitEventHandler( pobj, sciGetEventHandler( pfiguremdl ) ) ;
  sciInitIsEventHandlerEnable( pobj, sciGetIsEventHandlerEnable( pfiguremdl ) ) ;

  sciInitWindowDim(pobj, sciGetWindowWidth(pfiguremdl), sciGetWindowHeight(pfiguremdl) ) ;
  /* Set axes_size after to be sure to have correct values */
  if (sciInitDimension(pobj, sciGetWidth(pfiguremdl), sciGetHeight(pfiguremdl)) != RESIZE_SUCCESS)
  {
    FREE(pobj->pfeatures);
    FREE(pobj);
    return (sciPointObj *) NULL;
  }

  sciGetScreenPosition(pfiguremdl, &x[0], &x[1]) ;
	if (x[0] != -1 || x[1] != -1)
	{
		/* If default position is [-1,-1], then let the OS choose the window position. */
		sciInitScreenPosition( pobj, x[0], x[1] );
	}

	sciInitInfoMessage( pobj, ppModel->pModelData->infoMessage ) ;

  ppFigure->tag = NULL;

  sciInitPixmapMode(pobj, sciGetPixmapMode(pfiguremdl));

	sciInitAntialiasingQuality(pobj, sciGetAntialiasingQuality(pfiguremdl));

  /* Colormap */
  sciInitNumColors(pobj, 0);
  sciSetDefaultColorMap(pobj);

  /* for java */
  sciInitBackground(pobj, sciGetBackground(pfiguremdl));

  /* Add the figure in the list of created figures */
  /* Here we need to synchronize */
  startGraphicDataWriting();
  /* add the handle in the handle list */
  if ( sciAddNewHandle(pobj) == -1 )
  {
    FREE(pobj->pfeatures);
    FREE(pobj);
    return NULL ;
  }
  addNewFigureToList(pobj);
  endGraphicDataWriting();

  return pobj;
}







/**ConstructSubWin
 * This function creates the Subwindow (the Axe) and the elementaries structures
 */
sciPointObj *
ConstructSubWin(sciPointObj * pparentfigure)
{

	char logFlags[3];
	int i;
	char dir;
	BOOL autoTicks[3];
	sciPointObj *pobj = (sciPointObj *) NULL;
	sciSubWindow * ppsubwin = NULL;
	sciPointObj * paxesmdl = getAxesModel() ;
	sciSubWindow * ppaxesmdl = pSUBWIN_FEATURE (paxesmdl);

	if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
	{

		if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
			return NULL;
		sciSetEntityType (pobj, SCI_SUBWIN);
		if ((pobj->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
		{
			FREE(pobj);
			return (sciPointObj *) NULL;
		}

		startFigureDataWriting(pparentfigure);

		if ( sciStandardBuildOperations( pobj, pparentfigure ) == NULL )
		{
			FREE( pobj->pfeatures ) ;
			FREE( pobj ) ;
			endFigureDataWriting(pparentfigure);
			return NULL ;
		}

		ppsubwin =  pSUBWIN_FEATURE (pobj); /* debug */


		ppsubwin->callback = (char *)NULL;
		ppsubwin->callbacklen = 0;
		ppsubwin->callbackevent = 100;

		if (sciInitGraphicContext (pobj) == -1)
		{
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}
		if (sciInitGraphicMode (pobj) == -1)
		{
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		/* F.Leray 08.04.04 */
		if (sciInitFontContext (pobj) == -1)
		{
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		/* update colors so they will fit the colormap of parent figure and not model one*/
		sciRecursiveUpdateBaW(pobj, sciGetNumColors(getFigureModel()), sciGetNumColors(sciGetParentFigure(pobj)));

		sciGetLogFlags(paxesmdl, logFlags);
		sciInitLogFlags(pobj, logFlags);

		ppsubwin->axes.ticscolor  = ppaxesmdl->axes.ticscolor;
		ppsubwin->axes.subint[0]  = ppaxesmdl->axes.subint[0];
		ppsubwin->axes.subint[1]  = ppaxesmdl->axes.subint[1];
		ppsubwin->axes.subint[2]  = ppaxesmdl->axes.subint[2];

		dir= ppaxesmdl->axes.xdir;
		ppsubwin->axes.xdir = dir;
		dir= ppaxesmdl->axes.ydir;
		ppsubwin->axes.ydir = dir;

		ppsubwin->axes.rect  = ppaxesmdl->axes.rect;
		sciInitIsFilled(pobj, sciGetIsFilled(paxesmdl));
		for (i=0 ; i<7 ; i++)
		{
			ppsubwin->axes.limits[i]  = ppaxesmdl->axes.limits[i] ;
		}

		for (i=0 ; i<3 ; i++)
		{
			ppsubwin->grid[i]  = ppaxesmdl->grid[i] ;
		}

		ppsubwin->gridFront = ppaxesmdl->gridFront;

		ppsubwin->alpha  = ppaxesmdl->alpha;
		ppsubwin->theta  = ppaxesmdl->theta;
		ppsubwin->alpha_kp  = ppaxesmdl->alpha_kp;
		ppsubwin->theta_kp  = ppaxesmdl->theta_kp;
		ppsubwin->is3d  = ppaxesmdl->is3d;

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

		sciGetAutoTicks(paxesmdl, autoTicks);
		sciInitAutoTicks(pobj, autoTicks[0], autoTicks[1], autoTicks[2]);
		/* end 22.09.04 */

		ppsubwin->axes.flag[0]= ppaxesmdl->axes.flag[0];
		ppsubwin->axes.flag[1]= ppaxesmdl->axes.flag[1];
		ppsubwin->axes.flag[2]= ppaxesmdl->axes.flag[2];
		sciInitHiddenAxisColor(pobj, sciGetHiddenAxisColor(paxesmdl));
		ppsubwin->project[0]= ppaxesmdl->project[0];
		ppsubwin->project[1]= ppaxesmdl->project[1];
		ppsubwin->project[2]= ppaxesmdl->project[2];
		sciInitHiddenColor(pobj, sciGetHiddenColor(paxesmdl));
		ppsubwin->isoview= ppaxesmdl->isoview;
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

		ppsubwin->visible = ppaxesmdl->visible;

		ppsubwin->clip_region_set = 0 ;
		sciInitIsClipping( pobj, sciGetIsClipping(paxesmdl) ) ;
		sciSetClipping(   pobj, sciGetClipping(  paxesmdl) ) ;

		ppsubwin->cube_scaling = ppaxesmdl->cube_scaling;

		ppsubwin->SRect[0]  =  ppaxesmdl->SRect[0];
		ppsubwin->SRect[1]  =  ppaxesmdl->SRect[1];
		ppsubwin->SRect[2]  =  ppaxesmdl->SRect[2];
		ppsubwin->SRect[3]  =  ppaxesmdl->SRect[3];
		ppsubwin->SRect[4]  =  ppaxesmdl->SRect[4];
		ppsubwin->SRect[5]  =  ppaxesmdl->SRect[5];

		ppsubwin->tight_limits = ppaxesmdl->tight_limits;
		ppsubwin->FirstPlot = ppaxesmdl->FirstPlot;

		sciInitUseNurbs(pobj, sciGetUseNurbs(paxesmdl));

		/* clone user data from model */
		cloneUserData(paxesmdl, pobj);

		if (sciInitSelectedSubWin(pobj) < 0 )
		{
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *)NULL ;
		}

		/* Construction des labels: x,y,z et Title */

		if ((ppsubwin->mon_title =  ConstructLabel (pobj, "",1)) == NULL){
			sciDelThisToItsParent (pobj, sciGetParent (pobj)); /* pobj type = scisubwindow*/
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		sciSetStrings( ppsubwin->mon_title,
			sciGetText( pSUBWIN_FEATURE(paxesmdl)->mon_title) ) ;


		/*------------------------------------*/
		if ((ppsubwin->mon_x_label =  ConstructLabel (pobj, "",2)) == NULL){
			DestroyLabel(ppsubwin->mon_title);
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		sciSetStrings( ppsubwin->mon_x_label,
			sciGetText(pSUBWIN_FEATURE(paxesmdl)->mon_x_label) ) ;


		/*------------------------------------*/
		if ((ppsubwin->mon_y_label =  ConstructLabel (pobj, "",3)) == NULL){
			DestroyLabel(ppsubwin->mon_title);
			DestroyLabel(ppsubwin->mon_x_label);
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}
		sciSetStrings( ppsubwin->mon_y_label,
			sciGetText( pSUBWIN_FEATURE(paxesmdl)->mon_y_label) ) ;

		/*------------------------------------*/
		if ((ppsubwin->mon_z_label =  ConstructLabel (pobj, "",4)) == NULL){
			DestroyLabel(ppsubwin->mon_title);
			DestroyLabel(ppsubwin->mon_x_label);
			DestroyLabel(ppsubwin->mon_y_label);
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}
		sciSetStrings( ppsubwin->mon_z_label,
			sciGetText(pSUBWIN_FEATURE(paxesmdl)->mon_z_label)  ) ;

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

		cloneGraphicContext( ppaxesmdl->mon_x_label, ppsubwin->mon_x_label ) ;
		cloneGraphicContext( ppaxesmdl->mon_y_label, ppsubwin->mon_y_label ) ;
		cloneGraphicContext( ppaxesmdl->mon_z_label, ppsubwin->mon_z_label ) ;
		cloneGraphicContext( ppaxesmdl->mon_title  , ppsubwin->mon_title   ) ;

		endFigureDataWriting(pparentfigure);

		return (sciPointObj *)pobj;

	}
	else
	{
		Scierror(999, _("The parent has to be a FIGURE\n"));
		return NULL;
	}
}



/**
 * creates a new text object. However the object is not added in the handle list.
 * Its graphic and font context are also not initialized.
 * this function is to be used with objects including a text object.
 */
sciPointObj * allocateText( sciPointObj       * pparentsubwin,
                            char             ** text         ,
                            int                 nbRow        ,
                            int                 nbCol        ,
                            double              x            ,
                            double              y            ,
                            BOOL                autoSize     ,
                            double              userSize[2]  ,
                            BOOL                centerPos    ,
                            int               * foreground   ,
                            int               * background   ,
                            BOOL                isboxed      ,
                            BOOL                isline       ,
                            BOOL                isfilled     ,
                            sciTextAlignment    align         )
{
  sciPointObj * pObj = NULL ;
  sciText * ppText ;
  int i;
  if ( ( pObj = MALLOC( sizeof(sciPointObj) ) ) == NULL )
  {
    return NULL;
  }

  sciSetEntityType (pObj, SCI_TEXT);

  if ( ( pObj->pfeatures = MALLOC( sizeof (sciText) ) ) == NULL )
  {
    FREE( pObj ) ;
    return NULL;
  }

	/* Create the default relationShip */
	createDefaultRelationShip(pObj);

  ppText = pTEXT_FEATURE( pObj ) ;

  initUserData(pObj);


  /* it must be specified for some functions */
  sciSetParent( pObj, pparentsubwin ) ;

  ppText->callback = (char *)NULL;
  ppText->callbacklen = 0;
  ppText->callbackevent = 100;
  ppText->visible = sciGetVisibility( pparentsubwin );

  ppText->clip_region_set = 0 ;
  sciInitIsClipping( pObj, sciGetIsClipping( pparentsubwin ) ) ;
  sciSetClipping( pObj, sciGetClipping(pparentsubwin) );

  /* Check if we should load LaTex / MathML Java libraries */
  loadTextRenderingAPI(text, nbRow, nbCol);

  /* allocate the matrix */
  ppText->pStrings = newFullStringMatrix( text, nbRow, nbCol ) ;
  if ( ppText->pStrings == NULL )
  {
    FREE(pObj->pfeatures);
    FREE(pObj);
    return NULL ;
  }

  /* initialize position */
  ppText->is3d = TRUE ;
  ppText->x = x;
  ppText->y = y;
  ppText->z = 0.0; /**DJ.Abdemouche 2003**/

  /* initialize to a not too weird value */
  for (i = 0; i < 4; i++)
  {
    ppText->corners[i][0] = ppText->x;
    ppText->corners[i][1] = ppText->y;
    ppText->corners[i][2] = ppText->z;
  }

  ppText->centeredPos = centerPos ;
  ppText->autoSize = autoSize ;

  /* userSize must be specified if the size is given by the user */
  /* or the user specified a rectangle */

  if ( !autoSize || centerPos )
  {
    ppText->userSize[0] = userSize[0] ;
    ppText->userSize[1] = userSize[1] ;
  }
  else
  {
    ppText->userSize[0] = 0.0 ;
    ppText->userSize[1] = 0.0 ;
  }

  ppText->stringsAlign = align ;

  pObj->pDrawer = NULL ;

  if ( sciInitGraphicContext( pObj ) == -1 )
  {
    FREE(pObj->pfeatures);
    FREE(pObj);
    return NULL ;
  }

  if ( sciInitFontContext( pObj ) == -1 )
  {
    FREE(pObj->pfeatures);
    FREE(pObj);
    return NULL ;
  }

  sciInitIsBoxed(pObj,isboxed);
  sciInitIsLine(pObj,isline);
  sciInitIsFilled(pObj,isfilled);

  if ( foreground != NULL )
  {
    sciInitForeground(pObj,(*foreground));
  }

  if ( background != NULL )
  {
    sciInitBackground(pObj,(*background));
  }


  return pObj;
}

/**ConstructText
 * This function creates the parents window (manager) and the elementaries structures
 * @param  sciPointObj *pparentsubwin :
 * @param  char * text[] : intial text matrix string.
 * @param  int nbCol : the number column of the text
 * @param  int nbRow : the number of row of the text
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructText (sciPointObj * pparentsubwin, char ** text, int nbRow, int nbCol, double x,
	       double y, BOOL autoSize, double userSize[2], BOOL centerPos, int *foreground, int *background,
	       BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align )
{
  if ( sciGetEntityType( pparentsubwin ) == SCI_SUBWIN )
  {
    sciPointObj * pobj = allocateText( pparentsubwin, text, nbRow, nbCol, x, y,
                                       autoSize, userSize, centerPos, foreground, background,
                                       isboxed, isline, isfilled, align ) ;

    if ( pobj == NULL )
    {
      return NULL ;
    }

    pobj->pObservers = DoublyLinkedList_new() ;
    createDrawingObserver( pobj ) ;
    pobj->pDrawer = NULL ;

    if (sciAddNewHandle (pobj) == -1)
    {
      deallocateText( pobj ) ;
      return  NULL;
    }

    if ( !(sciAddThisToItsParent (pobj, pparentsubwin)) )
    {
      deleteMatrix( pTEXT_FEATURE( pobj )->pStrings ) ;
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return NULL;
    }

    return pobj ;

  }

  Scierror(999, _("The parent has to be a SUBWIN\n"));
  return NULL;
}


/**constructLegend
 * This function creates  Legend structure
 */
sciPointObj *
ConstructLegend (sciPointObj * pparentsubwin, char **text, long long tabofhandles[], int nblegends)
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
	if (psonstmp != NULL)
	{/* on peut commencer sur le next */
		/* tant que le fils n'est pas une legende */
		while ((psonstmp->pnext != NULL) && sciGetEntityType (psonstmp->pointobj) != SCI_LEGEND)
		{
			psonstmp = psonstmp->pnext;
		}
	}

	if (sciGetEntityType (psonstmp->pointobj) == SCI_LEGEND)
	{
		DestroyLegend (psonstmp->pointobj);
	}

	if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
	{
		if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
		{
			return NULL;
		}
		sciSetEntityType (pobj, SCI_LEGEND);
		if ((pobj->pfeatures = MALLOC ((sizeof (sciLegend)))) == NULL)
		{
			FREE(pobj);
			return (sciPointObj *) NULL;
		}
		/* get the pointer on the features */
		ppLegend = pLEGEND_FEATURE( pobj );


		if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
		{
			FREE( pobj->pfeatures ) ;
			FREE( pobj ) ;
			return NULL ;
		}

		ppLegend->text.callback = (char *)NULL;
		ppLegend->text.callbacklen = 0;
		ppLegend->text.callbackevent = 100;
		ppLegend->text.isboxed = FALSE ;

		ppLegend->visible = sciGetVisibility(sciGetParentSubwin(pobj));

		ppLegend->text.pStrings = newFullStringMatrix( text,nblegends,1 ) ;

		/* Allocation de la structure sciText */
		if ( ppLegend->text.pStrings == NULL)
		{
			Scierror(999, _("No more place to allocates text string, try a shorter string.\n"));
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(ppLegend);
			FREE(pobj);
			return (sciPointObj *) NULL;
		}
		/* on copie le texte du titre dans le champs specifique de l'objet */
		ppLegend->nblegends = nblegends;

		if ((ppLegend->tabofhandles =
			MALLOC(nblegends*sizeof(long long))) == NULL)
		{
			Scierror(999, _("%s: No more memory.\n"),"ConstructLegend");
			deleteMatrix( ppLegend->text.pStrings ) ;
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(ppLegend);
			FREE(pobj);
			return (sciPointObj *) NULL;
		}


		for (i=0; i < nblegends; i++)
		{
 			// Bug 4530: we must reverse the order of the handles
			ppLegend->tabofhandles[i] = tabofhandles[nblegends - 1 - i];
		}

		ppLegend->text.fontcontext.textorientation = 0.0;
		ppLegend->pos.x = 0;
		ppLegend->pos.y = 0;
		ppLegend->width = 0;
		ppLegend->height = 0;
		ppLegend->place = SCI_LEGEND_LOWER_CAPTION; /* Default position */
		ppLegend->isselected = TRUE;
		ppLegend->issurround = FALSE;

		/* no clipping by default */
		ppLegend->clip_region_set = 0 ;
		sciInitIsClipping( pobj, -1 ) ;
		sciSetClipping( pobj, sciGetClipping(pparentsubwin) );

		if (sciInitGraphicContext (pobj) == -1) /* NEW :  used to draw the line and marks of the curve F.Leray 21.01.05 */
		{
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			return (sciPointObj *) NULL;
		}

		/* With legends, force drawing of background */
		/* Otherwise underlying lines can be seen */
		sciSetIsFilled(pobj, TRUE);

		if (sciInitFontContext (pobj) == -1)
		{
			Scierror(999, _("Problem with sciInitFontContext\n"));
			FREE(ppLegend->tabofhandles);
			deleteMatrix( ppLegend->text.pStrings ) ;
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
		Scierror(999, _("The parent has to be a SUBWIN\n"));
		return (sciPointObj *) NULL;
	}
}
/*---------------------------------------------------------------------------------*/
/**
 * Create a polyline but does not add it to Scilab hierarchy
 */
sciPointObj * allocatePolyline(sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
                               int closed, int n1,int plot, int *foreground, int *background,
                               int *mark_style, int *mark_foreground, int *mark_background,
                               BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
  sciPointObj * pobj = NULL;
  sciPolyline * ppPoly = NULL;
  int i = 0;
  if (sciGetEntityType (pparentsubwin) != SCI_SUBWIN)
  {
    Scierror(999, _("The parent has to be a SUBWIN\n"));
    return (sciPointObj *) NULL;
  }

  if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
  {
    return NULL;
  }
  sciSetEntityType (pobj, SCI_POLYLINE);
  if ((pobj->pfeatures = MALLOC ((sizeof (sciPolyline)))) == NULL)
  {
    FREE(pobj);
    return (sciPointObj *) NULL;
  }

	/* Create the default relationShip */
	createDefaultRelationShip(pobj);

  sciSetParent( pobj, pparentsubwin ) ;

  pPOLYLINE_FEATURE (pobj)->x_shift = (double *) NULL;
  pPOLYLINE_FEATURE (pobj)->y_shift = (double *) NULL;
  pPOLYLINE_FEATURE (pobj)->z_shift = (double *) NULL;
  pPOLYLINE_FEATURE (pobj)->bar_width = 0.;

  pPOLYLINE_FEATURE (pobj)->callback = (char *)NULL;
  pPOLYLINE_FEATURE (pobj)->callbacklen = 0;
  pPOLYLINE_FEATURE (pobj)->callbackevent = 100;
  pPOLYLINE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

  pPOLYLINE_FEATURE (pobj)->clip_region_set = 0;
  sciInitIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)) ) ;
  sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));


  pPOLYLINE_FEATURE (pobj)->arsize_factor = 1;

  pPOLYLINE_FEATURE (pobj)->isselected = TRUE;
  ppPoly = pPOLYLINE_FEATURE (pobj);

  if ( n1 != 0 )
  {
    if ((ppPoly->pvx = MALLOC (n1 * sizeof (double))) == NULL)
    {
      FREE(pPOLYLINE_FEATURE(pobj));
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

    if ((ppPoly->pvy = MALLOC (n1 * sizeof (double))) == NULL)
    {
      FREE(pPOLYLINE_FEATURE (pobj)->pvx);
      FREE(pPOLYLINE_FEATURE(pobj));
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

    if ((pvecx != (double *)NULL)&&(pvecy != (double *)NULL))
    {
      for (i = 0; i < n1; i++)
      {
        ppPoly->pvx[i] = pvecx[i] ;
        ppPoly->pvy[i] = pvecy[i] ;
      }
    }
    else
    {
      for (i = 0; i < n1; i++)
      {
        ppPoly->pvx[i] = 0.0;
        ppPoly->pvy[i] = 0.0;
      }
    }


    /**DJ.Abdemouche 2003**/
    if (pvecz == NULL)
    {
      pPOLYLINE_FEATURE (pobj)->pvz = NULL;
    }
    else
    {
      if ((ppPoly->pvz = MALLOC (n1 * sizeof (double))) == NULL)
      {
        FREE(pPOLYLINE_FEATURE (pobj)->pvx);
        FREE(pPOLYLINE_FEATURE (pobj)->pvy);
        FREE(pPOLYLINE_FEATURE(pobj));
        FREE(pobj);
        return (sciPointObj *) NULL;
      }
      for (i = 0; i < n1; i++)
      {
        ppPoly->pvz[i] = pvecz[i];
      }
    }
  }
  else
  {
    ppPoly->pvx = NULL ;
    ppPoly->pvy = NULL ;
    ppPoly->pvz = NULL ;
  }

  ppPoly->n1 = n1;	  /* memorisation du nombre de points */
  ppPoly->closed = (closed > 0) ? 1 : 0;
  ppPoly->plot = plot;

  if (sciInitGraphicContext (pobj) == -1)
  {
    FREE(pPOLYLINE_FEATURE (pobj)->pvy);
    FREE(pPOLYLINE_FEATURE (pobj)->pvx);
    FREE(pPOLYLINE_FEATURE(pobj));
    FREE(pobj);
    return (sciPointObj *) NULL;
  }

  /* colors and marks setting */
  sciInitIsMark(pobj,ismark);
  sciInitIsLine(pobj,isline);
  sciInitIsFilled(pobj,isfilled);
  /*       sciSetIsInterpShaded(pobj,isinterpshaded); */

  ppPoly->isinterpshaded = isinterpshaded; /* set the isinterpshaded mode */

  if(foreground != NULL)
  {
    sciInitForeground(pobj,(*foreground));
  }

  ppPoly->scvector = (int *) NULL;

  if(background != NULL){
    if(isinterpshaded == TRUE)
		{ /* 3 or 4 values to store */

      sciSetInterpVector(pobj,n1,background);
    }
    else
		{
      sciInitBackground(pobj,(*background));
		}
  }

  if(mark_style != NULL)
	{
    sciInitMarkStyle(pobj,(*mark_style));
	}

  if(mark_foreground != NULL)
  {
    sciInitMarkForeground(pobj,(*mark_foreground));
  }

  if(mark_background != NULL)
  {
    sciInitMarkBackground(pobj,(*mark_background));
  }

  /* no sons for now */
  sciInitSelectedSons( pobj ) ;

  sciGetRelationship(pobj)->psons        = NULL ;
  sciGetRelationship(pobj)->plastsons    = NULL ;
  sciGetRelationship(pobj)->pSelectedSon = NULL ;

  sciInitVisibility( pobj, TRUE ) ;

  initUserData(pobj);

  pobj->pObservers = NULL;

  pobj->pDrawer = NULL ;

  return pobj;

}
/*---------------------------------------------------------------------------------*/
/**ConstructPolyline
 * This function creates  Polyline 2d structure
 */
sciPointObj *
ConstructPolyline (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
		   int closed, int n1,int plot, int *foreground, int *background,
		   int *mark_style, int *mark_foreground, int *mark_background,
		   BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
  sciPointObj * pobj = allocatePolyline(pparentsubwin, pvecx, pvecy, pvecz, closed, n1, plot,
                                        foreground, background, mark_style, mark_foreground, mark_background,
                                        isline, isfilled, ismark, isinterpshaded);

  if (pobj == NULL)
  {
    return NULL;
  }

  /* allocatePolyline created a "fake" relationship, destroy it */
  FREE(pobj->relationShip);
  
  if (sciStandardBuildOperations(pobj, pparentsubwin) == NULL)
  {
    FREE(pobj->pfeatures);
    FREE(pobj);
    return NULL;
  }

  return pobj;

}



/**ConstructArc
 * This function creates an Arc structure
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

      if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
      {
        FREE( pobj->pfeatures ) ;
        FREE( pobj ) ;
        return NULL ;
      }

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
      ppArc->visible = sciGetVisibility(pparentsubwin);
      /* By default use nurbs drawing */
      sciInitUseNurbs(pobj, sciGetUseNurbs(pparentsubwin));


      ppArc->clip_region_set = 0;
      /*ppArc->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      sciInitIsClipping( pobj, sciGetIsClipping(pparentsubwin) ) ;
      sciSetClipping(pobj,sciGetClipping(pparentsubwin));
      /*      pARC_FEATURE (pobj)->clip_region = (double *) NULL; */


      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(ppArc);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      sciInitIsFilled(pobj,isfilled);
      /* should be put after graphicContext initialization */
      sciInitIsLine(pobj,isline);

      if(foreground != NULL)
      {
	sciInitForeground(pobj,(*foreground));
      }

      if(background != NULL)
      {
	sciInitBackground(pobj,(*background));
      }

      return pobj;
    }
  else
    {
      Scierror(999, _("The parent has to be a SUBWIN\n"));
      return (sciPointObj *) NULL;
    }
}



/**ConstructRectangle
 * This function creates Rectangle structure and only this to destroy all sons use DelGraphicsSon
 */
sciPointObj *
ConstructRectangle (sciPointObj * pparentsubwin, double x, double y,
		    double height, double width,  int *foreground, int *background,
		    int isfilled, int isline)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if ( height < 0.0 || width < 0.0 )
  {
    Scierror(999,_("Width and height must be positive.\n"));
    return NULL ;
  }

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


      if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
      {
        FREE( pobj->pfeatures ) ;
        FREE( pobj ) ;
        return NULL ;
      }

      pRECTANGLE_FEATURE (pobj)->callback = (char *)NULL;
      pRECTANGLE_FEATURE (pobj)->callbacklen = 0;
      pRECTANGLE_FEATURE (pobj)->callbackevent = 100;


      pRECTANGLE_FEATURE (pobj)->x = x;
      pRECTANGLE_FEATURE (pobj)->y = y;
      pRECTANGLE_FEATURE (pobj)->z = 0.0;
      pRECTANGLE_FEATURE (pobj)->height = height;
      pRECTANGLE_FEATURE (pobj)->width = width;
      pRECTANGLE_FEATURE (pobj)->isselected = TRUE;
      pRECTANGLE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      pRECTANGLE_FEATURE (pobj)->clip_region_set = 0;
      sciInitIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)) ) ;
      sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));


      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pRECTANGLE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      sciInitIsLine(pobj,isline);
      sciInitIsFilled(pobj,isfilled);

      if(foreground != NULL)
	sciInitForeground(pobj,(*foreground));

      if(background != NULL)
	sciInitBackground(pobj,(*background));

      return pobj;
    }
  else
    {
      Scierror(999, _("The parent has to be a SUBWIN\n"));
      return (sciPointObj *) NULL;
    }
}



/**ConstructSurface
 * This function creates Surface Structure
 */
sciPointObj *
ConstructSurface (sciPointObj * pparentsubwin, sciTypeOf3D typeof3d,
		  double * pvecx, double * pvecy, double * pvecz,double *zcol,
		  int izcol, int dimzx, int dimzy,
		  int *flag, double *ebox,int flagcolor,
		  int *isfac, int *m1, int *n1, int *m2,
		  int *n2, int *m3, int *n3, int *m3n, int *n3n)
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

      /*debug F.Leray*/
      psurf = pSURFACE_FEATURE (pobj);
      if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
      {
        FREE( pobj->pfeatures ) ;
        FREE( pobj ) ;
        return NULL ;
      }

      psurf->callback = (char *)NULL;
      psurf->callbacklen = 0;
      psurf->callbackevent = 100;
      psurf->visible = sciGetVisibility(sciGetParentSubwin(pobj));



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

      sciInitIsClipping( pobj, sciGetIsClipping(pparentsubwin) ) ;
      sciSetClipping(pobj, sciGetClipping(pparentsubwin)) ;

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
      sciInitHiddenColor(pobj, sciGetHiddenColor(pparentsubwin));

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
      return pobj;
    }
  else
    {
      Scierror(999, _("The parent has to be a SUBWIN\n"));
      return (sciPointObj *) NULL;
    }
}

/********************** 14/05/2002 *****
 **ConstructGayplot
 * This function creates Grayplot
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

      if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
      {
        FREE( pobj->pfeatures ) ;
        FREE( pobj ) ;
        return NULL ;
      }

      pGRAYPLOT_FEATURE (pobj)->callback = (char *)NULL;
      pGRAYPLOT_FEATURE (pobj)->callbacklen = 0;
      pGRAYPLOT_FEATURE (pobj)->callbackevent = 100;

      pGRAYPLOT_FEATURE (pobj)->isselected = TRUE;
      pGRAYPLOT_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

      pGRAYPLOT_FEATURE (pobj)->type = type;
      pGRAYPLOT_FEATURE (pobj)->pvecx = (double *)NULL;
      pGRAYPLOT_FEATURE (pobj)->pvecy = (double *)NULL;

      sciInitIsClipping( pobj, sciGetIsClipping(pparentsubwin) ) ;
      sciSetClipping(pobj, sciGetClipping(pparentsubwin)) ;

      strcpy( pGRAYPLOT_FEATURE (pobj)->datamapping, "scaled" ) ;
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
      Scierror(999, _("The parent has to be a SUBWIN\n"));
      return (sciPointObj *) NULL;
    }
}





/**ConstructAxes
 * This function creates Axes structure
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

    if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
    {
      FREE( pobj->pfeatures ) ;
      FREE( pobj ) ;
      return NULL ;
    }

    pAXES_FEATURE (pobj)->callback = (char *)NULL;
    pAXES_FEATURE (pobj)->callbacklen = 0;
    pAXES_FEATURE (pobj)->callbackevent = 100;
    pAXES_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

    /*pAXES_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
    pAXES_FEATURE (pobj)->clip_region_set = 0;
    /*pAXES_FEATURE (pobj)->isclip = -1;*/  /*F.Leray Change here: by default Axis are not clipped. 10.03.04 */
    sciInitIsClipping( pobj, -1 ) ;
    sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
    /*       pAXES_FEATURE (pobj)->clip_region = (double *) NULL; */

    pAXES_FEATURE (pobj)->dir =dir;
    pAXES_FEATURE (pobj)->tics =tics;

    paxes = pAXES_FEATURE (pobj);
    /* pour le moment je garde les vecteurs separes, et non en POINT2D */
    if ((paxes->vx = MALLOC (nx * sizeof (double))) == NULL)
	  {
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pAXES_FEATURE(pobj));
	    FREE(pobj);
	    return (sciPointObj *) NULL;
	  }
    if ((paxes->vy = MALLOC (ny * sizeof (double))) == NULL)
	  {
	    FREE(pAXES_FEATURE (pobj)->vx);
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pAXES_FEATURE(pobj));
	    FREE(pobj);
	    return (sciPointObj *) NULL;
	  }

    for (i = 0; i < nx; i++)
	  {
	    paxes->vx[i] = vx[i];
	  }
    for (i = 0; i < ny; i++)
	  {
	    paxes->vy[i] = vy[i];
	  }

    pAXES_FEATURE (pobj)->nx =nx;
    pAXES_FEATURE (pobj)->ny =ny;

    pAXES_FEATURE (pobj)->nb_tics_labels = nb_tics_labels; /* F.Leray 29.04.05 */

    /* pAXES_FEATURE(pobj)->str = str;*/ /* Pb here, F.Leray : Weird init.: can not copy a string using '='*/
    if(str != (char **) NULL)
	  {
	    if(pAXES_FEATURE (pobj)->nb_tics_labels == -1){
	      Scierror(999, _("Impossible case when building axis\n"));
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
    pAXES_FEATURE (pobj)->seg =seg;
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

    if ( sciInitFontContext( pobj ) == -1 )
    {
      sciDelThisToItsParent (pobj, sciGetParent (pobj));
    	sciDelHandle (pobj);
    	FREE(pAXES_FEATURE (pobj));
    	FREE(pobj);
    	return NULL;
    }

    sciInitFontSize(pobj, fontsize);
    sciInitFontForeground(pobj, textcolor);
    sciInitForeground(pobj, ticscolor);

    return pobj;
  }
  else
  {
    Scierror(999, _("The parent has to be a SUBWIN\n"));
    return (sciPointObj *) NULL;
  }
}



/********************** 21/05/2002 *****
 **ConstructFec
 * This function creates Grayplot
 * @author Djalel.ABDEMOUCHE
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud,
	      double *pfun, int Nnode, int Ntr, double *zminmax, int *colminmax,
	      int *colout, BOOL with_mesh)
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

		if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
		{
			FREE( pobj->pfeatures ) ;
			FREE( pobj ) ;
			return NULL ;
		}

		pFEC_FEATURE (pobj)->callback = (char *)NULL;
		pFEC_FEATURE (pobj)->callbacklen = 0;
		pFEC_FEATURE (pobj)->callbackevent = 100;

		pFEC_FEATURE (pobj)->isselected = TRUE;
		pFEC_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));

		sciInitIsClipping( pobj, sciGetIsClipping(pparentsubwin) ) ;
		sciSetClipping(pobj, sciGetClipping(pparentsubwin)) ;

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
		/* sline mdoe is set using with_mesh */
		sciInitIsLine(pobj, with_mesh);

		return pobj;
	}
	else
	{
		Scierror(999, _("The parent has to be a SUBWIN\n"));
		return (sciPointObj *) NULL;
	}
}



/**ConstructSegs
 * This function creates Segments
 * @author Djalel.ABDEMOUCHE
 * @version 0.1
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructSegs ( sciPointObj * pparentsubwin, int type,
                double *vx, double *vy, double *vz,
                int Nbr1,int Nbr2, int Nbr3,
                double *vfx, double *vfy,
                int flag, int *style, double arsize,
                int colored, int typeofchamp)
{
	sciPointObj *pobj = (sciPointObj *) NULL;
	sciSegs * ppSegs = (sciSegs *) NULL;
	int i;

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

		if ( sciStandardBuildOperations( pobj, pparentsubwin ) == NULL )
		{
			FREE( pobj->pfeatures ) ;
			FREE( pobj ) ;
			return NULL ;
		}

		ppSegs = pSEGS_FEATURE(pobj) ;

		ppSegs->callback = (char *)NULL;
		ppSegs->callbacklen = 0;
		ppSegs->callbackevent = 100;

		ppSegs->isselected = TRUE;
		ppSegs->visible = sciGetVisibility(sciGetParentSubwin(pobj));

		/* this must be done prior to the call of sciSetClipping to know */
		/* if the clip_state has been set */
		ppSegs->clip_region_set = 0;
		sciInitIsClipping( pobj, sciGetIsClipping(sciGetParentSubwin(pobj) ));
		sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));


		ppSegs = pSEGS_FEATURE (pobj);
		ppSegs->ptype = type;

		ppSegs->pstyle = NULL ;

		if ((ppSegs->vx = MALLOC (Nbr1 * sizeof (double))) == NULL)
		{
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(ppSegs);
			FREE(pobj);
			return (sciPointObj *) NULL;
		}
		if ((ppSegs->vy = MALLOC (Nbr2 * sizeof (double))) == NULL)
		{
			FREE(ppSegs->vx);
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(ppSegs);
			FREE(pobj);
			return (sciPointObj *) NULL;
		}
    if (vz!=NULL)
    {
		  if ((ppSegs->vz = MALLOC (Nbr3 * sizeof (double))) == NULL)
		  {
			  FREE(ppSegs->vx);
			  FREE(ppSegs->vy);
			  sciDelThisToItsParent (pobj, sciGetParent (pobj));
			  sciDelHandle (pobj);
			  FREE(ppSegs);
			  FREE(pobj);
			  return (sciPointObj *) NULL;
		  }
    }
    else
    {
        ppSegs->vz = NULL;
    }

		for (i = 0; i < Nbr1; i++)
			ppSegs->vx[i] = vx[i];
		for (i = 0; i < Nbr2; i++)
			ppSegs->vy[i] = vy[i];
    if (vz!=NULL)
    {
  		for (i = 0; i < Nbr3; i++)
        {
	  		ppSegs->vz[i] = vz[i];
        }
    }

		ppSegs->ptype = type;

		/* F.Leray Test imprortant sur type ici*/
		if (type == 0) /* attention ici type = 0 donc...*/
		{
			ppSegs->typeofchamp = -1; /* useless property in the case type == 0 */
			ppSegs->arrowsize = arsize /** 100*/;       /* A revoir: F.Leray 06.04.04 */
			if ((ppSegs->pstyle = MALLOC (Nbr1 * sizeof (int) )) == NULL)
			{
				FREE(ppSegs->vx);
				FREE(ppSegs->vy);
        if (vz!=NULL)
  				FREE(ppSegs->vz);
				sciDelThisToItsParent (pobj, sciGetParent (pobj));
				sciDelHandle (pobj);
				FREE(ppSegs);
				FREE(pobj);
				return (sciPointObj *) NULL;
			}
			if (flag == 1)
			{
				for (i = 0; i < Nbr1; i++)
				{
					ppSegs->pstyle[i] = style[i];
				}
			}
			else
			{
				for (i = 0; i < Nbr1; i++)
				{
					ppSegs->pstyle[i] = style[0];
				}
			}

			ppSegs->iflag = flag;
			ppSegs->Nbr1 = Nbr1;
		}
		else /* Warning here type = 1 so... building comes from champg */
		{
			/* Rajout de psegs->arrowsize = arsize; F.Leray 18.02.04*/
			ppSegs->arrowsize = arsize /* * 100 */;
			ppSegs->Nbr1 = Nbr1;
			ppSegs->Nbr2 = Nbr2;
			sciInitForeground(pobj,sciGetForeground(sciGetCurrentSubWin())); /* set sciGetForeground(psubwin) as the current foreground */
			ppSegs->typeofchamp = typeofchamp; /* to know if it is a champ or champ1 */
			if ((ppSegs->vfx = MALLOC ((Nbr1*Nbr2) * sizeof (double))) == NULL)
			{
				FREE(ppSegs->vx);
				FREE(ppSegs->vy);
        if (vz!=NULL)
  				FREE(ppSegs->vz);
				sciDelThisToItsParent (pobj, sciGetParent (pobj));
				sciDelHandle (pobj);
				FREE(ppSegs);
				FREE(pobj);
				return (sciPointObj *) NULL;
			}
			if ((ppSegs->vfy = MALLOC ((Nbr1*Nbr2) * sizeof (double))) == NULL)
			{
				FREE(ppSegs->vx);
				FREE(ppSegs->vy);
        if (vz!=NULL)
  				FREE(ppSegs->vz);
				FREE(ppSegs->vfx);
				sciDelThisToItsParent (pobj, sciGetParent (pobj));
				sciDelHandle (pobj);
				FREE(ppSegs);
				FREE(pobj);
				return (sciPointObj *) NULL;
			}

			for (i = 0; i < (Nbr1*Nbr2); i++)
			{
				ppSegs->vfx[i] = vfx[i];
				ppSegs->vfy[i] = vfy[i];
			}
			pSEGS_FEATURE (pobj)->vfz=(double *) NULL; /**DJ.Abdemouche 2003**/
		}
		if (sciInitGraphicContext (pobj) == -1)
		{
			FREE(ppSegs->vx);
			FREE(ppSegs->vy);
      if (vz!=NULL)
  		  FREE(ppSegs->vz);
			if (type ==0)
			{
				FREE(ppSegs->pstyle);
			}
			else
			{
				FREE(ppSegs->vfx);
				FREE(ppSegs->vfy);
			}
			sciDelThisToItsParent (pobj, sciGetParent (pobj));
			sciDelHandle (pobj);
			FREE(ppSegs);
			FREE(pobj);
			return (sciPointObj *) NULL;
		}
    return pobj;
  }
  else
  {
    Scierror(999, _("The parent has to be a SUBWIN\n"));
    return (sciPointObj *) NULL;
  }
}


/**sciConstructCompound
 * constructes an Compound of entities
 * do only a association with a parent and a handle reservation !
 * check for valid handle can be done using CheckForCompound
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

  if ( sciStandardBuildOperations( pobj, sciGetParent(sciGetPointerFromHandle( (long) handelsvalue[0])) ) == NULL )
  {
    FREE( pobj->pfeatures ) ;
    FREE( pobj ) ;
    return NULL ;
  }

  ppCompound->callback = (char *)NULL;
  ppCompound->callbacklen = 0;
  ppCompound->visible = sciGetVisibility(sciGetParentSubwin(pobj));

  /* sonsnext = (sciSons *) NULL */

  /* initialisation with the first son */
  xtmp = (long) handelsvalue[0];
  for ( i = 0 ; i < number ; i++ )
    {


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
 * constructes an Compound of with the last n entities created in the current subwindow
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
  sciPointObj *pobj;
  int i;

  sciPointObj *psubwin;
  sciSubWindow *ppsubwin;
  sciAgreg     *ppagr;

  psubwin = sciGetCurrentSubWin();
  ppsubwin=pSUBWIN_FEATURE(psubwin);

  /* initialize the A Compound data structure */
  if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
  {
    return NULL;
  }

  sciSetEntityType (pobj, SCI_AGREG);
  if ((pobj->pfeatures = MALLOC ((sizeof (sciAgreg)))) == NULL)
  {
    return NULL;
  }

  ppagr = pAGREG_FEATURE(pobj) ;

  if ( sciStandardBuildOperations( pobj, psubwin ) == NULL )
  {
    FREE( pobj->pfeatures ) ;
    FREE( pobj ) ;
    return NULL ;
  }

  /* Remove the created objects after the compound and add them */
  /* Under the compound in the same order */
  for ( i = 0 ; i < number ; i++ )
  {
    /* Get the first object to move (the first son in the list is the compound) */
    sciSons * sonToMove = sciGetSons(psubwin)->pnext;
    sciPointObj * curObj = sonToMove->pointobj;
    /* remove it from the subwin */
    sciDelSonFromItsParent(sonToMove, psubwin);
    /* add it to the agreg */
    sciAddThisToItsParentLastPos(curObj, pobj);
  }

  sciInitSelectedSons(pobj);

  /* set Compound properties*/
  initUserData(pobj);
  ppagr->callback = (char *)NULL;
  ppagr->callbacklen = 0;
  ppagr->visible = sciGetVisibility (sciGetParentFigure(pobj));

  ppagr->isselected = TRUE;

  return (sciPointObj *)pobj;
}



/**ConstructLabel
 * This function creates Label structure used for x,y,z labels and for the Title.
 * @param  sciPointObj *pparentsubwin
 * @param  char text[] : intial text string.
 * @param  int type to get info. on the type of label
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructLabel (sciPointObj * pparentsubwin, char *text, int type)
{
  sciPointObj * pobj = NULL;
  /* get a pointer on the feature */
  sciLabel    * ppLabel ;
  char * emptyString = "" ;
  int defaultColor = 0 ;

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

    ppLabel = pLABEL_FEATURE( pobj ) ;

    ppLabel->text = allocateText( pparentsubwin, &emptyString, 1, 1,
                                  0.0, 0.0, TRUE, NULL, FALSE, &defaultColor, &defaultColor,
                                  FALSE, FALSE, FALSE, ALIGN_LEFT ) ;

		/* RelationShip is actually stored in the text object */
		pobj->relationShip = ppLabel->text->relationShip;

    sciStandardBuildOperations(pobj, pparentsubwin);

    /* labels are not clipped */
    sciSetIsClipping(ppLabel->text, -1) ;

    /* Use centered mode */
    sciInitCenterPos(ppLabel->text, FALSE);
    sciInitAutoSize(ppLabel->text, TRUE);

    /* 1.0 for logarithmic mode */
    sciInitTextPos(pobj, 1.0, 1.0, 1.0);

    sciInitIsFilled(pobj,FALSE); /* by default a simple text is display (if existing) */

    sciInitIs3d( pobj, FALSE ) ; /* the text of labels is displayed using 2d scale */

    ppLabel->ptype = type;
    ppLabel->auto_position = TRUE;
    ppLabel->auto_rotation = TRUE;

    ppLabel->isselected = TRUE;

    return pobj;
  }
  else
  {
    Scierror(999, _("The parent has to be a SUBWIN\n"));
    return (sciPointObj *) NULL;
  }
}
/*----------------------------------------------------------------------------*/
/**
 * contains the functions always called when creating an object
 * pObj should have just been allocated.
 * @return the modified object. Should be the same as pObj, unless an error occured.
 *         then it is NULL.
 */
sciPointObj * sciStandardBuildOperations( sciPointObj * pObj, sciPointObj * parent )
{

	/* Allocate relationShip */
	createDefaultRelationShip(pObj);

  /* add the handle in the handle list */
  if ( sciAddNewHandle(pObj) == -1 )
  {
    return NULL ;
  }


  /* connect the object under its parent in the hierarchy */
  if ( !sciAddThisToItsParent( pObj, parent) )
  {
    sciDelHandle(pObj) ;
    return NULL ;
  }

  sciInitVisibility( pObj, TRUE ) ;

  initUserData(pObj);

  pObj->pObservers = DoublyLinkedList_new() ;
  createDrawingObserver( pObj ) ;


  pObj->pDrawer = NULL ;

  return pObj ;

}
/*----------------------------------------------------------------------------*/
/**
 * Create a figure if none exists.
 */
void SciWin(void)
{
  if (!sciHasFigures())
  {
    sciGetCurrentFigure();
  }
}
/*----------------------------------------------------------------------------*/
/**
 * Create a new figure with already a subwindow inside and show it.
 * When creating a new figure this mehod mut be called
 * @param winNum if not NULL a pointer to the figure number otherwise
 *               a default figure number is chosen.
 * @return a pointer on the created figure or NULL if the creation could
 *         not be performed
 */
sciPointObj * createFullFigure(int * winNum)
{
  sciPointObj * newFig = NULL;
  sciPointObj * newSubwin = NULL;

  /* Check that environement is OK for creating a window */
  if (!sciGetIsAbleToCreateWindow())
  {
    return NULL;
  }


  /* Create figure */
  newFig = ConstructFigure(NULL, winNum);

  startFigureDataWriting(newFig);

  if (newFig == NULL)
  {
    endFigureDataWriting(newFig);
    return NULL;
  }

  sciSetCurrentFigure(newFig);

  /* Add a subwindow inside the figure */
  if (createFirstSubwin(newFig) == NULL)
  {
    DestroyFigure(newFig);
    endFigureDataWriting(newFig);
    return NULL;
  }

  endFigureDataWriting(newFig);



  /* show th enewly created window */
  showWindow(newFig);


  return newFig;
}
/*----------------------------------------------------------------------------*/
/**
 * Get the first subwin object within a figure.
 * If not exists, create one withj default value.
 * @return the first subwin or NULL if an error occured during subwin creation
 */
sciPointObj * createFirstSubwin(sciPointObj * pFigure)
{
	if (sciGetNbTypedObjects(pFigure, SCI_SUBWIN) > 0)
	{
		/* return the first object */
		return sciGetFirstTypedSelectedSon(pFigure, SCI_SUBWIN);
	}
	else
	{
		/* No subwins, create the default one */
		sciPointObj * newSubwin = ConstructSubWin(pFigure);
		if (newSubwin != NULL)
		{
			sciSetCurrentObj(newSubwin);
			sciSetOriginalSubWin(pFigure, newSubwin);
			return newSubwin;
		}
		else
		{
			return NULL;
		}
	}
}
/*----------------------------------------------------------------------------*/
void createDefaultRelationShip(sciPointObj * pObj)
{
	/* Create a new relationship structure */
	sciRelationShip * relationShip = NULL;
	if(sciGetEntityType(pObj) == SCI_LABEL)
	{
		/* labels have their relationShip stored in their text objects */
		return;
	}
	
	relationShip = MALLOC(sizeof(sciRelationShip));
	if (relationShip == NULL)
	{
		return;
	}

	/* Set default values */
	relationShip->handleIndex = 0;
	relationShip->plastsons = NULL;
	relationShip->psons = NULL;
	relationShip->pparent = NULL;
	relationShip->pSelectedSon = DoublyLinkedList_new();

	/* Add it to the object */
	pObj->relationShip = relationShip;

}
/*----------------------------------------------------------------------------*/
/**
 * Initialize the user data of a graphic obj
 */
void initUserData(sciPointObj * pObj)
{
	int ** userData = NULL ;
  int *  udSize   = NULL ;

	sciGetPointerToUserData( pObj, &userData, &udSize ) ;
  *userData = NULL ;
  *udSize   = 0    ;
}
/*----------------------------------------------------------------------------*/
