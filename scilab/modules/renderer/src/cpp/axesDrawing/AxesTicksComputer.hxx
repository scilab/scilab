/*------------------------------------------------------------------------*/
/* file: AxesTicksComputer.hxx                                            */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute axes ticks                                              */
/*------------------------------------------------------------------------*/

#ifndef _AXES_TICKS_COMPUTER_HXX_
#define _AXES_TICKS_COMPUTER_HXX_

#include "../subwinDrawing/ComputeTicksStrategy.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

class AxesTicksComputer: public ComputeTicksStrategy
{
public:

  AxesTicksComputer(DrawableAxes * axes);

  virtual ~AxesTicksComputer(void);

  /**
  * Reinitialize ticks computer data.
  */
  virtual void reinit(void);

  /**
  * Get the number of ticks which will be computed by the strategy.
  */
  virtual int getNbTicks(void);

  /**
  * Fill positions parameters
  * with the ticks abscissas, ordinate, ...
  */
  virtual void getTicksPosition(double positions[], char * labels[], char * labelsExponents[]);

  /**
  * Get the number of subticks which will be computed by the strategy.
  */
  virtual int getNbSubticks(double ticksPositions[], int nbTicks);

  /**
  * Get the subticks positions.
  */
  virtual void getSubticksPosition(const double ticksPositions[], int nbTicks,
                                    double subTickspositions[]);

  /**
  * To be called when there are too many ticks.
  * Compute a new sequence of ticks with less ticks.
  */
  virtual void reduceTicksNumber(void);

  /**
  * To know if the ticks computer needs decimation.
  */
  virtual bool needTicksDecimation(void) {return false;}


  /**
  * To know if we need to get ticks exposants or not.
  */
  virtual bool isDisplayingLabelsExponents(void) {return false;}

protected:

  /*-----------------------------------------------------*/
  DrawableAxes * m_pAxes;
  /*-----------------------------------------------------*/


};

}

#endif /* _AXES_TICKS_COMPUTER_HXX_ */
