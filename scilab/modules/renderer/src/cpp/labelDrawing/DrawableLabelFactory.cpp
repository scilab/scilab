/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for label objects. Automatically create a label
 * drawer with the right algorithms from the graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableLabelFactory.h"
#include "DrawableLabel.h"
#include "XLabelPositioner.hxx"
#include "YLabelPositioner.hxx"
#include "ZLabelPositioner.hxx"
#include "TitlePositioner.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableLabelFactory::create( void )
{
  DrawableLabel * newLabel = new DrawableLabel( m_pDrawed ) ;
  setPositionner(newLabel);
  return newLabel;
}
/*---------------------------------------------------------------------------------*/
void DrawableLabelFactory::update( void )
{
  // nothing for now, label type does not change
}
/*---------------------------------------------------------------------------------*/
void DrawableLabelFactory::setPositionner(DrawableLabel * label)
{

  switch (pLABEL_FEATURE(m_pDrawed)->ptype)
  {
  case 1:
    // title
    label->setLabelPositioner(new TitlePositioner(label));
    break;
  case 2:
    // x label
    label->setLabelPositioner(new XLabelPositioner(label));
    break;
  case 3:
    // y label
    label->setLabelPositioner(new YLabelPositioner(label));
    break;
  case 4:
    // z label
    label->setLabelPositioner(new ZLabelPositioner(label));
    break;
  default:
    label->setLabelPositioner(NULL);
    break;
  }

}
/*---------------------------------------------------------------------------------*/
}
