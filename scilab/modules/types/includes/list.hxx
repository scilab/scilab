/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include <list>
#include "container.hxx"

namespace types
{
  class List : public Container
  {
  public :
    List() : Container() 
    {
      m_plData = new std::list<InternalType *>();
    }

    ~List() 
    {
      delete m_plData;
    }

    int size_get() 
    {
      return m_plData->size();
    }

  private :
    std::list<InternalType *> *m_plData;
  };
}
