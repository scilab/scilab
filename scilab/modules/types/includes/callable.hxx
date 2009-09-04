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

#ifndef __CALLABLE_HXX__
#define __CALLABLE_HXX__

#include <string>
#include <cstdio>
#include "types.hxx"
#include <map>

namespace types
{
  class Callable : public InternalType
  {
  public :
    enum ReturnValue
    {
      OK,
			OK_NoResult,
      Error
    };

    Callable * 	getAsCallable() { return this; }
    bool isCallable() { return true; }

    virtual ReturnValue call(typed_list &in, int _iRetCount, typed_list &out) = 0;

    Callable(): InternalType() {}
    virtual ~Callable() {}
  };
}


#endif /* !__CALLABLE_HXX__ */
