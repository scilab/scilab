/*------------------------------------------------------------------------*/
/* file: XLabelPositionerJoGL.hxx                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning X label using JoGL                            */
/*------------------------------------------------------------------------*/


#ifndef _X_LABEL_POSITIONER_JOGL_HXX_
#define _X_LABEL_POSITIONER_JOGL_HXX_

#include "LabelPositionerJoGL.hxx"

namespace sciGraphics
{

class XLabelPositionerJoGL : public LabelPositionerJoGL
{
public:

  XLabelPositionerJoGL(DrawableLabel * label);

  virtual ~XLabelPositionerJoGL(void);

protected:

  /**
   * Compute the automatic position of the label
   */
  virtual double getAutoOrientation(void);

};

}

#endif /* _X_LABEL_POSITIONER_JOGL_HXX_ */
