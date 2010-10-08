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
/* file: CurrentObjectsManagemet.c                                        */
/* desc : Set of functions to retrieve the currents objects               */
/*------------------------------------------------------------------------*/

#include "CurrentObjectsManagement.h"
#include "BuildObjects.h"
#include "ObjectSelection.h"
#include "WindowList.h"
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
sciPointObj * sciGetCurrentFigure( void )
{
  /* debug F.Leray 22.07.04 */
  sciPointObj * pFigure = getCurrentPointedFigure();

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
      sciPointObj* newaxes = sciCloneObj(getAxesModel());

      /* Sets the parent-child relationship within the MVC */
      setGraphicObjectRelationship(pFigure->UID, newaxes->UID);

      /*
       * Added back to avoid creating a new Figure each time gcf() is executed.
       * This was previously done in ConstructFigure, called by createFullFigure
       * which has been replaced by the Figure model clone call above.
       */
      addNewFigureToList(pFigure);

      sciSetCurrentFigure(pFigure);

      // Register handle to Scilab.
      sciAddNewHandle(pFigure);
  }

  return pFigure;
}
/*----------------------------------------------------------------------------------*/
BOOL sciIsCurrentFigure(sciPointObj * pFigure)
{
  return (   pFigure != getFigureModel()
          && sciHasFigures()
          && (pFigure == sciGetCurrentFigure()));
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
sciPointObj * sciGetCurrentObj( void )
{
  return sciCurrentObject ;
}
/*----------------------------------------------------------------------------------*/
void sciSetCurrentObj( sciPointObj * pobj )
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
sciPointObj * sciGetCurrentSubWin( void )
{
  sciPointObj * currentFigure = sciGetCurrentFigure();
  sciPointObj * currentSubwin = NULL;
  int* nbChildren;
  char** children;

  if ( currentFigure == NULL ) { return NULL ; }

  nbChildren = (int*) getGraphicObjectProperty(currentFigure->UID, __GO_CHILDREN_COUNT__, jni_int);

  if (*nbChildren == 0)
  {
    return NULL;
  }

  /*
   * At the present moment, a figure is considered to have a unique Axes child object.
   * To be re-implemented, taking into account the last selected child object.
   */
  children = (char**) getGraphicObjectProperty(currentFigure->UID, __GO_CHILDREN__, jni_string_vector);

  currentSubwin = MALLOC(sizeof(sciPointObj));

  currentSubwin->UID = children[0];
  sciAddNewHandle(currentSubwin);

#if 0
  currentSubwin = sciGetFirstTypedSelectedSon( currentFigure, SCI_SUBWIN ) ;
#endif

  return currentSubwin;
}
/*-----------------------------------------------------------------------------*/

