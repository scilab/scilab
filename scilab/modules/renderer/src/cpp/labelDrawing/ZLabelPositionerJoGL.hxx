/*------------------------------------------------------------------------*/
/* file: ZLabelPositionerJoGL.hxx                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning Z label using JoGL                            */
/*------------------------------------------------------------------------*/


#ifndef _Z_LABEL_POSITIONER_JOGL_HXX_
#define _Z_LABEL_POSITIONER_JOGL_HXX_

#include "LabelPositionerJoGL.hxx"

namespace sciGraphics
{

class ZLabelPositionerJoGL : public LabelPositionerJoGL
{
public:

  ZLabelPositionerJoGL(DrawableLabel * label);

  virtual ~ZLabelPositionerJoGL(void);

protected:

  /**
   * Compute the automatic position of the label
   */
  virtual double getAutoOrientation(void);

};

}

#endif /* _Z_LABEL_POSITIONER_JOGL_HXX_ */
