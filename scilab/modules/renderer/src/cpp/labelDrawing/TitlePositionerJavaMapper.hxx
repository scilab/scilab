/*------------------------------------------------------------------------*/
/* file: TitlePositionerJavaMapper.hxx                                    */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by TitlePositionerJoGL     */
/*------------------------------------------------------------------------*/

#ifndef _TITLE_POSITIONER_JAVA_MAPPER_HXX_
#define _TITLE_POSITIONER_JAVA_MAPPER_HXX_

#include "LabelPositionerJavaMapper.hxx"
#include "TitlePositionerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by TitlePositionerJoGL 
 */
class TitlePositionerJavaMapper : public virtual LabelPositionerJavaMapper
{
public:

  TitlePositionerJavaMapper(void);

  virtual ~TitlePositionerJavaMapper(void);
  /*----------------------------------------------------------------------*/
  // Inherited from DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for LabelsPositioner
  virtual void getLabelPosition(double distanceToAxis, double labelPos[3]);

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax);

  virtual void setLabelPixBoundingBox(int corner1[2], int corner2[2],
                                      int corner3[2], int corner4[2]);
  /*----------------------------------------------------------------------*/
  // specific for TitlePositioner
  virtual void setViewingAreaUpperPos(int xPos, int yPos);
  /*----------------------------------------------------------------------*/


protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_labelDrawing::TitlePositionerGL * m_pJavaObject;

};

}

#endif /* _TITLE_POSITIONER_JAVA_MAPPER_HXX_ */
