/*------------------------------------------------------------------------*/
/* file: UserDefLogTicksComputer.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute ticks from ticks given by the user in logarithmic mode  */
/*------------------------------------------------------------------------*/

#ifndef _USER_DEF_LOG_TICKS_COMPUTER_HXX_
#define _USER_DEF_LOG_TICKS_COMPUTER_HXX_

#include "UserDefinedTicksComputer.hxx"

namespace sciGraphics
{

class UserDefLogTicksComputer: public UserDefinedTicksComputer
{
public:

  UserDefLogTicksComputer(DrawableSubwin * subwin);

  virtual ~UserDefLogTicksComputer(void);

  /**
  * Fill positions parameters
  * with the ticks abscissas, ordinate, ...
  */
  virtual void getTicksPosition(double positions[], char * labels[], char * labelsExponents[]);

  /**
  * Get the subticks positions.
  */
  virtual void getSubticksPosition(const double ticksPositions[], int nbTicks,
                                   double subTickspositions[]);


protected:

};

}

#endif /* _USER_DEF_LOG_TICKS_COMPUTER_HXX_ */
