/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: CurrentObjectsManagement.c                                        */
/* desc : Set of functions to retrieve the currents objects               */
/*------------------------------------------------------------------------*/

#include "BuildObjects.h"
#include "ObjectSelection.h"
#include "FigureList.h"
#include "HandleManagement.h"
#include "InitObjects.h"
#include "MALLOC.h"
#include "CloneObjects.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "callJoGLView.h"

/*----------------------------------------------------------------------------------*/
/* root of the graphic hierarchy */
/* singleton, there is only one screen */
static sciPointObj * sciCurrentObject  = NULL ;
static sciPointObj * sciCurrentFigure  = NULL ;
/*----------------------------------------------------------------------------------*/
static sciPointObj * getCurrentPointedFigure(void);
/*----------------------------------------------------------------------------------*/
static sciPointObj * getCurrentPointedFigure(void)
{
  return sciCurrentFigure;
}
/*----------------------------------------------------------------------------------*/
sciPointObj * __sciGetCurrentFigure( void )
{
    // ???
    abort();
#if 0
  /* debug F.Leray 22.07.04 */
  sciPointObj * pFigure = getCurrentPointedFigure();
  sciPointObj* newaxes = NULL;

  if( !sciHasFigures() )
  {
    /* it would mean that we have change the driver to GIF,Pos or PPM and perform a xinit F.Leray 22.07.04 */
    /* for now, no higher entities than figure */
      int iZero = 0;
      pFigure = sciCloneObj(getFigureModel());
      setGraphicObjectProperty(pFigure->UID, __GO_ID__, &iZero, jni_int, 1);
      createJoGLView(pFigure->UID);

      /*
       * Clones a new Axes object using the Axes model which is then
       * attached to the newly created Figure.
       */
      newaxes = sciCloneObj(getAxesModel());

      /* Sets the parent-child relationship within the MVC */
      setGraphicObjectRelationship(pFigure->UID, newaxes->UID);

      /* Sets the newly created Axes as the Figure's current selected child */
      setGraphicObjectProperty(pFigure->UID, __GO_SELECTED_CHILD__, newaxes->UID, jni_string, 1);

      /*
       * Added back to avoid creating a new Figure each time gcf() is executed.
       * This was previously done in ConstructFigure, called by createFullFigure
       * which has been replaced by the Figure model clone call above.
       */
      // No more needed with MVC.
      //addNewFigureToList(pFigure);

      setCurrentFigure(pFigure->UID);

      // Register handle to Scilab.
//      sciAddNewHandle(pFigure);

      /*
       * Registers the Axes' handle and sets the Axes as the current object.
       * This was previously done in ConstructSubWin, called by createFirstSubwin
       * which was also called by createFullFigure.
       */
//      sciAddNewHandle(newaxes);
//      sciSetCurrentObj(newaxes);
  }

  return pFigure;
#endif
  return NULL;
}
/*----------------------------------------------------------------------------------*/
BOOL sciIsCurrentFigure(sciPointObj * pFigure)
{
#if 0
    return (   pFigure != getFigureModel()
          && sciHasFigures()
          && (pFigure == sciGetCurrentFigure()));
#endif
    return FALSE;
}
/*----------------------------------------------------------------------------------*/
int sciInitCurrentFigure( sciPointObj * mafigure )
{
  sciCurrentFigure = mafigure ;
  return 0 ;
}
/*----------------------------------------------------------------------------------*/
int sciSetCurrentFigure ( sciPointObj * mafigure )
{
  if ( sciGetCurrentFigure() == mafigure )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitCurrentFigure( mafigure ) ;
}
/*----------------------------------------------------------------------------------*/
sciPointObj * __sciGetCurrentObj( void )
{
  return sciCurrentObject ;
}
/*----------------------------------------------------------------------------------*/
void __sciSetCurrentObj( sciPointObj * pobj )
{
  sciCurrentObject = pobj ;
}
/*----------------------------------------------------------------------------------*/
BOOL sciIsCurrentObject(sciPointObj * pobj)
{
	return (pobj == sciGetCurrentObj());
}
/*----------------------------------------------------------------------------------*/
long sciGetCurrentHandle( void )
{
  return sciGetHandle( sciGetCurrentObj() );
}
/*-----------------------------------------------------------------------------*/
sciPointObj * __sciGetCurrentSubWin( void )
{
#if 0
  sciPointObj * currentFigure = sciGetCurrentFigure();
  sciPointObj * currentSubwin = NULL;
  int iNbChildren = 0;
  int *piNbChildren = &iNbChildren;
  char* selectedChild;

  if ( currentFigure == NULL ) { return NULL ; }

  getGraphicObjectProperty(currentFigure->UID, __GO_CHILDREN_COUNT__, jni_int, &piNbChildren);

  if (iNbChildren == 0)
  {
    return NULL;
  }

  /* The figure's current selected child corresponds to the current subwindow */
  getGraphicObjectProperty(currentFigure->UID, __GO_SELECTED_CHILD__, jni_string, &selectedChild);

  currentSubwin = MALLOC(sizeof(sciPointObj));

  currentSubwin->UID = selectedChild;
//  sciAddNewHandle(currentSubwin);

  /*
   * Former way to get the Figure's current selected Axes.
   * To be deleted
   */
  currentSubwin = sciGetFirstTypedSelectedSon( currentFigure, SCI_SUBWIN ) ;

  return currentSubwin;
#endif
  return NULL;
}
/*-----------------------------------------------------------------------------*/

