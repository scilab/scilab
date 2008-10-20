/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_HH__
#define __TYPES_HH__

#include <vector>
#include <iostream>
#include <string.h>
#include "internal.hxx"
#include "anytype.hxx"

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

  public :

  protected :
    GenericType() {}

  public :
    virtual void whoAmI(void) { std::cout << "types::GenericType" << std::endl; }

		/*sub Type management*/
    /* String */
    bool isString(void) { return (getType() == RealString); }

    /* Double */
    bool isDouble(void) { return (getType() == RealDouble); }

    /* Float */
    bool isFloat(void) { return (getType() == RealFloat); }

    /* Int */
    bool isInt(void) { return (getType() == RealInt); }

    /* UInt */
    bool isUInt(void) { return (getType() == RealUInt); }

    /* Bool */
    bool isBool(void) { return (getType() == RealBool); }

		/*commun functions*/
		int cols_get();
		int rows_get();
		int size_get();

    virtual bool isAssignable(void) { return true; }

		virtual RealType getType(void) { return RealGeneric; }
  protected :
  };


  /*
  ** List of types
  */
  typedef std::vector<GenericType *> typed_list;

}
#endif /* !__TYPES_HH__ */
