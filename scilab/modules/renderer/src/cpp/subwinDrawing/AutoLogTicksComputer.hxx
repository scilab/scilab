/*------------------------------------------------------------------------*/
/* file: AutoLogTicksComputer.hxx                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute automatic ticks with logarithmic scaling                */
/*------------------------------------------------------------------------*/

#ifndef _AUTO_LOG_TICKS_COMPUTER_HXX_
#define _AUTO_LOG_TICKS_COMPUTER_HXX_

#include "AutomaticTicksComputer.hxx"

namespace sciGraphics
{

class AutoLogTicksComputer: public AutomaticTicksComputer
{
public:

  AutoLogTicksComputer(DrawableSubwin * subwin);

  virtual ~AutoLogTicksComputer(void);

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
   * Set bounds between which we must compute ticks.
   */
  virtual void setAxisBounds(double min, double max);

  /**
  * To know if we need to get ticks exposants or not.
  */
  virtual bool isDisplayingLabelsExponents(void) {return true;}

protected:

};

}

#endif /* _AUTO_LOG_TICKS_COMPUTER_HXX_ */
