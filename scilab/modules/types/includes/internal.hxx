/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/


#ifndef __INTERNAL_HXX__
#define __INTERNAL_HXX__

#include <vector>
#include <iostream>
#include <string.h>
#include "anytype.hxx"

namespace types
{
	class InternalType
  {
  public :
    enum RealType {
      RealInt,
      RealString,
      RealDouble,
			RealUInt,
			RealBool,
			RealFloat,
      RealGeneric,
			RealUserType,
			RealImplicitList,
			RealInternal
    };

    virtual bool isAssignable(void) { return false; }
		virtual RealType getType(void) { return RealInternal; }

    bool isString(void) { return (getType() == RealString); }
    virtual String* getAsString(void) { return NULL; }

    /* Double */
    bool isDouble(void) { return (getType() == RealDouble); }
    virtual Double* getAsDouble(void) { return NULL; }

    /* Float */
    bool isFloat(void) { return (getType() == RealFloat); }
    virtual Float* getAsFloat(void) { return NULL; }

    /* Int */
    bool isInt(void) { return (getType() == RealInt); }
    virtual Int* getAsInt(void) { return NULL; }

    /* UInt */
    bool isUInt(void) { return (getType() == RealUInt); }
    virtual UInt* getAsUInt(void) { return NULL; }

    /* Bool */
    bool isBool(void) { return (getType() == RealBool); }
    virtual Bool* getAsBool(void) { return NULL; }

    /* ImplicitList */
		bool isList(void) { return (getType() == RealImplicitList); }
		virtual ImplicitList* getAsList(void) { return NULL; }

  protected :
    InternalType() {}
	};
}
#endif /* !__INTERNAL_HXX__ */
