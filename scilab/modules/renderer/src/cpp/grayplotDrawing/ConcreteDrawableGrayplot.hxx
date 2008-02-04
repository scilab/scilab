/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableGrayplot.hxx                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independent routine for drawing     */
/*        a grayplot object                                               */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_GRAYPLOT_HXX_
#define _CONCRETE_DRAWABLE_GRAYPLOT_HXX_

#include "DrawableGrayplot.h"
#include "GrayplotDecompositionStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawableGrayplot : public DrawableGrayplot
{
public:

  ConcreteDrawableGrayplot(sciPointObj * pObj);

  ~ConcreteDrawableGrayplot(void);


  void setDeccompositionStrategy(GrayplotDecompositionStrategy * strategy);

protected:
  /*----------------------------------------------------------*/
  /**
   * Actually draw the object
   */
  virtual void drawGrayplot(void);

  /**
   * show the object
   */ 
  virtual void showGrayplot(void);
  /*----------------------------------------------------------*/
  GrayplotDecompositionStrategy * m_pDecomposer;
  /*----------------------------------------------------------*/


};

}

#endif /* _CONCRETE_DRAWABLE_GRAYPLOT_HXX_ */
