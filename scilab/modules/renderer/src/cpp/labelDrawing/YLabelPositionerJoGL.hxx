/*------------------------------------------------------------------------*/
/* file: YLabelPositionerJoGL.hxx                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning Y label using JoGL                            */
/*------------------------------------------------------------------------*/


#ifndef _Y_LABEL_POSITIONER_JOGL_HXX_
#define _Y_LABEL_POSITIONER_JOGL_HXX_

#include "LabelPositionerJoGL.hxx"

namespace sciGraphics
{

class YLabelPositionerJoGL : public LabelPositionerJoGL
{
public:

  YLabelPositionerJoGL(DrawableLabel * label);

  virtual ~YLabelPositionerJoGL(void);

protected:

  /**
   * Compute the automatic position of the label
   */
  virtual double getAutoOrientation(void);

};

}

#endif /* _Y_LABEL_POSITIONER_JOGL_HXX_ */
