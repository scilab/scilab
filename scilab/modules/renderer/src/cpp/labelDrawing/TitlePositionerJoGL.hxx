/*------------------------------------------------------------------------*/
/* file: TitlePositionerJoGL.hxx                                          */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning Tile label using JoGL                         */
/*------------------------------------------------------------------------*/


#ifndef _TITLE_POSITIONER_JOGL_HXX_
#define _TITLE_POSITIONER_JOGL_HXX_

#include "LabelPositionerJoGL.hxx"
#include "TitlePositionerJavaMapper.hxx"

namespace sciGraphics
{

class TitlePositionerJoGL : public LabelPositionerJoGL
{
public:

  TitlePositionerJoGL(DrawableLabel * label);

  virtual ~TitlePositionerJoGL(void);

protected:

  /**
   * Compute the automatic position of the label
   */
  virtual double getAutoOrientation(void);

  /**
   * Compute the automatic position of the label
   */
  virtual void getAutoPosition(double pos[3]);

  /**
   * Get the object performing mapping with Java class.
   */
  TitlePositionerJavaMapper * getTitlePositionerJavaMapper(void);

};

}

#endif /* _TITLE_POSITIONER_JOGL_HXX_ */
