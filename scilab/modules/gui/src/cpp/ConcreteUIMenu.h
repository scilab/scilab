/*------------------------------------------------------------------------*/
/* file: ConcreteUIMenu.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : Class containing driver independent algorithms to draw a        */
/*        uimenu handle                                                   */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_UIMENU_H_
#define _CONCRETE_UIMENU_H_

#include "UIMenu.h"

namespace sciGraphics
{

class ConcreteUIMenu : public UIMenu
{

public:

  ConcreteUIMenu( sciPointObj * pObj ) : UIMenu(pObj) {}

  virtual ~ConcreteUIMenu( void ) {}

};

}

#endif /* _CONCRETE_UIMENU_H_ */
