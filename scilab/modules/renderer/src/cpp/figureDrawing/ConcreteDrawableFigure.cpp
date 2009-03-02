/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver independent algorithms to draw a figure handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableFigure.h"
#include "getHandleDrawer.h"
#include "BasicAlgos.hxx"

extern "C"
{
#include "GetProperty.h"
#include "pixel_mode.h"
#include "SetProperty.h"
}

using namespace std;

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ConcreteDrawableFigure::ConcreteDrawableFigure( sciPointObj * pObj )
    : DrawableFigure( pObj )
{
  m_oSingleObjects.clear();
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableFigure::~ConcreteDrawableFigure(void)
{
  m_oSingleObjects.clear();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFigure::drawSingleObjs(list<sciPointObj *>& singleObjects)
{
  // copy the list
  m_oSingleObjects = singleObjects;

  // launch display
  // displaySingleObject will be called
  forceDisplay();

  // restore normal mode
  m_oSingleObjects.clear();
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::checkAutoRedraw( void )
{
	return ( sciGetIsAutoDrawable(m_pDrawed) == TRUE ) || !m_oSingleObjects.empty() ;
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::isDisplayingSingleObject(void)
{
  if (m_oSingleObjects.empty())
  {
    // not object to draw
    return false;
  }

  // check wether ther is a figure inside
  // if so redraw the whole figure

  list<sciPointObj *>::iterator it = m_oSingleObjects.begin();
  for (; it != m_oSingleObjects.end(); it++)
  {
    if (sciGetEntityType(*it) == SCI_FIGURE)
    {
      return false;
    }
  }
    
  return true;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFigure::displaySingleObject(void)
{

  // compute list of figure which have children to draw
  list<sciPointObj *> parentSubwins = getParentSubwinList(m_oSingleObjects);

  list<sciPointObj *>::iterator it = parentSubwins.begin();
  for (; it != parentSubwins.end(); it++)
  {
    sciPointObj * curSubwin = *it;
    // get all the children of this subwin we need to draw
    list<sciPointObj *> childrens = getChildrenOfSubwin(curSubwin, m_oSingleObjects);

    // then draw them
    getSubwinDrawer(curSubwin)->displaySingleObjs(childrens);
  }
}
/*---------------------------------------------------------------------------------*/
list<sciPointObj *> ConcreteDrawableFigure::getParentSubwinList(list<sciPointObj *> pObjs)
{
  list<sciPointObj *> res;
  list<sciPointObj *>::iterator it = pObjs.begin();
  for (; it != pObjs.end(); it++)
  {
    sciPointObj * parentSubwin = sciGetParentSubwin(*it);
    if (!BasicAlgos::listContains(res, parentSubwin))
    {
      res.push_back(parentSubwin);
    }
  }
  return res;
}
/*---------------------------------------------------------------------------------*/
list<sciPointObj *> ConcreteDrawableFigure::getChildrenOfSubwin(sciPointObj * parentSubwin,
                                                                list<sciPointObj *> pObjs)
{
  // compute the list of children of the current figure
  list<sciPointObj *> res;
  list<sciPointObj *>::iterator it = pObjs.begin();
  for (; it != pObjs.end(); it++)
  {
    if (sciGetParentSubwin(*it) == parentSubwin)
    {
      res.push_back(*it);
    }
  }
  return res;
}
/*---------------------------------------------------------------------------------*/


}
