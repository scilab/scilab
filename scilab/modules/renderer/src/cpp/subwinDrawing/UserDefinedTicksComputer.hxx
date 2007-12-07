/*------------------------------------------------------------------------*/
/* file: UserDefinedTicksComputer.hxx                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute ticks from ticks given by the user                      */
/*------------------------------------------------------------------------*/

#ifndef _USER_DEFINED_TICKS_COMPUTER_HXX_
#define _USER_DEFINED_TICKS_COMPUTER_HXX_

#include "ComputeTicksStrategy.hxx"

namespace sciGraphics
{

class UserDefinedTicksComputer: public ComputeTicksStrategy
{
public:

  UserDefinedTicksComputer(DrawableSubwin * subwin);

  virtual ~UserDefinedTicksComputer(void);

  /**
   * Get the number of ticks which will be computed by the strategy.
   */
  virtual int getNbTicks(void);

  /**
   * Fill positions parameters
   * with the ticks abscissas, ordinate, ...
   */
  virtual void getTicksPosition(double positions[]);

  /**
   * Get the label to display for each ticks.
   */
  virtual void getTicksLabels(char * labels[]);

  /**
   * Get the number of subticks which will be computed by the strategy.
   */
  virtual int getNbSubticks(void);

  /**
  * Get the subticks positions.
  */
  virtual void getSubticksPosition(double positions[]);

  /**
   * Set the parameter for drawing user ticks.
   * @param nbSubticks number of subticks between each ticks.
   */
  void setUserTicks(double * userTicksPos, char ** userLabels,
                    int nbUserTicks, int nbSubticks);

protected:

  double * m_aUserTicksPositions;
  char ** m_aUserTicksLabels;
  int m_iNbUserTicks;
  int m_iNbSubticks;

};

}

#endif /* _USER_DEFINED_TICKS_COMPUTER_HXX_ */
