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
    List();
    ~List(); 

  private :
    List(List *_oListCopyMe);
    std::list<InternalType *> *getData();

  public :
    int size_get(); 

    void whoAmI(void) { std::cout << "types::List"; };

    RealType getType(void) { return RealList; } 

  /**
   ** append(InternalType *_typedValue)
   ** Append the given value to the end of the List
   */
    void append(InternalType *_typedValue);

  /**
   ** Clone
   ** Create a new List and Copy all values.
   */
    List *clone();

    std::string toString(int _iPrecision, int _iLineLen);

    List *getAsList(void) { return this; }

  private :
    std::list<InternalType *> *m_plData;
  };
}
