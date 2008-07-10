/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to create and access a drawer of graphic handles
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <list>

#include "DrawingBridge.h"
#include "getHandleDrawer.h"
#include "DrawableObjectFactory.h"
#include "GraphicSynchronizerInterface.h"
#include "BasicAlgos.hxx"

extern "C"
{
#include "../../../graphics/includes/GetProperty.h"
}

using namespace sciGraphics ;
using namespace std ;

/*---------------------------------------------------------------------------------*/
static list<sciPointObj *> getParentFigureList(sciPointObj * pObjs[], int nbObjs);
static list<sciPointObj *> getChildrenOfFigure(sciPointObj * parentFigure,
                                        sciPointObj * pObjs[], int nbObjs);
/*---------------------------------------------------------------------------------*/
void createDrawer( sciPointObj * pObj )
{
  getHandleDrawer( pObj ) ;
}
/*---------------------------------------------------------------------------------*/
void destroyHandleDrawer( sciPointObj * pObj )
{
  /* don't call getHandleDrawer for the test, otherwise a drawer will be created */
  if ( pObj->pDrawer != NULL )
  {
    delete getHandleDrawer( pObj ) ;
    delete (DrawableObjectWrapper *)pObj->pDrawer ;
    pObj->pDrawer = NULL ;
  }
}
/*---------------------------------------------------------------------------------*/
void sciDrawObj( sciPointObj * pObj )
{
  /* force total redraw */
  /* we need to recreate a new one */
  getHandleDrawer(pObj)->hasChanged() ;
  /* redisplay everything, including this handle */
  getHandleDrawer( sciGetParentFigure( pObj ) )->display() ;
}
/*---------------------------------------------------------------------------------*/
void sciMoveObj(sciPointObj * pObj, const double translation[3])
{
  /* Specify that the object has moved */
  forceMove(pObj, translation[0], translation[1], translation[2]);
  /* redisplay everything, including this handle */
  getHandleDrawer( sciGetParentFigure( pObj ) )->display() ;
}
/*---------------------------------------------------------------------------------*/
void sciDrawSingleObj( sciPointObj * pObj )
{
 sciDrawSetOfObj(&pObj, 1); 
}
/*---------------------------------------------------------------------------------*/
void sciDrawSetOfObj(sciPointObj * pObjs[], int nbObjs )
{
  /* force redraw of the objects */
  for (int i = 0; i < nbObjs; i++)
  {
    forceRedraw(pObjs[i]);
  }

  // compute list of figure which have children to draw
  list<sciPointObj *> parentFigures = getParentFigureList(pObjs, nbObjs);

  list<sciPointObj *>::iterator it = parentFigures.begin();
  for (; it != parentFigures.end(); it++)
  {
    sciPointObj * curFigure = *it;
    // get all the children of this figure we need to draw
    list<sciPointObj *> childrens = getChildrenOfFigure(curFigure, pObjs, nbObjs);

    // then draw them
    getFigureDrawer(curFigure)->drawSingleObjs(childrens);
  }
  
}
/*---------------------------------------------------------------------------------*/
void displayChildren( sciPointObj * pObj )
{
  getHandleDrawer( pObj )->displayChildren() ;
}
/*---------------------------------------------------------------------------------*/
void redrawHierarchy( sciPointObj * pObj )
{
  /* Update everything */
  forceHierarchyRedraw(pObj);

  /* redisplay everything, including this handle */
  getHandleDrawer( sciGetParentFigure( pObj ) )->display() ;
}
/*---------------------------------------------------------------------------------*/
void forceHierarchyRedraw( sciPointObj * pObj )
{
  sciPointObj * parentFigure = sciGetParentFigure(pObj);
  startFigureDataWriting(parentFigure);
  getHandleDrawer(pObj)->familyHasChanged();
  endFigureDataWriting(parentFigure);
}
/*---------------------------------------------------------------------------------*/
void forceRedraw(sciPointObj * pObj)
{
  sciPointObj * parentFigure = sciGetParentFigure(pObj);
  startFigureDataWriting(parentFigure);
  getHandleDrawer(pObj)->hasChanged();
  endFigureDataWriting(parentFigure);
}
/*---------------------------------------------------------------------------------*/
void forceMove(sciPointObj * pObj, double tx, double ty, double tz)
{
  double displacement[3];
  displacement[0] = tx;
  displacement[1] = ty;
  displacement[2] = tz;
  sciPointObj * parentFigure = sciGetParentFigure(pObj);
  startFigureDataWriting(parentFigure);
  getHandleDrawer(pObj)->move(displacement);
  endFigureDataWriting(parentFigure);
}
/*---------------------------------------------------------------------------------*/
static list<sciPointObj *> getParentFigureList(sciPointObj * pObjs[], int nbObjs)
{
  list<sciPointObj *> res;
  for (int i = 0; i < nbObjs; i++)
  {
    sciPointObj * parentFigure = sciGetParentFigure(pObjs[i]);
    if (!BasicAlgos::listContains(res, parentFigure))
    {
      res.push_back(parentFigure);
    }
  }
  return res;
}
/*---------------------------------------------------------------------------------*/
static list<sciPointObj *> getChildrenOfFigure(sciPointObj * parentFigure,
                                        sciPointObj * pObjs[], int nbObjs)
{
  // compute the list of children of the current figure
  list<sciPointObj *> res;
  for (int i = 0; i < nbObjs; i++)
  {
    if (sciGetParentFigure(pObjs[i]) == parentFigure)
    {
      res.push_back(pObjs[i]);
    }
  }
  return res;
}
/*---------------------------------------------------------------------------------*/
