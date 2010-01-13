/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_HXX__
#define __TYPES_HXX__

#include <vector>
#include <iostream>
#include <string>
#include "internal.hxx"

using namespace std;

namespace types
{
  /*
  ** Type
  */
  class GenericType : public InternalType
  {

  protected :
    int m_iRows;
    int m_iCols;
    int m_iSize;

    GenericType() : InternalType(), m_iRows(0), m_iCols(0), m_iSize(0) {}
    virtual ~GenericType() {}

  public :
    void whoAmI(void) { std::cout << "types::GenericType"; }
    
    /*commun functions*/
    int cols_get();
    int rows_get();
    int size_get();

    std::string DimToString();

    /* FIXME : should be : virtual GenericType*	get(int _iPos) = 0; */
    virtual GenericType*	get(int _iPos) { return NULL; }

    bool isIdentity(void);
    virtual bool isAssignable(void) { return true; }

    virtual RealType getType(void) { return RealGeneric; }
  };
}
#endif /* !__TYPES_HXX__ */
