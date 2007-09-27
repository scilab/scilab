/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableText.hxx                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms                                                      */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawableText.hxx"

namespace sciGraphics
{

using namespace std;

/*------------------------------------------------------------------------------------------*/
ConcreteDrawableText::ConcreteDrawableText(sciPointObj * pObj) : DrawableText(pObj)
{
  m_oDrawingBoxStrategies.clear();
  m_pDrawingTextStrategy = NULL;
}
/*------------------------------------------------------------------------------------------*/
ConcreteDrawableText::~ConcreteDrawableText(void)
{
  removeBoxDrawingStrategies();
  removeTextDrawingStrategy();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  m_pDrawingTextStrategy->getBoundingRectangle(corner1, corner2, corner3, corner4);
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::getScreenBoundingBox(int * xPos, int * yPos, int * width, int * height)
{
  m_pDrawingTextStrategy->getScreenBoundingBox(xPos, yPos, width, height);
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::addBoxDrawingStrategy(DrawTextBoxStrategy * strategy)
{
  m_oDrawingBoxStrategies.push_back(strategy);
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::removeBoxDrawingStrategies(void)
{
  list<DrawTextBoxStrategy *>::iterator it = m_oDrawingBoxStrategies.begin();
  for( ; it != m_oDrawingBoxStrategies.end(); it++)
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawingBoxStrategies.clear();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::setTextDrawingStrategy(DrawTextContentStrategy * strategy)
{
  m_pDrawingTextStrategy = strategy;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::removeTextDrawingStrategy(void)
{
  delete m_pDrawingTextStrategy;
  m_pDrawingTextStrategy = NULL;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::drawBox(void)
{
  list<DrawTextBoxStrategy *>::iterator it = m_oDrawingBoxStrategies.begin();
  for( ; it != m_oDrawingBoxStrategies.end(); it++)
  {
    (*it)->drawBox();
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::drawTextContent(void)
{
  m_pDrawingTextStrategy->drawTextContent();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::showBox(void)
{
  list<DrawTextBoxStrategy *>::iterator it = m_oDrawingBoxStrategies.begin();
  for( ; it != m_oDrawingBoxStrategies.end(); it++)
  {
    (*it)->showBox();
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableText::showTextContent(void)
{
  m_pDrawingTextStrategy->drawTextContent();
}
/*------------------------------------------------------------------------------------------*/

}
