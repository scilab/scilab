/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Interface for drawing text box
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawTextBoxStrategy.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawTextBoxStrategy::DrawTextBoxStrategy(DrawableText * text)
{
  m_pDrawed = text;
  for (int i = 0; i < 4; i++)
  {
    m_aCorners[i][0] = 0.0;
    m_aCorners[i][1] = 0.0;
    m_aCorners[i][2] = 0.0;
  }
}
/*---------------------------------------------------------------------------------*/
DrawTextBoxStrategy::~DrawTextBoxStrategy(void)
{
  m_pDrawed = NULL;
}
/*---------------------------------------------------------------------------------*/
void DrawTextBoxStrategy::setBoxCorners(const double corner1[3], const double corner2[3],
                                        const double corner3[3], const double corner4[3])
{
  // apply log scale if needed
  m_pDrawed->pointScale(corner1[0], corner1[1], corner1[2],
                        &(m_aCorners[0][0]), &(m_aCorners[0][1]), &(m_aCorners[0][2]) );
  m_pDrawed->pointScale(corner2[0], corner2[1], corner2[2],
                        &(m_aCorners[1][0]), &(m_aCorners[1][1]), &(m_aCorners[1][2]) );
  m_pDrawed->pointScale(corner3[0], corner3[1], corner3[2],
                        &(m_aCorners[2][0]), &(m_aCorners[2][1]), &(m_aCorners[2][2]) );
  m_pDrawed->pointScale(corner4[0], corner4[1], corner4[2],
                        &(m_aCorners[3][0]), &(m_aCorners[3][1]), &(m_aCorners[3][2]) );
}
/*---------------------------------------------------------------------------------*/
}
