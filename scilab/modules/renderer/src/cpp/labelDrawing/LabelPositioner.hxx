/*------------------------------------------------------------------------*/
/* file: LabelPositioner.hxx                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in positioning labels                         */
/*------------------------------------------------------------------------*/

#ifndef _LABEL_POSITIONER_HXX_ 
#define _LABEL_POSITIONER_HXX_

#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

class DrawableLabel;

class LabelPositioner : public virtual DrawableObjectBridge
{
public:

  LabelPositioner(void);

  virtual ~LabelPositioner(void);

  /**
   * Set position of the label.
   */
  void setLabelPosition(void);

  /**
   * Set font angle for labels.
   */
  void setLabelOrientation(void);

  /**
   * Set distance in pixels from labels to axis
   */
  void setDistanceToAxis(double distance);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Compute the automatic position of the label
   */
  virtual void getAutoPosition(double pos[3]) = 0;

  /**
   * Compute the automatic font angle of the label.
   */
  virtual double getAutoOrientation(void) = 0;
  /*----------------------------------------------------------------------*/
  double m_dDistanceToAxis;
  /*----------------------------------------------------------------------*/
};

}

#endif /* _LABEL_POSITIONER_HXX_ */
