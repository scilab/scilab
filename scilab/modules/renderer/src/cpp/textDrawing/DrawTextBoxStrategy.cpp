/*------------------------------------------------------------------------*/
/* file: DrawTextBoxStrategy.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for drawing text box                                  */
/*------------------------------------------------------------------------*/

#include "DrawTextBoxStrategy.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------*/
DrawTextBoxStrategy::~DrawTextBoxStrategy(void)
{
  m_pDrawed = NULL;
}
/*------------------------------------------------------------------------------------------*/
void DrawTextBoxStrategy::setBoxCorners(const double corner1[3], const double corner2[3],
                                        const double corner3[3], const double corner4[3])
{
  for (int j = 0; j < 3; j++)
  {
    m_aCorners[0][j] = corner1[j];
    m_aCorners[1][j] = corner2[j];
    m_aCorners[2][j] = corner3[j];
    m_aCorners[3][j] = corner4[j];
  }
}
/*------------------------------------------------------------------------------------------*/
}
