/*------------------------------------------------------------------------*/
/* file: LabelPositionerJoGL.hxx                                          */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in positioning labels using JoGL              */
/*------------------------------------------------------------------------*/


#ifndef _LABEL_POSITIONER_JOGL_HXX_
#define _LABEL_POSITIONER_JOGL_HXX_

#include "../DrawableObjectJoGL.h"
#include "DrawableLabel.h"
#include "LabelPositioner.hxx"
#include "LabelPositionerJavaMapper.hxx"

namespace sciGraphics
{

class LabelPositionerJoGL : public LabelPositioner, public DrawableObjectJoGL
{
public:

  LabelPositionerJoGL(DrawableLabel * label);

  virtual ~LabelPositionerJoGL(void);

protected:

  /**
   * Compute the automatic position of the label
   */
  virtual void getAutoPosition(double pos[3]);

  /**
   * Get the object performing mapping with Java class.
   */
  LabelPositionerJavaMapper * getLabelPositionerJavaMapper(void);

};

}

#endif /* _LABEL_POSITIONER_JOGL_HXX_ */
