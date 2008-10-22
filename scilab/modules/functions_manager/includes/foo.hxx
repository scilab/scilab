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

#ifndef __FOO_HH__
#define __FOO_HH__

#include <iostream>

#include "alltypes.hxx"
#include "scilabFunction.hxx"

class Foo : public ScilabFunction {

public :
  ReturnValue call(types::typed_list &in, int _iRetCount, types::typed_list &out);

public :
  ReturnValue callUntyped(types::GenericType *in, int _iRetCount, types::typed_list &out);

  ReturnValue callout(int _iRetCount, types::String *out);
  ReturnValue call(types::String *in);
  ReturnValue call(types::Int *in);
};

#endif /* !__FOO_HH__ */
