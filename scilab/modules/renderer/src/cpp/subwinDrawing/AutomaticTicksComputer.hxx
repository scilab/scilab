/*------------------------------------------------------------------------*/
/* file: AutomaticTicksComputer.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute automatic ticks                                         */
/*------------------------------------------------------------------------*/

#ifndef _AUTOMATIC_TICKS_COMPUTER_HXX_
#define _AUTOMATIC_TICKS_COMPUTER_HXX_

#include "ComputeTicksStrategy.hxx"

namespace sciGraphics
{

class AutomaticTicksComputer: public ComputeTicksStrategy
{
public:

  AutomaticTicksComputer(DrawableSubwin * subwin);

  virtual ~AutomaticTicksComputer(void);

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
  virtual bool needTicksDecimation(void) {return true;}

  /**
   * Set bounds between which we must compute ticks.
   */
  virtual void setAxisBounds(double min, double max);

  /**
   * To know if we need to get ticks exposants or not.
   */
  virtual bool isDisplayingLabelsExponents(void) {return false;}

protected:

  /** Current number of ticks */
  int m_iNbTicks;

  double m_dMinBounds;
  double m_dMaxBounds;

};

}

#endif /* _AUTOMATIC_TICKS_COMPUTER_HXX_ */
