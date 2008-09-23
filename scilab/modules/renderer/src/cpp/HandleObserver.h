/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Observer observing a sciPointObj object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _HANDLE_OBSERVER_H_
#define _HANDLE_OBSERVER_H_

extern "C"
{
#include "ObjectStructure.h"
}

#include "Observer.h"

namespace sciGraphics
{

/**
 * Class observing handles (sciPointObj *).
 */
class HandleObserver : public Observer
{
public:

  /**
   * Constructor with the link to the observed object.
   */
  HandleObserver( sciPointObj * subject ) { m_pSubject = subject ; }

  /**
   * Destructor
   */
  virtual ~HandleObserver( void ) {}
  
protected:

  /**
   * The observed Object
   */
  sciPointObj * m_pSubject ;

} ;

}

#endif /* _HANDLE_OBSERVER_H_ */
